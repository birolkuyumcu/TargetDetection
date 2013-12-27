#include "alignmentmatrixcalc.h"

/* constructor sets default settings
 */
AlignmentMatrixCalc::AlignmentMatrixCalc()
{
    exc.setModuleName("AlignmentMatrixCalc");

    hMethod = featureBased;
    keyRetainFactor = 0.75;
    homographyCalcMethod = CV_RANSAC;
    ransacReprojThreshold = 3;

    matchType = normalMatch;
    maxRatio = 0.50;
    maxRadius = 100;

    setDetectorSimple("SURF");
    setDescriptorSimple("SURF");
    setMatcherSimple("BruteForce-L1");

    setMatcherSimple("BruteForce-L1");
    isHomographyCalc=false;
    stage = firstPass;
    numOfPointsMin = 50;
    errorCount=0;

    if(!cv::initModule_nonfree())
    {
        exc.showException("Compiled without Non-free Option!" );
    }
}

/* inputImage ; always same size ,single channel  and same depth CV_8U - 8-bit unsigned integers ( 0..255 )
 * main calculation method
 * gets frame and run with respect to stage of process
 * if firstPass ; run init()
 * elseif secondPass ; if error count above the threshold value bact to firstPass
 * else - secondPass or onGoing stage - run();
 *   if run() returns true calculate homography with respect to HomograpyMethod
 *   else isHomographyCalc set to false;
 *
 **/
void AlignmentMatrixCalc::process(cv::Mat &inputImage)
{
    if(inputImage.empty())
    {
        exc.showException("Empty Frame..." );
        return;
    }

    if(stage == secondPass)
    {
        errorCount++;
        if(errorCount >= 4)
        {
            stage=firstPass;
            errorCount=0;
        }

    }

    if(stage == firstPass)
    {
        init(inputImage);
    }
    else // secondPass or onGoing stage
    {
        if(stage == onGoing) // Third and so on passes
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
                // pointsPrev = pointsCurrent;
            }
        }

        inputImage.copyTo(currentFrame);

        if(run())
        {
            if(hMethod == featureBased)
            {
                featureBasedHomography();
            }
            else
            {
                flowBasedHomography();
            }
        }
        else //  false
        {
            //wayBack();
            isHomographyCalc = false;
            return;
        }


    }

}

/*delete buffers
 *set AlignmentMatrixCalc to initial state
 * Not used, can be removed...
 */
void AlignmentMatrixCalc::reset()
{

    prevFrame.release();
}

/* Internally used
 * firstPass stage of calculation
 */
void AlignmentMatrixCalc::init(cv::Mat &frame)
{
    qDebug()<<"Init\n\n";
    frame.copyTo(prevFrame);

    if(hMethod == featureBased)
    {
        detector->detect(prevFrame, keypointsPrev);

        cv::KeyPointsFilter::retainBest(keypointsPrev, keyRetainFactor*keypointsPrev.size() );
        // if enough points retained
        if(keypointsPrev.size() >= numOfPointsMin)
        {
            stage = secondPass;
            descriptor->compute(prevFrame, keypointsPrev, descriptorsPrev);
        }

    }
    else if(hMethod == flowBased)
    {
        detector->detect(prevFrame, keypointsPrev);

        cv::KeyPointsFilter::retainBest(keypointsPrev, keyRetainFactor*keypointsPrev.size() );
        // if enough points retained
        if(keypointsPrev.size() >= numOfPointsMin)
        {
            stage = secondPass;
            pointsPrev.clear();

            for(unsigned int i = 0; i<keypointsPrev.size(); i++)
            {
                pointsPrev.push_back(keypointsPrev[i].pt);
            }

            cv::cornerSubPix(prevFrame, pointsPrev, cv::Size(5,5), cv::Size(-1,-1),
                             cv::TermCriteria(cv::TermCriteria::MAX_ITER+cv::TermCriteria::EPS, 30, 0.1));

        }

    }
}


/* Internally used
 * if current frame points ready retruns true
 * if not retruns false
 **/
