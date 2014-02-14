#include "movevector.h"

MoveVector::MoveVector()
{
    exc.setModuleName("MoveVector");
    stepX = 20;
    stepY = 20;


}

void MoveVector::process(cv::Mat &inputImage)
{   
    if(pointsPrev.size() == 0 )
    {
 //       qDebug()<<"Init "<<"\n\n";
        currentFrame=inputImage.clone();
        init();
//        qDebug()<<"Init Ends "<<"\n\n";
    }
    else
    {
//        qDebug()<<"Process "<<"\n\n";
        prevFrame = currentFrame ;
        currentFrame = inputImage.clone();
        calc();
    }
}

void MoveVector::show()
{
    if(currentFrame.rows == 0)
        return;

    cv::Mat showFrame;
    cv::cvtColor(currentFrame, showFrame, CV_GRAY2BGR);

    cv::line(showFrame,cv::Point(centerX,centerY) ,cv::Point(cvRound(centerX+15*mDeltaX), cvRound(centerY+15*mDeltaY)),cv::Scalar(255,0,0),2);
    cv::line(showFrame,cv::Point(centerX,centerY) ,cv::Point(cvRound(centerX+1*mDeltaX), cvRound(centerY+1*mDeltaY)),cv::Scalar(0,0,255),4);
    cv::imshow("Move Vector Show ",showFrame);
    cv::waitKey(30);

}

void MoveVector::init()
{
    for(int i=stepX ; i < currentFrame.cols  ; i+=stepX)
    {
        for(int j=stepY ; j < currentFrame.rows ; j+=stepY)
        {
            pointsPrev.push_back(cvPoint(i,j));
        }

    }

    centerX=currentFrame.cols/2;
    centerY=currentFrame.rows/2;

}

void MoveVector::calc()
{

    std::vector<cv::Point2f> pointsCurrent;
    std::vector<uchar>status;
    std::vector<float>err;
    std::vector<cv::Point2f>tempPrev;
    std::vector<cv::Point2f>tempCurrent;

    std::vector<float>dXList;
    std::vector<float>dYList;
    double sumdX = 0;
    double sumdY = 0;
    int nPoints =0;

/*
    qDebug()<<"Calc : "<<pointsPrev.size()<<"\n\n";

    qDebug()<<"PrevFrame : "<<prevFrame.cols<<"\n\n";
    qDebug()<<"PrevFrame : "<<prevFrame.rows<<"\n\n";
    qDebug()<<"currentFrame : "<<currentFrame.cols<<"\n\n";
*/
    // flow calculation
    cv::calcOpticalFlowPyrLK(prevFrame, currentFrame, pointsPrev, pointsCurrent, status, err);

//    qDebug()<<"after OF  "<<"\n\n";

    //
    for (unsigned int i=0; i < pointsPrev.size(); i++)
    {
        if(status[i])  // Sadece OF hesaplananlar
        {
            double tempdX=pointsCurrent[i].x - pointsPrev[i].x;
            double tempdY=pointsCurrent[i].y - pointsPrev[i].y;
            dXList.push_back(tempdX);
            dYList.push_back(tempdY);
          //  qDebug()<<"tempdX : "<<tempdX<<"\n\n";
            sumdX += tempdX;
            sumdY += tempdY;
            nPoints += 1;

        }

    }

    mDeltaX = sumdX / nPoints ;
    mDeltaY = sumdY / nPoints ;


}
