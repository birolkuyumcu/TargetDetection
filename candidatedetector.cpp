#include "candidatedetector.h"

/* Constructor
 * Initiate default settings
*/
CandidateDetector::CandidateDetector()
{
    exc.setModuleName("CandidateDetector");
    settings.minWidth = 3;
    settings.maxWidth = 50;
    settings.minHeight = 3;
    settings.maxHeight = 150;
    iWidth = 0; // means uninitialized
    scaleFactor = 0.0;  // means uninitialized
}

/* find contours from image
 * approx to Poly and Rotated Rect
 * and filter by using dimentional filter
 * put them into candidateList
 * inputImage ; must be gray level and single channel
 *
*/
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
        // record Width of Input image to predicta scale factor
        // aspect ratio dont be changed
        if(iWidth == 0)
        {
            iWidth = inputImage.size().width;
        }

        candidateList.clear();
        // CV_RETR_EXTERNAL retrieves only the extreme outer contours.
        findContours( inputImage, contours, hierarchy, CV_RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);


        for(unsigned int i=0 ; i < contours.size() ; i++ )
        {
            std::vector<cv::Point> tempContour;
            cv::approxPolyDP(cv::Mat(contours[i]),tempContour, 3, true);
            cv::RotatedRect tempRect=cv::minAreaRect(contours[i]);
            Candidate tempCandidate;
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
                tempCandidate.contour=tempContour;
                tempCandidate.rRect=tempRect;
                candidateList.push_back(tempCandidate);
            }

        }
    }


}

/* for Debugging Purposes
 * draw center and outer lines of RRects of Candidates from List
 * to inputImage
 * and show it wName Window
*/
void CandidateDetector::showCandidates(cv::Mat& inputImage, char *wName)
{
    // for debuging

    if ( scaleFactor == 0.0)
    {
        scaleFactor = inputImage.size().width / iWidth;
    }

    if(inputImage.channels() == 1)
    {
        cv::cvtColor(inputImage,inputImage,CV_GRAY2RGB);
    }

    for(unsigned int j = 0; j < candidateList.size() ; j++ )
    {

        cv::Point2f vertices[4];
        candidateList[j].rRect.points(vertices);
        for (int i = 0; i < 4; i++)
        {
            cv::line(inputImage, scaleFactor*vertices[i], scaleFactor*vertices[(i+1)%4], cv::Scalar(0,255,0));
        }
        cv::circle(inputImage,scaleFactor*candidateList[j].rRect.center,3,cv::Scalar(0,0,255),-1);
    }
    if(wName!= NULL)
      imshow(wName, inputImage );

}

