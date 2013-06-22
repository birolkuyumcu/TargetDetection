#include "framealigner.h"

<<<<<<< HEAD


=======
/*
>>>>>>> 0c3d3e095355a8442dfc8c8b7d7198e7dad9713e
frameAligner::frameAligner()
{
//    exc.setModuleName("frameAligner");
    prevFrame=NULL;
    currentFrame=NULL;
    //
    hMethod=featureBased;
    keyRetainFactor=0.75;
    homographyCalcMethod=CV_RANSAC;
    ransacReprojThreshold=3;


    if(!initModule_nonfree()){
//        exc.showException("Compiled without Non-free Option ! " );

    }


}

void frameAligner::add(cv::Mat &frame)
{
    if(prevFrame.empty()){
        init(frame);
        return;
    }
    if(!currentFrame.empty()){
         if(hMethod==featureBased){
            prevFrame=currentFrame;
            keypointsPrev=keypointsCurrent;
            descriptorsPrev=descriptorsCurrent;
         }
         else if(hMethod==flowBased){
             prevFrame=currentFrame;
             pointsPrev=pointsCurrent;
         }
    }
    frame.copyTo(currentFrame);
    process();

    if(hMethod==featureBased)
        featureBasedHomography();
    else
        flowBasedHomography();

}

void frameAligner::init(cv::Mat &frame)
{
    frame.copyTo(prevFrame);
    if(hMethod==featureBased){
        detector->detect(prevFrame,keypointsPrev);
        cv::KeyPointsFilter::retainBest(keypointsPrev,keyRetainFactor*keypointsPrev.size() );
        descriptor->compute(prevFrame,keypointsPrev,descriptorsPrev);
    }
    else if(hMethod==flowBased){
        detector->detect(prevFrame,keypointsPrev);
        cv::KeyPointsFilter::retainBest(keypointsPrev,keyRetainFactor*keypointsPrev.size() );
        for(unsigned int i=0;i<keypointsPrev.size();i++)
            pointsPrev.push_back(keypointsPrev[i].pt);
        cv::cornerSubPix(prevFrame,pointsPrev,cv::Size(5,5),cv::Size(-1,-1),cv::TermCriteria(cv::TermCriteria::MAX_ITER+cv::TermCriteria::EPS,30,0.1));

    }

//
}

void frameAligner::process()
{
    if(hMethod==featureBased){
        detector->detect(currentFrame,keypointsCurrent);
        cv::KeyPointsFilter::retainBest(keypointsCurrent,keyRetainFactor*keypointsCurrent.size() );
        descriptor->compute(currentFrame,keypointsCurrent,descriptorsCurrent);
    }
    else if(hMethod==flowBased){
        if(pointsPrev.size() < minimumFlowPoint){
            detector->detect(prevFrame,keypointsPrev);
            cv::KeyPointsFilter::retainBest(keypointsPrev,keyRetainFactor*keypointsPrev.size() );
            for(unsigned int i=0;i<keypointsPrev.size();i++)
                pointsPrev.push_back(keypointsPrev[i].pt);
            cv::cornerSubPix(prevFrame,pointsPrev,cv::Size(5,5),cv::Size(-1,-1),cv::TermCriteria(cv::TermCriteria::MAX_ITER+cv::TermCriteria::EPS,30,0.1));
        }

    }

}

void frameAligner::featureBasedHomography()
{
    std::vector< DMatch > matches,matches12,matches21;
    // Simetri testi
    matcher->match( descriptorsPrev, descriptorsCurrent, matches12 );
    matcher->match( descriptorsCurrent, descriptorsPrev, matches21 );
    for( size_t i = 0; i < matches12.size(); i++ )
    {
      DMatch forward = matches12[i];
      DMatch backward = matches21[forward.trainIdx];
        if( backward.trainIdx == forward.queryIdx )
            matches.push_back( forward );
    }
    pointsPrev.clear();
    pointsCurrent.clear();

    for (int p = 0; p < (int)matches.size(); ++p)
    {
        pointsPrev.push_back(keypointsPrev[matches[p].queryIdx].pt);
        pointsCurrent.push_back(keypointsCurrent[matches[p].trainIdx].pt);
    }
    homography=cv::findHomography(pointsPrev,pointsCurrent,homographyCalcMethod,ransacReprojThreshold);


}

void frameAligner::flowBasedHomography()
{
    vector<uchar>status;
    vector<float>err;
    calcOpticalFlowPyrLK(prevFrame,currentFrame,pointsPrev,pointsCurrent,status,err);
    vector<Point2f>tempPrev;
    vector<Point2f>tempCurrent;
    for (unsigned int i=0; i<pointsPrev.size();i++) {
            if(status[i] && err[i]<=flowErrorThreshold){
                tempPrev.push_back(pointsPrev[i]);
                tempCurrent.push_back(pointsCurrent[i]);
            }
    }

    homography=cv::findHomography(tempPrev,tempCurrent,homographyCalcMethod,ransacReprojThreshold);
    pointsCurrent=tempCurrent;

}

<<<<<<< HEAD
void frameAligner::setDetector(cv::Ptr<FeatureDetector> idetector)
{
    detector=idetector;
}

void frameAligner::setDetectorSimple(char *detectorName)
{
    setDetector(cv::FeatureDetector::create(detectorName ));
}

void frameAligner::setDescriptor(cv::Ptr<DescriptorExtractor> idescriptor)
{
    descriptor=idescriptor;
}

void frameAligner::setDescriptorSimple(char *descriptorName)
{
    setDescriptor(cv::DescriptorExtractor::create(descriptorName));
}

void frameAligner::setMatcher(cv::Ptr<DescriptorMatcher> imatcher)
{
    matcher=imatcher;
}

void frameAligner::setMatcherSimple(char *matcherName)
{
    setMatcher(cv::DescriptorMatcher::create(matcherName));
}

void frameAligner::alignPrevFrame(cv::Mat &alignedPrev)
{
    std::vector<Point2f> prevCorners(4);
    prevCorners[0] = cvPoint(0,0);
    prevCorners[1] = cvPoint( prevFrame.cols, 0 );
    prevCorners[2] = cvPoint( prevFrame.cols, prevFrame.rows );
    prevCorners[3] = cvPoint( 0, prevFrame.rows );
    std::vector<Point2f> alignedCorners(4);

    perspectiveTransform( prevCorners, alignedCorners, homography);

    float x,y,minx, miny,maxx,maxy;
    minx=0;
    maxx=currentFrame.cols;
    miny=0;
    maxy=currentFrame.rows;

    for(int i=0; i<4; i++){
        x=alignedCorners[i].x;
        y=alignedCorners[i].y;
        if(x<minx)
            minx=x;
        if(x>maxx)
            maxx=x;
        if(y<miny)
            miny=y;
        if(y>maxy)
            maxy=y;
    }

    int MinX=cvFloor(minx);
    int MinY=cvFloor(miny);
    int MaxX=cvCeil(maxx);
    int MaxY=cvCeil(maxy);
    int tX=0-MinX;
    int tY=0-MinY;

    Mat tempImg;
    tempImg.create(Size(MaxX - MinX, MaxY-MinY), prevFrame.depth());
    Mat mask(tempImg,Rect(tX,tY,currentFrame.cols,currentFrame.rows));
    warpPerspective(prevFrame,mask,homography,mask.size(),INTER_LINEAR| CV_WARP_FILL_OUTLIERS);
    alignedPrev=mask;

}
=======
*/
>>>>>>> 0c3d3e095355a8442dfc8c8b7d7198e7dad9713e
