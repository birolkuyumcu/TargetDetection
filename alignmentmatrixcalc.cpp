#include "alignmentmatrixcalc.h"

AlignmentMatrixCalc::AlignmentMatrixCalc()
{
    exc.setModuleName("AlignmentMatrixCalc");

    hMethod = featureBased;
    keyRetainFactor=0.75;
    homographyCalcMethod=CV_RANSAC;
    ransacReprojThreshold = 3;

    matchType=normal;

    setDetectorSimple("SURF");
    setDescriptorSimple("SURF");

    setMatcherSimple("BruteForce-L1");
    isHomographyCalc=false;

    if(!cv::initModule_nonfree())
    {
        exc.showException("Compiled without Non-free Option!" );
    }
}

void AlignmentMatrixCalc::process(cv::Mat &inputImage)
{
    if(!prevFrame.empty())
    {
        if(!currentFrame.empty())
        {
            if(hMethod == featureBased)
            {
                prevFrame = currentFrame;
                keypointsPrev = keypointsCurrent;
                descriptorsPrev = descriptorsCurrent;
            }
            else if(hMethod == flowBased)
            {
                prevFrame = currentFrame;
                pointsPrev = pointsCurrent;
            }
        }

        inputImage.copyTo(currentFrame);
        run();

        if(hMethod == featureBased)
        {
            featureBasedHomography();
        }
        else
        {
            flowBasedHomography();
        }
    }
    else // if(!prevFrame.empty()){
    {
        init(inputImage);
    }
}

void AlignmentMatrixCalc::init(cv::Mat &frame)
{
    frame.copyTo(prevFrame);

    if(hMethod == featureBased)
    {
        detector->detect(prevFrame,keypointsPrev);
        cv::KeyPointsFilter::retainBest(keypointsPrev,keyRetainFactor*keypointsPrev.size() );
        descriptor->compute(prevFrame,keypointsPrev,descriptorsPrev);
    }
    else if(hMethod == flowBased)
    {
        detector->detect(prevFrame, keypointsPrev);
        cv::KeyPointsFilter::retainBest(keypointsPrev, keyRetainFactor*keypointsPrev.size() );

        for(unsigned int i = 0; i<keypointsPrev.size(); i++)
        {
            pointsPrev.push_back(keypointsPrev[i].pt);
        }

        cv::cornerSubPix(prevFrame, pointsPrev, cv::Size(5,5), cv::Size(-1,-1),
                         cv::TermCriteria(cv::TermCriteria::MAX_ITER+cv::TermCriteria::EPS, 30, 0.1));

    }
}

void AlignmentMatrixCalc::run()
{
    if(hMethod == featureBased)
    {
        detector->detect(currentFrame, keypointsCurrent);
        cv::KeyPointsFilter::retainBest(keypointsCurrent, keyRetainFactor*keypointsCurrent.size() );
        descriptor->compute(currentFrame, keypointsCurrent, descriptorsCurrent);
    }
    else if(hMethod == flowBased)
    {
        if(pointsPrev.size() < minimumFlowPoint)
        {
            detector->detect(prevFrame, keypointsPrev);
            cv::KeyPointsFilter::retainBest(keypointsPrev, keyRetainFactor*keypointsPrev.size() );

            for(unsigned int i=0; i < keypointsPrev.size(); i++)
            {
                pointsPrev.push_back(keypointsPrev[i].pt);
            }

            cv::cornerSubPix(prevFrame,pointsPrev,cv::Size(5,5),cv::Size(-1,-1),cv::TermCriteria(cv::TermCriteria::MAX_ITER+cv::TermCriteria::EPS,30,0.1));
        }

    }

}