bool AlignmentMatrixCalc::run()
{
    bool returnValue=true;

    if(hMethod == featureBased)
    {
        // detect keyoints for current frame
        detector->detect(currentFrame, keypointsCurrent);
        // select best of them with respect to keyRetainFactor
        cv::KeyPointsFilter::retainBest(keypointsCurrent, keyRetainFactor*keypointsCurrent.size() );
        // if enough points retained
        if(keypointsCurrent.size() >= numOfPointsMin)
        {
            stage = onGoing;
            descriptor->compute(currentFrame, keypointsCurrent, descriptorsCurrent);
        }
        else
        {
            stage = secondPass;
            returnValue=false;
        }

    }
    else if(hMethod == flowBased)
    {
        detector->detect(prevFrame, keypointsPrev);

     //   qDebug()<<"Before retainBest :"<<keypointsPrev.size();
        cv::KeyPointsFilter::retainBest(keypointsPrev, keyRetainFactor*keypointsPrev.size() );

      //  cv::KeyPointsFilter::retainBest(keypointsPrev, 80 );
     //   qDebug()<<"After retainBest :"<<keypointsPrev.size();

        if(keypointsPrev.size() >= numOfPointsMin)
        {
            stage = onGoing;
            pointsPrev.clear();

            for(unsigned int i=0; i < keypointsPrev.size(); i++)
            {
                pointsPrev.push_back(keypointsPrev[i].pt);
            }

            cv::cornerSubPix(prevFrame, pointsPrev, cv::Size(5, 5), cv::Size(-1,-1),
                             cv::TermCriteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS,30,0.1));
        }
        else
        {
            stage = secondPass;
            returnValue=false;
        }



    }
    return returnValue;

}


void AlignmentMatrixCalc::featureBasedHomography()
{
    std::vector<cv::DMatch> matchesPrevToCurrent;
    std::vector<cv::DMatch> matchesCurrentToPrev;
    std::vector<std::vector<cv::DMatch> > kmatchesPrevToCurrent;
    std::vector<std::vector<cv::DMatch> > kmatchesCurrentToPrev;
    std::vector<cv::DMatch> matchesPassed;

    if( matchType == normalMatch )
    {
        matcher->match( descriptorsPrev, descriptorsCurrent, matchesPrevToCurrent );
        matcher->match( descriptorsCurrent, descriptorsPrev, matchesCurrentToPrev );
        // Symmetry Test start
        symmetryTest(matchesPrevToCurrent, matchesCurrentToPrev, matchesPassed);

    }
    else if( matchType == knnMatch)
    {
        qDebug()<<"Match : "<<keypointsCurrent.size()<<"  "<<keypointsPrev.size()<<"\n";
        matcher->knnMatch(descriptorsPrev, descriptorsCurrent, kmatchesPrevToCurrent,2);
        qDebug()<<"Ratio Test 1 :"<<kmatchesPrevToCurrent.size()<<"\n";
        ratioTest(kmatchesPrevToCurrent);
        qDebug()<<"Ratio Test 1 End :"<<kmatchesPrevToCurrent.size()<<"\n";
        matcher->knnMatch(descriptorsCurrent,descriptorsPrev, kmatchesCurrentToPrev, 2);
        qDebug()<<"Ratio Test 2 :"<<kmatchesCurrentToPrev.size()<<"\n";
        ratioTest(kmatchesCurrentToPrev);
        qDebug()<<"Ratio Test 2 End :"<<kmatchesCurrentToPrev.size()<<"\n";
        // Symmetry Test not working for knn
        //matchesPassed=matchesPrevToCurrent;
        symmetryTest(kmatchesPrevToCurrent,kmatchesCurrentToPrev,matchesPassed);
        qDebug()<<"Sym Test  :"<<matchesPassed.size()<<"\n";

    }
    else if( matchType == radiusMatch)
    {
        // there is no documentation
        // matcher->radiusMatch(descriptorsPrev, descriptorsCurrent, kmatchesPrevToCurrent,maxRadius );
        // work but there is no matching back for any maxRadius
        //convertRMatches(kmatchesCurrentToPrev,matchesPassed);
        exc.showException("radiusMatch not working Dont use it!" );

    }


    // Matching Section end

    pointsPrev.clear();
    pointsCurrent.clear();

    for (int p = 0; p < (int)matchesPassed.size(); ++p)
    {
        pointsPrev.push_back(keypointsPrev[matchesPassed[p].queryIdx].pt);
        pointsCurrent.push_back(keypointsCurrent[matchesPassed[p].trainIdx].pt);
    }




    if(pointsPrev.size() >=4 && pointsCurrent.size() >= 4)
    {
        // Sub-pixsel Accuracy

        cv::cornerSubPix(prevFrame, pointsPrev, cv::Size(5,5), cv::Size(-1,-1),
                         cv::TermCriteria(cv::TermCriteria::MAX_ITER+cv::TermCriteria::EPS,30,0.1));

        cv::cornerSubPix(currentFrame, pointsCurrent, cv::Size(5,5), cv::Size(-1,-1),
                         cv::TermCriteria(cv::TermCriteria::MAX_ITER+cv::TermCriteria::EPS,30,0.1));

        homography = cv::findHomography(pointsPrev, pointsCurrent, homographyCalcMethod,
                                        ransacReprojThreshold);
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
        stage=secondPass;
        qDebug()<<"Reset\n";
        //wayBack();
        return;
    }
    //If Homogrphy not valid

    if(!isHomographyValid())
    {
     //   stage=secondPass;
     //   wayBack();
    }


}

