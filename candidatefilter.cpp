#include "candidatefilter.h"

/* Constructor
 * Initiate default settings
*/
CandidateFilter::CandidateFilter()
{
    settings.distanceThreshold = 15;
    settings.visibilityThreshold = 5;
    settings.invisibilityThreshold = 5;
    settings.showCandidate = false;
    settings.showTargetId = false;
    settings.showVisible = true;
    settings.showInvisible = false;
    settings.showVector = true;
    targetIdCounter = 0;
}

/* gets CandidateList
 * if targetList empty call init()
 * match candidates and targtes call match ()
 * process Unmatched Targets call processUnmatchedTargets()
 * process Unmatched Candidates call processUnmatchedCandidates()
*/

void CandidateFilter::process(std::vector<Candidate> *iCandidateList)
{
    candidateList = iCandidateList;

    if(targetList.size() == 0)
    {
        init();
    }
    else if(candidateList->size() > 0)
    {
        match();
        processUnmatchedTargets();
        processUnmatchedCandidates();

    }
   // refine();
}

/*
 * for unmatched targets
 * if visible set to invisible
 * if invisible and  statuscounter > invisibilityThreshold  remove from targetList
 * if candidate remove from targetList
 *
*/
void CandidateFilter::processUnmatchedTargets()
{
    std::vector<Target>::iterator it;

    for(it = targetList.begin() ; it < targetList.end() ; ++it)
    {

        if(it->isMatched == false ) // Unmatched Targets
        {
            if(  it->status == visible)
            {
                it->status = invisible;
                it->statusCounter=1;

            }
            else if( it->status == invisible)
            {
                it->statusCounter++;
                if(it->statusCounter > settings.invisibilityThreshold )
                {
                    if(targetList.size()== 1)
                    {
                        targetList.clear();
                       // qDebug()<<"Satatus Invisible\n";
                        break;

                    }
                    else
                    {
                        it=targetList.erase(it); // after erase it points to nothing so incrimenting rises a error
                        qDebug()<<"CandidateFilter 81\n";
                        if(it != targetList.begin())
                            --it;
                    }

                }

            }
            else if (it->status == candidate)
            {
                if(targetList.size()== 1)
                {
                    targetList.clear();
                   // qDebug()<<"Satatus Candidate\n";
                    break;
                }
                else
                {
                    it=targetList.erase(it); // after erase it points to nothing so incrimenting rises a error
                    qDebug()<<"CandidateFilter 100\n";
                    if(it != targetList.begin())
                        --it;
                }
            }


        }
    }

}

/*
 * all records  from candidateList aded to targetList with status candidate
*/
void CandidateFilter::init()
{
    for(unsigned int i = 0; i<candidateList->size(); i++)
    {

        Target temp;
        temp.location = candidateList->at(i).rRect;
        temp.contour =candidateList->at(i).contour;
        temp.status = candidate;
        temp.statusCounter = 1;
        temp.targetId = ++targetIdCounter;
        temp.deltaX = 0 ;
        temp.deltaY = 0 ;
        temp.movingAngle = 0 ;

        if(isNewTarget(temp)) // if temp is not a subtarget of any other target
        {
            targetList.push_back(temp);
        }

    }
 }

