#include "framealigner.h"

/*
frameAligner::frameAligner()
{
    exc.setModuleName("frameAligner");
    prevFrame=NULL;
    currentFrame=NULL;
    hMethod=featureBased;
    keyRetainFactor=0.75;
    if(!initModule_nonfree()){
        exc.showException("Non free Opsiyonlu derlenmemiş " );

    }


}

void frameAligner::add(cv::Mat &frame)
{
    if(currentFrame==NULL){
        init(frame)
        return;
    }
    prevFrame=currentFrame;
    keypointsPrev=keypointsCurrent;
    descriptorsPrev=descriptorsCurrent;

    frame.copyTo(currentFrame);
    process();

    if(hMethod==featureBased)
        homography=featureBasedHomography();
    else
        homography=flowBasedHomography();

}

void frameAligner::init(cv::Mat &frame)
{
    frame.copyTo(prevFrame);
    if(hMethod==featureBased){
        detector->detect(prevFrame,keypointsPrev);
        cv::KeyPointsFilter::retainBest(keypointsPrev,keyRetainFactor*keypointsPrev.size() );
        descriptor->compute(prevFrame,keypointsPrev,descriptorsPrev);
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
}

*/