void AlignmentMatrixCalc::flowBasedHomography()
{
    std::vector<uchar>status;
    std::vector<float>err;
    std::vector<cv::Point2f>tempPrev;
    std::vector<cv::Point2f>tempCurrent;

    pointsCurrent.clear();
    calcOpticalFlowPyrLK(prevFrame, currentFrame, pointsPrev, pointsCurrent, status, err);

    qDebug()<<"\n\n Prev Frame Features: "<<pointsPrev.size();

    for (unsigned int i=0; i < pointsPrev.size(); i++)
    {
        if(status[i] && err[i] <= flowErrorThreshold)
        {
            tempPrev.push_back(pointsPrev[i]);
            tempCurrent.push_back(pointsCurrent[i]);
        }
    }
    qDebug()<<"After Flow Filtered Features : "<<tempCurrent.size();

    if(tempPrev.size() >= 4 && tempCurrent.size() >= 4)
    {
        homography = cv::findHomography(tempPrev, tempCurrent, homographyCalcMethod,
                                        ransacReprojThreshold);
      //  pointsCurrent = tempCurrent;
        isHomographyCalc=true;
        isHomographyValid();
    }
    else
    {
        isHomographyCalc=false;
    }




/*
    // Burda bir kontrol eklenmeli filitrelenen noktaların belli bir sayı altına düştüğünde
    // yeniden feature bazlı nokta tespit ettirilmeli
    if(pointsCurrent.size() < 50)
    {
        init(currentFrame);
    }

    //If Homogrphy not valid !
    /*if(isHomographyValid())
    {
      //  wayBack();
    }
*/
}


void AlignmentMatrixCalc::setDetector(cv::Ptr<cv::FeatureDetector> idetector)
{
    detector = idetector;
}

void AlignmentMatrixCalc::setDetectorSimple(QString detectorName)
{
    setDetector(cv::FeatureDetector::create(detectorName.toStdString()));
}

void AlignmentMatrixCalc::setDescriptor(cv::Ptr<cv::DescriptorExtractor> idescriptor)
{
    descriptor = idescriptor;
}

void AlignmentMatrixCalc::setDescriptorSimple(QString descriptorName)
{
    setDescriptor(cv::DescriptorExtractor::create(descriptorName.toStdString()));
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
    hMethod = ihMethod;
}

void AlignmentMatrixCalc::setHomographyCalcMethod(int ihomographyCalcMethod)
{
    /*
    – 0 - a regular method using all the points
    – CV_RANSAC - RANSAC-based robust method
    – CV_LMEDS - Least-Median robust method
    */

    homographyCalcMethod = ihomographyCalcMethod;
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
    matchType = iType;
}

