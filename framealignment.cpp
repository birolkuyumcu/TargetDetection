#include "framealignment.h"

FrameAlignment::FrameAlignment()
{
}

void FrameAlignment::process(cv::Mat &inputImage, cv::Mat &homography,cv::Mat &outputImage)
{
    std::vector<cv::Point2f> inputCorners(4);
    inputCorners[0] = cvPoint(0,0);
    inputCorners[1] = cvPoint( inputImage.cols, 0 );
    inputCorners[2] = cvPoint( inputImage.cols, inputImage.rows );
    inputCorners[3] = cvPoint( 0, inputImage.rows );
    std::vector<cv::Point2f> alignedCorners(4);
    float x, y, minx, miny, maxx, maxy;

    cv::perspectiveTransform( inputCorners, alignedCorners, homography);

    minx=0;
    maxx=inputImage.cols;
    miny=0;
    maxy=inputImage.rows;

    for(int i = 0; i < 4; i++)
    {
        x = alignedCorners[i].x;
        y = alignedCorners[i].y;

        if(x < minx)
        {
            minx = x;
        }
        if(x > maxx)
        {
            maxx = x;
        }
        if(y < miny)
        {
            miny = y;
        }
        if(y > maxy)
        {
            maxy = y;
        }
    }

    int MinX = cvFloor(minx);
    int MinY = cvFloor(miny);
    int MaxX = cvCeil(maxx);
    int MaxY = cvCeil(maxy);
    int tX = 0-MinX;
    int tY = 0-MinY;

    cv::Mat tempImg;
    tempImg.create(cv::Size((MaxX - MinX), (MaxY - MinY)), inputImage.depth());
    cv::Mat mask(tempImg, cv::Rect(tX,tY,inputImage.cols,inputImage.rows));
    warpPerspective(inputImage,mask,homography,mask.size(), cv::INTER_LINEAR | CV_WARP_FILL_OUTLIERS);
    outputImage = mask;

}

void FrameAlignment::calculateBinaryDiffImageAccording2pixelNeighborhood(cv::Mat& image1,
                                                                         cv::Mat& image2,
                                                                         cv::Mat& outputImage)
{

    int wMin;
    int wMax;
    int hMin;
    int hMax;

    int image1rowsCnt = image1.rows;
    int image1colsCnt = image1.cols;

    unsigned long rgbVectorValueForImg1;
    unsigned long rgbVectorValueForImg2;

    unsigned int closePixelFound = 0;

    outputImage.create(image1.size(), CV_8UC1);
    outputImage = cv::Scalar(0);


    for (int j = 1; j < image1rowsCnt - 1; j++) // for all rows    // (except first and last)
    {
        hMin = j - _CVS_PIXEL_NEIGHBORHOOD_DIST;
        hMax = j + _CVS_PIXEL_NEIGHBORHOOD_DIST;

        //check boundary conditions
        if(hMin < 0) hMin = 0;
        if(hMax >= image1rowsCnt) hMax = image1rowsCnt-1;

        for (int i = 1; i < image1colsCnt - 1; i++) // for all columns    // (except first and last)
        {

            wMin = i - _CVS_PIXEL_NEIGHBORHOOD_DIST;
            wMax = i + _CVS_PIXEL_NEIGHBORHOOD_DIST;

            //check boundary conditions
            if(wMin < 0) wMin = 0;
            if(wMax >= image1colsCnt) wMax = image1colsCnt-1;


            //the rgb vector for position i,j in image1
            rgbVectorValueForImg1 = 0;
            rgbVectorValueForImg1 = image1.at<char>(j, i);

            //loop for pixel neighborhood
            closePixelFound = 0;

            //scan a region on image2 to find a similar pixel feom image1
            for(int h = hMax; h >= hMin; h--)
            {
                for(int  w = wMax; w >= wMin; w--)
                {
                    rgbVectorValueForImg2 = 0;
                    rgbVectorValueForImg2 = image2.at<char>(h, w);

                    if( abs((long)(rgbVectorValueForImg2 - rgbVectorValueForImg1)) <= _CVS_IS_PIXEL_DIFFERENT_THRES)
                    {
                        //similar color pixel found
                        closePixelFound = 1;
                        break;
                    }
                }
                if(closePixelFound == 1)
                {
                    break;
                }
            }

            if(closePixelFound == 0)
            {
                //if a similar pixel is not found mark the position
                outputImage.at<char>(j, i) = 255;
            }


        }
    }

/*

    // Set the unprocess pixels to 0
    outputImage.row(0).setTo(cv::Scalar(0));
    outputImage.row(outputImage.rows - 1).setTo(cv::Scalar(0));
    outputImage.col(0).setTo(cv::Scalar(0));
    outputImage.col(outputImage.cols - 1).setTo(cv::Scalar(0));
    */

}