void AlignmentMatrixCalc::featureBasedHomography()
{
    // Matching Section start
    // Re-writed
    // there is a another matchig system exist
    /*
      matcher->knnMatch();
      matcher->radiusMatch();
    */

    std::vector<cv::DMatch> matchesPrevToCurrent;
    std::vector<cv::DMatch> matchesCurrentToPrev;
    std::vector<cv::DMatch> matchesPassed;

    matcher->match( descriptorsPrev, descriptorsCurrent, matchesPrevToCurrent );
    matcher->match( descriptorsCurrent, descriptorsPrev, matchesCurrentToPrev );

    // Symetri Test start
    symmetryTest(matchesPrevToCurrent,matchesCurrentToPrev,matchesPassed);


    // Matching Section end

    pointsPrev.clear();
    pointsCurrent.clear();

    for (int p = 0; p < (int)matchesPassed.size(); ++p)
    {
        pointsPrev.push_back(keypointsPrev[matchesPassed[p].queryIdx].pt);
        pointsCurrent.push_back(keypointsCurrent[matchesPassed[p].trainIdx].pt);
    }

    // Sub-pixsel Accuracy
    cv::cornerSubPix(prevFrame,pointsPrev,cv::Size(5,5),cv::Size(-1,-1),cv::TermCriteria(cv::TermCriteria::MAX_ITER+cv::TermCriteria::EPS,30,0.1));
    cv::cornerSubPix(currentFrame,pointsCurrent,cv::Size(5,5),cv::Size(-1,-1),cv::TermCriteria(cv::TermCriteria::MAX_ITER+cv::TermCriteria::EPS,30,0.1));


    if(pointsPrev.size() !=0 && pointsCurrent.size() != 0)
    {
        homography = cv::findHomography(pointsPrev, pointsCurrent, homographyCalcMethod, ransacReprojThreshold);
        /*
         cv::findHomography can return empty matrix in some cases.
         This seems happen only when cv::RANSAC flag is passed.
         check the computed homography before using it
         */
        if(!homography.empty())
        {
           isHomographyCalc = true;
        }
        else
        {
            isHomographyCalc = false;
        }

    }
    else
    {
        isHomographyCalc = false;
    }

}

void AlignmentMatrixCalc::flowBasedHomography()
{
    std::vector<uchar>status;
    std::vector<float>err;
    std::vector<cv::Point2f>tempPrev;
    std::vector<cv::Point2f>tempCurrent;

    calcOpticalFlowPyrLK(prevFrame, currentFrame, pointsPrev, pointsCurrent, status, err);

    for (unsigned int i=0; i<pointsPrev.size(); i++)
    {
        if(status[i] && err[i] <= flowErrorThreshold)
        {
            tempPrev.push_back(pointsPrev[i]);
            tempCurrent.push_back(pointsCurrent[i]);
        }
    }


    homography = cv::findHomography(tempPrev, tempCurrent, homographyCalcMethod, ransacReprojThreshold);
    pointsCurrent = tempCurrent;
    isHomographyCalc=true;
    // Burda bir kontrol eklenmeli filitrelenen noktaların belli bir sayı altına düştüğünde
    // yeniden feature bazlı nokta tespit ettirilmeli
    if(pointsCurrent.size()<50)
    {
        init(currentFrame);
    }

}


void AlignmentMatrixCalc::setDetector(cv::Ptr<cv::FeatureDetector> idetector)
{
    detector = idetector;
}

void AlignmentMatrixCalc::setDetectorSimple(const char *detectorName)
{
    setDetector(cv::FeatureDetector::create(detectorName ));
}

void AlignmentMatrixCalc::setDescriptor(cv::Ptr<cv::DescriptorExtractor> idescriptor)
{
    descriptor=idescriptor;
}

void AlignmentMatrixCalc::setDescriptorSimple(const char *descriptorName)
{
    setDescriptor(cv::DescriptorExtractor::create(descriptorName));
}

void AlignmentMatrixCalc::setMatcher(cv::Ptr<cv::DescriptorMatcher> imatcher)
{
    matcher = imatcher;
}

void AlignmentMatrixCalc::setMatcherSimple(const char *matcherName)
{
    setMatcher(cv::DescriptorMatcher::create(matcherName));
}

void AlignmentMatrixCalc::setHomographyMethod(HomograpyMethod ihMethod)
{
    hMethod=ihMethod;
}

void AlignmentMatrixCalc::setHomographyCalcMethod(int ihomographyCalcMethod)
{
    /*
    – 0 - a regular method using all the points
    – CV_RANSAC - RANSAC-based robust method
    – CV_LMEDS - Least-Median robust method
    */

    homographyCalcMethod=ihomographyCalcMethod;
}

bool AlignmentMatrixCalc::getHomography(cv::Mat &gHomography)
{
    if(isHomographyCalc)
    {
        gHomography = homography;
    }

    return isHomographyCalc;
}

void AlignmentMatrixCalc::setMatchingType(MatchingType iType)
{
    matchType=iType;
}

void AlignmentMatrixCalc::symmetryTest(std::vector<cv::DMatch> &matchesPrevToCurrent, std::vector<cv::DMatch> &matchesCurrentToPrev, std::vector<cv::DMatch> &matchesPassed)
{
    for( size_t i = 0; i < matchesCurrentToPrev.size(); i++ )
    {

        cv::DMatch forward = matchesPrevToCurrent[i];
        cv::DMatch backward = matchesCurrentToPrev[forward.trainIdx];
        if( backward.trainIdx == forward.queryIdx && forward.distance<0.3)
        {
            matchesPassed.push_back( forward );
             std::cout<<forward.distance<<"\n"; // for debugging
        }

    }
}

