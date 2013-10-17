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
    std::vector<std::vector<float>> distanceMatrix;
    distanceMatrix.reserve(candidateList->size());
    for(int i=0;i<candidateList->size();i++)
    {
        for(int j=0;j<targetList.size();j++)
        {
            distanceMatrix[i].push_back(calculateDistance(candidateList->at(i),targetList.at(j).location));
        }
    }

}

float CandidateFilter::calculateDistance(cv::RotatedRect &r1, cv::RotatedRect &r2)
{
    return 0;
}
