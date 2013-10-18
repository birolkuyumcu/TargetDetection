#include "candidatefilter.h"

CandidateFilter::CandidateFilter()
{
    settings.distanceThreshold = 50;
    settings.visibilityThreshold = 3;
    settings.invisibilityThreshold = 5;
}


void CandidateFilter::process(std::vector<cv::RotatedRect> *iCandidateList)
{
    candidateList=iCandidateList;
    if(targetList.size()==0)
    {
        init();
        return;
    }
    if(candidateList->size()> 0)
    {
        match();
    }
    processUnmatchedTargets();
    processUnmatchedCandidates();
}

void CandidateFilter::processUnmatchedTargets()
{
    std::vector<Target>::iterator it;

    for(it = targetList.begin() ; it < targetList.end() ; it++)
    {
        Target tmp=*it;

        if( tmp.isMatched == false ) // Unmatched Targets
        {
            if(  tmp.status == visible ||   tmp.status == candidate)
            {
                tmp.status = invisible;
                tmp.statusCounter=1;

            }
            else if(  tmp.status == invisible)
            {
                tmp.statusCounter++;
                if( tmp.statusCounter > settings.invisibilityThreshold )
                    targetList.erase(it);
            }

        }

    }
    
}

void CandidateFilter::init()
{
    for(int i=0;i<candidateList->size();i++)
    {
        Target temp;
        temp.location=candidateList->at(i);
        temp.status=candidate;
        temp.statusCounter=1;
        targetList.push_back(temp);

    }
}

void CandidateFilter::match()
{

    std::vector<MatchItem> matchTable;

    isCandidateMatched.reserve(candidateList->size());
    isCandidateMatched.assign(candidateList->size(),false);

    for(int j=0;j<targetList.size();j++)
    {
        targetList.at(j).isMatched=false;
        for(int i=0;i<candidateList->size();i++)
        {

            MatchItem temp;
            temp.candidateIndex=i;
            temp.targetIndex=j;
            temp.distance=calculateDistance(candidateList->at(i),targetList.at(j).location);
            if(temp.distance <= settings.distanceThreshold ) // distanceThreshold dan büyük olan uzaklıkları tabloya ekleme
               matchTable.push_back(temp);

        }
    }
    if(matchTable.size() == 0)
        return;

    std::sort(matchTable.begin(),matchTable.end());

    std::vector<MatchItem>::iterator tableIndex;

    for( tableIndex = matchTable.begin(); tableIndex != matchTable.end(); ++tableIndex )
    {
      //cout << *theIterator;
        // Eşleştir
        MatchItem temp=*tableIndex;
//        if(temp.distance > distanceThreshod)  break;
        int mTarget=temp.targetIndex;
        int mCandidate=temp.candidateIndex;
        // process Matched Targets
        if(isCandidateMatched[mCandidate] ||targetList.at(mTarget).isMatched ) // daha önceden eşleşen bir Target Yada Candidate ise  bir sonraki iterasyona git
            continue;
        targetList.at(temp.targetIndex).location=candidateList->at(temp.candidateIndex);
        targetList.at(temp.targetIndex).isMatched=true;
        targetList.at(temp.targetIndex).statusCounter++;
        if( targetList.at(temp.targetIndex).status == candidate )
        {
            if(targetList.at(temp.targetIndex).statusCounter > settings.visibilityThreshold)
                targetList.at(temp.targetIndex).status=visible;
        }
        else if( targetList.at(temp.targetIndex).status == invisible )
        {
            targetList.at(temp.targetIndex).status=visible;
            
        }
        // Eliminate Matched  Candidates
        isCandidateMatched[mCandidate]=true;

    }




}

void CandidateFilter::showTargets(cv::Mat &inputImage)
{
    // for debuging

    if(inputImage.channels() == 1)
    {
        cv::cvtColor(inputImage,inputImage,CV_GRAY2RGB);
    }
    for( int i = 0; i < targetList.size() ; i++ )
    {
        Target temp=targetList[i];

        cv::Point2f vertices[4];
        temp.location.points(vertices);
        int linetype;
        int thickness;
        if(temp.status == candidate )
        {
            linetype  = 4 ;
            thickness = 2;

        }
        else if (temp.status == visible )
        {
            linetype  = 8 ;
            thickness = 3;
        }
        else
        {
            linetype  = CV_AA ;
            thickness = 1;
        }


        for (int i = 0; i < 4; i++)
            cv::line(inputImage, vertices[i], vertices[(i+1)%4], cv::Scalar(0,255,0),thickness,linetype);
        cv::circle(inputImage,temp.location.center,3,cv::Scalar(0,0,255),-1);
    }
    imshow("Targets", inputImage );

}

float CandidateFilter::calculateDistance(cv::RotatedRect &r1, cv::RotatedRect &r2)
{
    float deltaX=(r1.center.x-r2.center.x);
    float deltaY=(r1.center.y-r2.center.y);
    return sqrtf(deltaX*deltaX+deltaY*deltaY);
}


void CandidateFilter::processUnmatchedCandidates()
{
    for(int i=0;i<candidateList->size();i++)
    {
        if(isCandidateMatched[i] == false )
        {

            Target temp;
            temp.location=candidateList->at(i);
            temp.status=candidate;
            temp.statusCounter=1;
            targetList.push_back(temp);
        }

    }

}