/*
 * calculate a match table
 * for each target
 * to all candidate
 * calculate distance
 * if distance lower than  distanceThreshold
 * sort matchTable respect to distance
 * process table see below
 *
*/
void CandidateFilter::match()
{

    std::vector<MatchItem> matchTable;

    isCandidateMatched.reserve(candidateList->size());
    isCandidateMatched.assign(candidateList->size(), false);

    for(unsigned int j=0; j<targetList.size(); j++)
    {
        targetList.at(j).isMatched = false;

        for(unsigned int i = 0; i < candidateList->size(); i++)
        {

            MatchItem temp;
            temp.candidateIndex = i;
            temp.targetIndex = j;
            temp.distance = calculateDistance(candidateList->at(i).rRect,targetList.at(j).location);

            if(temp.distance <= settings.distanceThreshold ) // distanceThreshold dan büyük olan uzaklıkları tabloya ekleme
            {
               matchTable.push_back(temp);
            }

        }
    }


    /*
     * Process Matching Table
     */

    if(matchTable.size() != 0)
    {
        std::sort(matchTable.begin(), matchTable.end());

        std::vector<MatchItem>::iterator tableIndex;

        for( tableIndex = matchTable.begin(); tableIndex != matchTable.end(); ++tableIndex )
        {
            // for each element of table
            MatchItem temp = *tableIndex;
            int mTarget = temp.targetIndex;
            int mCandidate = temp.candidateIndex;

            // if canidate is matched before go next iteration
            if(isCandidateMatched[mCandidate]) // daha önceden eşleşen bir Candidate ise  bir sonraki iterasyona git
            {
                continue;
            }
            /* if Target matched before
             * if candidates near 1.1 distance of matching distance
             * set to a matched
             * aded to a matched target
             * recalculate contour and rRect
             * to unifying near candidates
            */
            if(targetList.at(mTarget).isMatched && (temp.distance <= 1.1*targetList.at(mTarget).matchingDistance )) // daha önceden eşleşen bir Target ise Birleştirme Kıstası
            {
                // Eğer yakınlık farkı %10 ve daha az ise o Candidate'i de matched diye işaretle
                isCandidateMatched[mCandidate] = true;
                // Adding to a mTarget
                for(unsigned int i = 0 ; i < candidateList->at(mCandidate).contour.size() ; i++ )
                {
                    targetList.at(mTarget).contour.push_back(candidateList->at(mCandidate).contour.at(i));
                }
              //  std::vector<cv::Point> newContour
                std::vector<cv::Point> tempContour;
                cv::approxPolyDP(targetList.at(mTarget).contour,tempContour, 3, true);
                cv::RotatedRect tempRect=cv::minAreaRect(targetList.at(mTarget).contour);
                targetList.at(mTarget).location = tempRect;
                targetList.at(mTarget).contour = tempContour;

                continue;
            }
            /* if matching Target and Canddate is not matched before
             * target.isMatched set to true;
             * isCandidateMatched[mCandidate] set to true;
            */
            // First set vector of Move
            targetList.at(mTarget).setMovingAngle(candidateList->at(mCandidate).rRect);
            // than set new position
            targetList.at(mTarget).location = candidateList->at(mCandidate).rRect;
            targetList.at(mTarget).contour = candidateList->at(mCandidate).contour;
            targetList.at(mTarget).isMatched = true;
            targetList.at(mTarget).statusCounter++;
            targetList.at(mTarget).matchingDistance=temp.distance;
            // Signing Matched  Candidates so dont match again...
            isCandidateMatched[mCandidate] = true;

            // change target status if required
            if( targetList.at(mTarget).status == candidate ) // if candidate
            {
                if(targetList.at(mTarget).statusCounter > settings.visibilityThreshold) // becomes visible
                {
                    targetList.at(mTarget).status = visible;
                }
            }
            else if( targetList.at(mTarget).status == invisible )
            {
                targetList.at(mTarget).status = visible; // becomes visible

            }


        }
    }

}

/* for Debugging Purposes
 * draw center and outer lines of Targets and Id from List
 * to inputImage
 * respect to show settings
 * and show it wName Window
*/
void CandidateFilter::showTargets(cv::Mat &inputImage, char *wName)
{
    // for debuging purposes

    if(inputImage.channels() == 1)
    {
        cv::cvtColor(inputImage,inputImage,CV_GRAY2RGB);
    }

    char *targetStatus;
    for(unsigned int i = 0; i < targetList.size() ; i++ )
    {
        Target temp=targetList[i];

        cv::Point2f vertices[4];
        temp.location.points(vertices);
        int linetype;
        int thickness;
        cv::Scalar color;

        if((temp.status == candidate ) && (settings.showCandidate == false) ) continue;
        if((temp.status == visible ) && (settings.showVisible == false) ) continue;
        if((temp.status == invisible ) && (settings.showInvisible == false) ) continue;


        if(temp.status == candidate )
        {
            color=cv::Scalar( 0,255,255);
            linetype  = 1 ;
            thickness = 1;
            targetStatus = "C";


        }
        else if (temp.status == visible )
        {
            color=cv::Scalar( 0,0,255);
            linetype  = 1 ;
            thickness = 4;
            targetStatus = "V";
        }
        else
        {
           color=cv::Scalar(255,0,0);
            linetype  = 4 ;
            thickness = 1;
            targetStatus = "I";

        }


        for (int i = 0; i < 4; i++)
        {
            cv::line(inputImage, vertices[i], vertices[(i+1)%4],color,thickness,linetype);
        }

        if( settings.showTargetId == true)
        {
            char Buf[512];
            sprintf(Buf,"%s-%d%c",targetStatus,temp.targetId,0);
            cv::putText(inputImage,Buf,temp.location.center,
                        cv::FONT_HERSHEY_COMPLEX_SMALL, 0.6,cv::Scalar(255,255,0), 1);
        }
        if( settings.showVector == true)
        {
           cv::line(inputImage,temp.location.center ,cv::Point(cvRound(temp.location.center.x+temp.deltaX), cvRound(temp.location.center.y+temp.deltaY)),cv::Scalar(255,255,0),2,linetype);
        }

    }
    if(wName != NULL)
      imshow(wName, inputImage );


}

