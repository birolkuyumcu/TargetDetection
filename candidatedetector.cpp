#include "candidatedetector.h"

CandidateDetector::CandidateDetector()
{
    exc.setModuleName("CandidateDetector");
    settings.minWidth = 3;
    settings.maxWidth = 50;
    settings.minHeight = 3;
    settings.maxHeight = 150;

}


void CandidateDetector::process(cv::Mat inputImage)
{
    std::vector<cv::Vec4i> hierarchy;
    std::vector<std::vector<cv::Point> > contours;

    if( inputImage.empty() )
    {
        exc.showException("Input Image is Empty !" );
    }
    else if( inputImage.channels() != 1)
    {
        exc.showException("Input Image must be a Gray Level ( single channel )!" );
    }
    else
    {

        candidateList.clear();
        candidateRRectsList.clear();

        findContours( inputImage, contours, hierarchy, CV_RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);


        for(int i=0 ; i < contours.size() ; i++ )
        {
            std::vector<cv::Point> tempContour;
            cv::approxPolyDP(cv::Mat(contours[i]),tempContour, 3, true);
            cv::RotatedRect tempRect=cv::minAreaRect(contours[i]);
            // width not always less than height so for filtering reverse it if required
            int tempWidth;
            int tempHeight;
            if(tempRect.size.width < tempRect.size.height )
            {
                tempWidth=tempRect.size.width;
                tempHeight=tempRect.size.height;
            }
            else
            {
                tempWidth=tempRect.size.height;
                tempHeight=tempRect.size.width;
            }

            if( (tempWidth < settings.maxWidth  && tempWidth > settings.minWidth) && (tempHeight < settings.maxHeight  && tempHeight > settings.minHeight) )
            {
                candidateList.push_back(tempContour);
                candidateRRectsList.push_back(tempRect);
            }

        }
    }


}

void CandidateDetector::showCandidates(cv::Mat inputImage, char *wName)
{
    // for debuging

    if(inputImage.channels() == 1)
    {
        cv::cvtColor(inputImage,inputImage,CV_GRAY2RGB);
    }
    for( int i = 0; i < candidateList.size() ; i++ )
    {

        cv::Point2f vertices[4];
        candidateRRectsList[i].points(vertices);
        for (int i = 0; i < 4; i++)
        {
            cv::line(inputImage, vertices[i], vertices[(i+1)%4], cv::Scalar(0,255,0));
        }
        cv::circle(inputImage,candidateRRectsList[i].center,3,cv::Scalar(0,0,255),-1);
    }
    if(wName==NULL)
        wName="Candidates";
    imshow(wName, inputImage );

}

