#include <Test/TEST_frameAllignment.h>


#define TEST_VIDEO_FILE_CNT 14

static void processVideoAndGetScores(QString &videoFileName, int startFrame, AllignementTestScore& score);
static void reportScoresForVideoFile(AllignementTestScore score);

//
int64 times[10];
bool measureStart[10]={false,false,false,false,false,
                       false,false,false,false,false};

long int timeMeasure(int i)
{
    long timeResult = 0;

    if(measureStart[i] == false)
    {
        times[i] = cv::getTickCount();
        measureStart[i] = true;
    }
    else
    {
        timeResult = ((double)cv::getTickCount() - times[i]) / cv::getTickFrequency();
        qDebug()<<"Time Measurement "<<i<<" : "<<timeResult;

        measureStart[i] = false;
    }

    return timeResult;
}
//


void TEST_frameAllignment()
{
    QString videoFileName;
    AllignementTestScore scoreForSingleVideo;
    int startFrame = 0;

    //open videos sequentialy.

    for(int i = 1; i <= TEST_VIDEO_FILE_CNT; ++i)
    {
        //determine video fileName
#ifdef WIN32
        videoFileName = "D:/cvs/data/testavi/output";
#else
        videoFileName = "output";//"output";
#endif

        videoFileName +=  QString::number(i);
        videoFileName += ".avi";

        scoreForSingleVideo.videoFileName = videoFileName;


        //process video and get results
        processVideoAndGetScores(videoFileName, startFrame, scoreForSingleVideo);

        //save scores to file
        reportScoresForVideoFile(scoreForSingleVideo);

        //TODOtotalScoreForVideos += scoreForSingleVideo;

        //TODOscoreForSingleVideo = 0;
    }

    //report total score, variable 0 express total score
    //TODOreportScoresForVideoFile(0, totalScoreForVideos);

}

static void reportScoresForVideoFile(AllignementTestScore score)
{
    static bool captionWriten = 0;

    QFile file("testScore.txt");

    if (file.open(QIODevice::Append) )
    {
        QTextStream out(&file);

#ifdef _CVS_READABLE_TEST_RESULT
        //total score for videos
        out<<"------------------------------------------------------"<<"\r\n";
        out<<"Video File Name                 :"<<score.videoFileName<<"\r\n";
        out<<"Homography Method               :"<<score.HomographyMethod<<"\r\n";
        out<<"Used Detector                   :"<<score.usedDetector<<"\r\n";
        out<<"Used Descriptor                 :"<<score.usedDescriptor<<"\r\n";
        out<<"Neighbourhood Filter Size       :"<<score.neighbourhoodFilterSize<<"\r\n";
        out<<"White Pixel PerFramePixels      :"<<score.whitePixelPerFramePixels<<"\r\n";
        out<<"homograpyFoundPercent           :"<<score.homograpyFoundPercent<<"\r\n";
        out<<"Processing fps                  :"<<score.fps<<"\r\n";
        out<<"Total Processing TimeSn         :"<<score.TotalTimeSn<<"\r\n";
        out<<"--------------------------------------------------------"<<"\r\n";
#else
        if(captionWriten == 0)
        {
            out.setFieldWidth(0);
            out<<"videoFile";
            out.setFieldWidth(15);
            out<<"HomgrpMet";
            out.setFieldWidth(10);
            out<<"Detec.";
            out.setFieldWidth(20);
            out<<"Descr.";
            out.setFieldWidth(10);
            out<<"nghFilter";
            out.setFieldWidth(20);
            out<<"wPixelPerFPixels";
            out.setFieldWidth(15);
            out<<"homogFndPer";
            out.setFieldWidth(10);
            out<<"fps";
            out.setFieldWidth(10);
            out<<"TtlTime"<<"\r\n";

            captionWriten = 1;
        }

        out.setFieldWidth(0);
        out<<score.videoFileName;
        out.setFieldWidth(15);
        out<<score.HomographyMethod;
        out.setFieldWidth(10);
        out<<score.usedDetector;
        out.setFieldWidth(20);
        out<<score.usedDescriptor;
        out.setFieldWidth(10);
        out<<score.neighbourhoodFilterSize;
        out.setFieldWidth(20);
        out<<score.whitePixelPerFramePixels;
        out.setFieldWidth(15);
        out<<score.homograpyFoundPercent;
        out.setFieldWidth(10);
        out<<score.fps;
        out.setFieldWidth(10);
        out<<score.TotalTimeSn<<"\r\n";

#endif

        file.close();
    }
}