/*
 * Refines  Target List by removing sub targets
 * not working as required
*/
void CandidateFilter::refine()
{
    std::vector<Target>::iterator it;

    for(it = targetList.begin() ; it < targetList.end() ; ++it)
    {

        if(!isNewTarget(*it)) // if temp is a subtarget of any other target
        {
            it=targetList.erase(it); // remove from List
            qDebug()<<"CandidateFilter 354\n";
            if(it != targetList.begin())
                --it;
        }
    }

}

/*
 * Simple distance calculate
*/
float CandidateFilter::calculateDistance(cv::RotatedRect &r1, cv::RotatedRect &r2)
{
    float deltaX=(r1.center.x-r2.center.x);
    float deltaY=(r1.center.y-r2.center.y);
    return sqrtf(deltaX*deltaX+deltaY*deltaY);
}
/*
 * if tempTarget is not a sub of any Target from Target List
 * return true
 * else
 * return false
*/
bool CandidateFilter::isNewTarget(Target &tempTarget)
{
    bool isNew = true;
    for(unsigned int i = 0; i < targetList.size() ; i++ )
    {
        if(targetList[i].isWithin(tempTarget))
        {
            isNew = false;
            break;
        }
    }

    return isNew;

}

/*
 * unmatched candidate if isNewTarget()
 * add to target List as candidate
*/
void CandidateFilter::processUnmatchedCandidates()
{
    for(unsigned int i = 0; i<candidateList->size(); i++)
    {
        if(isCandidateMatched[i] == false )
        {

            Target temp;
            temp.location=candidateList->at(i).rRect;
            temp.contour =candidateList->at(i).contour;
            temp.status=candidate;
            temp.statusCounter=1;
            temp.targetId=++targetIdCounter;
            temp.deltaX = 0 ;
            temp.deltaY = 0 ;
            temp.movingAngle = 0;
            if(isNewTarget(temp)) // if temp is not a subtarget of any other target
            {
                targetList.push_back(temp);
            }
        }

    }

}


/*
 * if isSubTarget is within this Target
 * return true
*/

bool Target::isWithin(Target &isSubTarget)
{
    cv::Point2f vertices[4];
    isSubTarget.location.points(vertices);
    int score=0;
    for(int i = 0 ; i < 4 ; i++ )
    {
        /* Performs a point-in-contour test
         * determines whether the point is inside a contour, outside, or lies on an edge (or coincides with a vertex).
         * It returns positive (inside), negative (outside), or zero (on an edge) value, correspondingly.
         * When measureDist=false , the return value is +1, -1, and 0, respectively.
         * Otherwise, the return value is a signed distance between the point and
         * the nearest contour edge.
       */
        if(cv::pointPolygonTest(this->contour,vertices[i],false) < 0.0 )
        {
             score +=1 ;
        }

    }

    if (score >= 2)
    {
        return false;
    }
    else
    {
        return true;
    }

}

/* vector of Move from this.location.center to r.center
*/
void Target::setMovingAngle(cv::RotatedRect r)
{
    deltaX=location.center.x - r.center.x ;
    deltaY=location.center.y - r.center.y ;
    movingAngle = atan2(deltaY , deltaX);
}
