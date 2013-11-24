#include "candidatefilter.h"

CandidateFilter::CandidateFilter()
{
    settings.distanceThreshold = 100;
    settings.visibilityThreshold = 5;
    settings.invisibilityThreshold = 5;
    targetIdCounter = 0;
}


void CandidateFilter::process(std::vector<cv::RotatedRect> *iCandidateList)
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

}

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
                    if(it != targetList.begin())
                        --it;
                }
            }


        }
    }

}

void CandidateFilter::init()
{
    for(int i = 0; i<candidateList->size(); i++)
    {

        Target temp;
        temp.location = candidateList->at(i);
        temp.status = candidate;
        temp.statusCounter = 1;
        temp.targetId = ++targetIdCounter;
        targetList.push_back(temp);

    }
 }

void CandidateFilter::match()
{

    std::vector<MatchItem> matchTable;

    isCandidateMatched.reserve(candidateList->size());
    isCandidateMatched.assign(candidateList->size(), false);

    for(int j=0; j<targetList.size(); j++)
    {
        targetList.at(j).isMatched = false;

        for(int i = 0; i < candidateList->size(); i++)
        {

            MatchItem temp;
            temp.candidateIndex = i;
            temp.targetIndex = j;
            temp.distance = calculateDistance(candidateList->at(i),targetList.at(j).location);

            if(temp.distance <= settings.distanceThreshold ) // distanceThreshold dan büyük olan uzaklıkları tabloya ekleme
            {
               matchTable.push_back(temp);
            }

        }
    }
    if(matchTable.size() == 0)
    {
        return; //multiple return is forbidded
    }

    std::sort(matchTable.begin(), matchTable.end());

    std::vector<MatchItem>::iterator tableIndex;

    for( tableIndex = matchTable.begin(); tableIndex != matchTable.end(); ++tableIndex )
    {
        // Eşleştir
        MatchItem temp = *tableIndex;
        int mTarget = temp.targetIndex;
        int mCandidate = temp.candidateIndex;

        // process Matched Targets
        if(isCandidateMatched[mCandidate] ||targetList.at(mTarget).isMatched ) // daha önceden eşleşen bir Target Yada Candidate ise  bir sonraki iterasyona git
        {
            continue;
        }

        targetList.at(mTarget).location = candidateList->at(mCandidate);
        targetList.at(mTarget).isMatched = true;
        targetList.at(mTarget).statusCounter++;

        if( targetList.at(mTarget).status == candidate )
        {
            if(targetList.at(mTarget).statusCounter > settings.visibilityThreshold)
            {
                targetList.at(mTarget).status = visible;
            }
        }
        else if( targetList.at(mTarget).status == invisible )
        {
            targetList.at(mTarget).status = visible;
            
        }
        // Signing Matched  Candidates so dont match again...
        isCandidateMatched[mCandidate] = true;

    }

}

void CandidateFilter::showTargets(cv::Mat &inputImage, char *wName)
{
    // for debuging purposes

    if(inputImage.channels() == 1)
    {
        cv::cvtColor(inputImage,inputImage,CV_GRAY2RGB);
    }

    char *targetStatus;
    for( int i = 0; i < targetList.size() ; i++ )
    {
        Target temp=targetList[i];

        cv::Point2f vertices[4];
        temp.location.points(vertices);
        int linetype;
        int thickness;
        cv::Scalar color;
        if(temp.status == candidate )
        {
            color=cv::Scalar( 0,0,255);
            linetype  = 4 ;
            thickness = 2;
            targetStatus = "C";

        }
        else if (temp.status == visible )
        {
            color=cv::Scalar( 0,255,0);
            linetype  = 8 ;
            thickness = 3;
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

        char Buf[512];
        sprintf(Buf,"%s-%d%c",targetStatus,temp.targetId,0);
        cv::putText(inputImage,Buf,temp.location.center,
                    cv::FONT_HERSHEY_COMPLEX_SMALL, 0.6,cv::Scalar(0,255,255), 1);
    //    cv::circle(inputImage,temp.location.center,3,cv::Scalar(0,0,255),-1);
    }
    if(wName==NULL)
        wName="Candidates";
    imshow(wName, inputImage );


}

float CandidateFilter::calculateDistance(cv::RotatedRect &r1, cv::RotatedRect &r2)
{
    float deltaX=(r1.center.x-r2.center.x);
    float deltaY=(r1.center.y-r2.center.y);
    return sqrtf(deltaX*deltaX+deltaY*deltaY);
}


void CandidateFilter::processUnmatchedCandidates()
{
    for(int i = 0; i<candidateList->size(); i++)
    {
        if(isCandidateMatched[i] == false )
        {

            Target temp;
            temp.location=candidateList->at(i);
            temp.status=candidate;
            temp.statusCounter=1;
            temp.targetId=++targetIdCounter;
            targetList.push_back(temp);
        }

    }

}