void AlignmentMatrixCalc::symmetryTest(std::vector<cv::DMatch> &matchesPrevToCurrent, std::vector<cv::DMatch> &matchesCurrentToPrev, std::vector<cv::DMatch> &matchesPassed)
{
    for( size_t i = 0; i < matchesPrevToCurrent.size(); i++ )
    {

        cv::DMatch forward = matchesPrevToCurrent[i];
        cv::DMatch backward = matchesCurrentToPrev[forward.trainIdx];
        if( backward.trainIdx == forward.queryIdx && forward.trainIdx==backward.queryIdx)
        {
            matchesPassed.push_back( forward );

        }

    }
}

void AlignmentMatrixCalc::symmetryTest(std::vector<std::vector<cv::DMatch> >&kmatchesPrevToCurrent,std::vector<std::vector<cv::DMatch> >&kmatchesCurrentToPrev,std::vector< cv::DMatch >& matchesPassed)
{

    for(std::vector<std::vector<cv::DMatch> >::iterator mPi= kmatchesPrevToCurrent.begin(); mPi != kmatchesPrevToCurrent.end(); ++mPi)
    {
        if(mPi->size() < 2 )
        {
            continue;
        }

        for(std::vector<std::vector<cv::DMatch> >::iterator mCi= kmatchesCurrentToPrev.begin();
            mCi != kmatchesCurrentToPrev.end();
            ++mCi)
        {

            if(mCi->size() < 2 )
            {
                continue;
            }

            cv::DMatch forward = (*mPi)[0];
            cv::DMatch backward = (*mCi)[0];

            if((forward.queryIdx == backward.trainIdx) && (backward.queryIdx == forward.trainIdx) )
            {
                matchesPassed.push_back(forward);
                break;
            }
        }
    }

}

void AlignmentMatrixCalc::ratioTest(std::vector<std::vector<cv::DMatch> > &kmatches)
{
    for(std::vector<std::vector<cv::DMatch> >::iterator mi=kmatches.begin();
        mi != kmatches.end();
        ++mi)
    {
        if(mi->size() > 1)
        {
            const cv::DMatch& best = (*mi)[0];
            const cv::DMatch& good = (*mi)[1];

            assert(best.distance <= good.distance);

            float ratio = (best.distance / good.distance);

            if (ratio > maxRatio)
            {
                mi->clear();
            }
        }
        else
        {
            mi->clear();
        }

    }
}

bool AlignmentMatrixCalc::isHomographyValid()
{
    std::vector<cv::Point2f> inputCorners(4);
    inputCorners[0] = cvPoint(0,0);
    inputCorners[1] = cvPoint( prevFrame.cols, 0 );
    inputCorners[2] = cvPoint( prevFrame.cols, prevFrame.rows );
    inputCorners[3] = cvPoint( 0, prevFrame.rows );
    std::vector<cv::Point2f> alignedCorners(4);

    perspectiveTransform( inputCorners, alignedCorners, homography);

    float upDeltaX = fabs(alignedCorners[0].x-alignedCorners[1].x);
    float downDeltaX = fabs(alignedCorners[2].x-alignedCorners[3].x);
    float upDeltaY = fabs(alignedCorners[0].y-alignedCorners[3].y);
    float downDeltaY = fabs(alignedCorners[1].y-alignedCorners[2].y);
    float alignedCols=(upDeltaX+downDeltaX)/2;
    float alignedRows=(upDeltaY+downDeltaY)/2;
    float colsDifference=fabs(alignedCols - prevFrame.cols) / prevFrame.cols;
    float rowsDifference=fabs(alignedRows - prevFrame.rows) / prevFrame.rows;

    if( colsDifference < 0.1 && rowsDifference < 0.1 )
    {
       isHomographyCalc = true;
    }
    else
    {
       isHomographyCalc = false;
       qDebug()<<"Homography Matrix is Invalid : "<<colsDifference<<" "<<rowsDifference ;
    /*   errorCount++;
       if(errorCount >= 4)
       {
           stage=firstPass;
           errorCount=0;
       }
       */
    }

    return isHomographyCalc;

}

void AlignmentMatrixCalc::wayBack()
{
    if(hMethod == featureBased)
    {
       // return;
        currentFrame = prevFrame;
        keypointsCurrent = keypointsPrev;
        descriptorsCurrent = descriptorsPrev;
    }
    else if(hMethod == flowBased)
    {
        currentFrame = prevFrame;
        pointsCurrent = pointsPrev;
    }
}
