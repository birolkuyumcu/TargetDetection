#include "frameconsumer.h"
#include <Test/TEST_frameAllignment.h>

FrameConsumer::FrameConsumer(QObject *parent) :
    QThread(parent)
{
            exc.setModuleName("FrameConsumer");
            nPass=5;

}

void FrameConsumer::run()
{
    cv::Mat prevFrame;
    cv::Mat alignedPrevFrame;
    cv::Mat currentDiffImage;



    while(1)
    {

         qDebug()<<frameBuffer->size()<<"Consumer Side\n";
        if(frameBuffer->size() > 0 )
        {
            cv::Mat frame = frameBuffer->front();
            //Do Processing
            pFrame = frame.clone();
            cv::cvtColor(pFrame,pFrame,CV_RGB2GRAY);

            cv::Mat copyCurrentFrame=pFrame.clone();
            cv::Mat H;
            calc.process(copyCurrentFrame);

            if(calc.getHomography(H) == true)
            {
                cv::Mat canImg = frame.clone();
                cv::Mat tarImg = frame.clone();

                qDebug()<<"Homography Found \n";
                // Düzgün dönüşüm matrisi bulunduysa
                cv::Mat mask(prevFrame.size(),CV_8U);
                mask=cv::Scalar(255);

                // Önceki frame aktif frame çevir
                aligner.process(prevFrame,H,alignedPrevFrame);
                // çevrilmiş önceki frame için maske oluştur
                aligner.process(mask,H,mask);
                mask=copyCurrentFrame&mask;
                cv::absdiff(alignedPrevFrame,mask,currentDiffImage);
                processedFrameBuffer->push(currentDiffImage.clone());
                cv::Mat element = cv::getStructuringElement( cv::MORPH_RECT,cv::Size( 3, 3 ),cv::Point( 1, 1 ) );
           //     processedFrameBuffer->push(currentDiffImage.clone());
                //

                cv::threshold(currentDiffImage,currentDiffImage,dynamicThresholdValue(currentDiffImage),255,cv::THRESH_BINARY);
        //        cv::morphologyEx(currentDiffImage,currentDiffImage,cv::MORPH_CLOSE, element,cv::Point(-1,-1),4 );
       //
                cv::dilate(currentDiffImage,currentDiffImage, element,cv::Point(-1,-1),4 );
                cv::erode(currentDiffImage,currentDiffImage, element,cv::Point(-1,-1),4 );

    //            processedFrameBuffer->push(currentDiffImage.clone());
                //
                cDet.process(currentDiffImage);
                cDet.showCandidates(canImg);
                cFilt.process(&cDet.candidateList);
                cFilt.showTargets(tarImg);

                processedFrameBuffer->push(canImg.clone());
                processedFrameBuffer->push(tarImg.clone());
                emit frameProcessed();

            }
            prevFrame=copyCurrentFrame;



            // End Processing

      //      QThread::msleep(1000./25);
            for(int i=0;i<nPass && frameBuffer->size();i++) // to by pass some frame
              frameBuffer->pop();
        }
        else
        {
           QThread::msleep(100);
        }
    }
    // emit a signal to GUI when processing Ends
    emit processingEnd();
}

void FrameConsumer::setBuffers(std::queue<cv::Mat> *iframeBuffer, std::queue<cv::Mat> *iprocessedFrameBuffer)
{
        frameBuffer = iframeBuffer;
        processedFrameBuffer = iprocessedFrameBuffer;
}

void FrameConsumer::setParameters(QVector<QString> &parameterTexts)
{
    calc.setDetectorSimple(parameterTexts[0]);
    calc.setDescriptorSimple(parameterTexts[1]);
    calc.setMatcherSimple(parameterTexts[2]);

    if(parameterTexts[3] == "Feature Based")
    {
        calc.setHomographyMethod(featureBased);
    }
    else // Optical Flow Based
    {
        calc.setHomographyMethod(flowBased);
    }

    if(parameterTexts[4] == "Ransac")
    {
        calc.setHomographyCalcMethod(CV_RANSAC);
    }
    else if(parameterTexts[4] == "Least - Median")
    {
        calc.setHomographyCalcMethod(CV_LMEDS);
    }
    else // Regular
    {
        calc.setHomographyCalcMethod(0);
    }
}
