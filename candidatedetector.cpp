#include "candidatedetector.h"

CandidateDetector::CandidateDetector()
{
    exc.setModuleName("CandidateDetector");
    settings.minWidth = 4;
    settings.maxWidth = 50;
    settings.minHeight = 10;
    settings.maxHeight = 100;

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

            if( (tempRect.size.width < settings.maxWidth  && tempRect.size.width > settings.minWidth) && (tempRect.size.height < settings.maxHeight  && tempRect.size.height > settings.minHeight) )
            {
                candidateList.push_back(tempContour);
                candidateRRectsList.push_back(tempRect);
            }

        }
    }


}

void CandidateDetector::showCandidates(cv::Mat inputImage)
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
    imshow("Candidates", inputImage );

}

