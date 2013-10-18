#include "candidatefilter.h"

CandidateFilter::CandidateFilter()
{
}


void CandidateFilter::process(std::vector<cv::RotatedRect> *iCandidateList)
{
    candidateList=iCandidateList;
    if(targetList.size()==0)
    {
        init();
        return;
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

    std::list<MatchItem> matchTable;

    for(int j=0;j<targetList.size();j++)
    {
        targetList.at(j).isMatched=false;
        for(int i=0;i<candidateList->size();i++)
        {

            MatchItem temp;
            temp.candidateIndex=i;
            temp.targetIndex=j;
            temp.distance=calculateDistance(candidateList->at(i),targetList.at(j).location);
            matchTable.push_back(temp);

        }
    }

    matchTable.sort();

    std::list<MatchItem>::iterator tableIndex;

    for( tableIndex = matchTable.begin(); tableIndex != matchTable.end(); tableIndex++ )
    {
      //cout << *theIterator;
        // Eşleştir
        MatchItem temp=*tableIndex;
//        if(temp.distance > distanceThreshod)  break;
        int mTarget=temp.targetIndex;
        int mCandidate=temp.candidateIndex;
        targetList.at(temp.targetIndex).location=candidateList->at(temp.candidateIndex);
        targetList.at(temp.targetIndex).isMatched=true;
        targetList.at(temp.targetIndex).statusCounter++;
        if(targetList.at(temp.targetIndex).statusCounter > 3)
            targetList.at(temp.targetIndex).status=visible;
        // Eşleşen indexleri listeden kaldır.
        std::list<MatchItem>::iterator removeIndex;
        for( removeIndex = matchTable.begin(); removeIndex != matchTable.end(); removeIndex++ )
        {
            MatchItem removeTemp=*removeIndex;
            if(removeTemp.candidateIndex == mCandidate || removeTemp.targetIndex == mTarget)
                matchTable.erase(removeIndex);

        }

    }




}

float CandidateFilter::calculateDistance(cv::RotatedRect &r1, cv::RotatedRect &r2)
{
    float deltaX=(r1.center.x-r2.center.x);
    float deltaY=(r1.center.y-r2.center.y);
    return sqrtf(deltaX*deltaX+deltaY*deltaY);
}
