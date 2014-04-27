#include "frameconsumer.h"
#include <Test/TEST_frameAllignment.h>

FrameConsumer::FrameConsumer(QObject *parent) :
    QThread(parent)
{
    exc.setModuleName("FrameConsumer");
    nPass = 5;
    isReadingEnd = false;

}

void FrameConsumer::run()
{
    cv::Mat prevFrame;
    cv::Mat alignedPrevFrame;
    cv::Mat currentDiffImage;

    while(1)
    {

        if(frameBuffer->size() > 0 )
        {
            cv::Mat frame_ = frameBuffer->front();

            cv::Mat frame = frame_.clone();

            //Do Processing
            pFrame = frame.clone();
            cv::resize(pFrame, pFrame, cv::Size(320,240));
            cv::cvtColor(pFrame,pFrame,CV_BGR2GRAY);


            cv::Mat copyCurrentFrame = pFrame.clone();
            cv::equalizeHist(copyCurrentFrame,copyCurrentFrame);
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
                processedFrame2UiAbsDiff = currentDiffImage.clone();
                cv::Mat element = cv::getStructuringElement( cv::MORPH_RECT,cv::Size( 3, 3 ),cv::Point( 1, 1 ) );


                cv::threshold(currentDiffImage,currentDiffImage,dynamicThresholdValue(currentDiffImage),255,cv::THRESH_BINARY);
                cv::dilate(currentDiffImage,currentDiffImage, element,cv::Point(-1,-1),4 );
                cv::erode(currentDiffImage,currentDiffImage, element,cv::Point(-1,-1),4 );

                cDet.process(currentDiffImage);
                cDet.showCandidates(canImg);
                cFilt.process(&cDet.candidateList);
                cFilt.showTargets(tarImg,cDet.scaleFactor);

                processedFrame2UiCandidates = canImg.clone();
                processedFrame2UiTargets = tarImg.clone();

            }
            prevFrame = copyCurrentFrame;

            emit frameProcessed();

            frameBuffer->pop();

/* causes a error "Debug Assertion Failed!" and
 * "Expression: deque iterator not dereferencable."
 * mutex not solved

            QMutex mutex;
            mutex.lock();
            for(int i=0; i < nPass ; i++) // to by pass some frame
            {
                qDebug()<<"frame Buffersize : " <<frameBuffer->size()<<"\n";
                if(frameBuffer->size() < 1)
                    break;
                else
                    frameBuffer->pop();
            }
            mutex.unlock();
*/

        }
        else
        {
           QThread::msleep(100);
        }
    }

}

void FrameConsumer::setBuffers(std::queue<cv::Mat> *iframeBuffer)
{
        frameBuffer = iframeBuffer;

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
