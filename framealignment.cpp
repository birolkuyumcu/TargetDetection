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

    perspectiveTransform( inputCorners, alignedCorners, homography);

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
