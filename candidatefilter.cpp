#include "candidatefilter.h"

CandidateFilter::CandidateFilter()
{
    settings.distanceThreshold = 100;
    settings.visibilityThreshold = 3;
    settings.invisibilityThreshold = 9;
    targetIdCounter=0;
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

    for(it = targetList.begin() ; it < targetList.end() ; ++it)
    {

        if(it->isMatched == false ) // Unmatched Targets
        {
           // qDebug()<<"Unmatched Targets \n";
            if(  it->status == visible)
            {
                qDebug()<<"Unmatched Targets  Visible\n";
                it->status = invisible;
                it->statusCounter=1;

            }
            else if( it->status == invisible)
            {
                qDebug()<<"Unmatched Targets  Invisible\n";
                it->statusCounter++;
                if(it->statusCounter > settings.invisibilityThreshold )
                {
                    it=targetList.erase(it);
                    if(it != targetList.begin())
                        --it;
                }

            }
            else if (it->status == candidate)
            {
                qDebug()<<"Unmatched Targets  Candidate\n";
                it=targetList.erase(it);
                if(it != targetList.begin())
                    --it;
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
        temp.targetId=++targetIdCounter;
        targetList.push_back(temp);

    }
    qDebug()<<"Candidate Filter Init\n\n";
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
        qDebug()<<"Candidate :"<<mCandidate<<" matched to Target : "<<mTarget<<"\n";
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
            targetStatus="C";

        }
        else if (temp.status == visible )
        {
            color=cv::Scalar( 0,255,0);
            linetype  = 8 ;
            thickness = 3;
            targetStatus="V";
        }
        else
        {
           color=cv::Scalar(255,0,0);
            linetype  = 4 ;
            thickness = 1;
            targetStatus="I";
        }


        for (int i = 0; i < 4; i++)
            cv::line(inputImage, vertices[i], vertices[(i+1)%4],color,thickness,linetype);
        char Buf[512];
        sprintf(Buf,"%s-%d%c",targetStatus,temp.targetId,0);
        cv::putText(inputImage,Buf,temp.location.center,
                    cv::FONT_HERSHEY_COMPLEX_SMALL, 0.6, color, 1);
    //    cv::circle(inputImage,temp.location.center,3,cv::Scalar(0,0,255),-1);
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
            temp.targetId=++targetIdCounter;
            targetList.push_back(temp);
        }

    }

}