static void processVideoAndGetScores(QString &videoFileName, int startFrame, AllignementTestScore& score)
{
    AlignmentMatrixCalc alignMatrixcalc;
    FrameAlignment frameAlligner;

    cv::VideoCapture videoCap;
    cv::Mat videoFrame;
    cv::Mat currentFrame;
    cv::Mat prevFrame;
    cv::Mat sequentalImageDiff;
    cv::Mat sequentalImageDiffBinary;
    cv::Mat sequentalImageDiffBinary1;

    cv::Mat prevFrameAlligned;
    long sumNonZero = 0;
    cv::Mat homograpyMatrix;

    long frameCount = 0;
    long homographyFoundFrameCount = 0;
    long totalProcessedFrameCount = 0;


    QString         TestDetectorName = "SURF";
    QString         TestDescriptorName = "SURF";
    HomograpyMethod TestHomograpyMethod = featureBased;



    score.usedDetector      = TestDetectorName;
    score.usedDescriptor    = TestDescriptorName;

    score.HomographyMethod  = (TestHomograpyMethod == featureBased)?"featureBased":"flowBased";

    alignMatrixcalc.setHomographyMethod(TestHomograpyMethod);
    alignMatrixcalc.setDetectorSimple(TestDetectorName);
    alignMatrixcalc.setDescriptorSimple(TestDescriptorName);

    alignMatrixcalc.setHomographyCalcMethod(CV_LMEDS);
    alignMatrixcalc.setMatchingType(knnMatch);

    videoCap.open(videoFileName.toStdString());
    qDebug()<<videoFileName;
    //burası düzeltilecek düzgün init fonksiyonu koyulacak.
    if(startFrame != 0)
    {
        videoCap.set(CV_CAP_PROP_POS_FRAMES, (double)startFrame);
        frameCount = startFrame;
    }

    videoCap.read(videoFrame);
    frameCount ++;

    cv::cvtColor(videoFrame, videoFrame, CV_BGR2GRAY);
    cv::resize(videoFrame, videoFrame, cv::Size(640,480));

    //buna neden gerek var. sadece getHomography olsa olmuyor mu?
    alignMatrixcalc.process(videoFrame);
    prevFrame = videoFrame.clone();

    timeMeasure(1);

    while (videoCap.read(videoFrame))
    {
        cv::resize(videoFrame, videoFrame, cv::Size(640,480));
        currentFrame = videoFrame.clone();

        cv::cvtColor(currentFrame, currentFrame, CV_BGR2GRAY);

        alignMatrixcalc.process(currentFrame);

        if(alignMatrixcalc.getHomography(homograpyMatrix) == true)
        {
            cv::Mat mask(currentFrame.size(), CV_8U);
            mask = cv::Scalar(255);

            cv::Mat maskedVideoFrame(currentFrame.size(), CV_8U);
            maskedVideoFrame = cv::Scalar(255);

            frameAlligner.process(prevFrame, homograpyMatrix, prevFrameAlligned);

            frameAlligner.process(mask, homograpyMatrix, mask);

            maskedVideoFrame = currentFrame & mask;

            sequentalImageDiffBinary.create(maskedVideoFrame.size(), CV_8UC1);
            frameAlligner.calculateBinaryDiffImageAccording2pixelNeighborhood(prevFrameAlligned,
                                                                              maskedVideoFrame,
                                                                              sequentalImageDiffBinary);

            cv::imshow("sequentalImageDiffBinaryPixelNeigh", sequentalImageDiffBinary);
            sumNonZero += cv::countNonZero(prevFrameAlligned);


            cv::putText(videoFrame, "Homograpy status: OK", cvPoint(10,35),
                        cv::FONT_HERSHEY_COMPLEX_SMALL, 0.6, cvScalar(200,200,250), 1, CV_AA);

            homographyFoundFrameCount ++;

        }
        else
        {
            cv::putText(videoFrame, "Homograpy status: Not Found!", cvPoint(10,35),
                        cv::FONT_HERSHEY_COMPLEX_SMALL, 0.6, cvScalar(200,200,250), 1, CV_AA);
        }

        frameCount++;
        totalProcessedFrameCount ++;

        QString videoInfo;

        videoInfo = videoFileName + " @ " + QString::number(frameCount);

        cv::putText(videoFrame, videoInfo.toStdString(), cvPoint(10,20),
                    cv::FONT_HERSHEY_COMPLEX_SMALL, 0.6, cvScalar(200,200,250), 1, CV_AA);

        prevFrame = currentFrame.clone();

        cv::imshow("input", videoFrame);
        cv::waitKey(5);
    }

    score.neighbourhoodFilterSize = _CVS_PIXEL_NEIGHBORHOOD_DIST;
    score.TotalTimeSn = timeMeasure(1);
    score.fps = (1.0 * totalProcessedFrameCount) / score.TotalTimeSn;
    score.homograpyFoundPercent = ((homographyFoundFrameCount * 1.0) / totalProcessedFrameCount) * 100;
    score.whitePixelPerFramePixels = (sumNonZero * 1.0) / homographyFoundFrameCount / (640 * 480);

}

