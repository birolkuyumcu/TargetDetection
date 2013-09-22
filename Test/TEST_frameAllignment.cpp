#include <Test/TEST_frameAllignment.h>


#define TEST_VIDEO_FILE_CNT 14

static long processVideoAndGetScores(QString &videoFileName);
static void reportScoresForVideoFile(int videoFileIndex, long score);


void TEST_frameAllignment()
{
    QString videoFileName;
    long scoreForSingleVideo;
    long totalScoreForVideos = 0;

    //open videos sequentialy.

    for(int i = 0; i <= TEST_VIDEO_FILE_CNT; ++i)
    {
        //determine video fileName
#ifdef WIN32
        videoFileName = "D:/cvs/data/testavi/output";
#else
        videoFileName = "rvid";//"output";
#endif

        videoFileName +=  QString::number(i);
        videoFileName += ".avi";


        //process video and get results
        scoreForSingleVideo = processVideoAndGetScores(videoFileName);

        //save scores to file
        reportScoresForVideoFile(i, scoreForSingleVideo);

        totalScoreForVideos += scoreForSingleVideo;

        scoreForSingleVideo = 0;
    }

    //report total score, variable 0 express total score
    reportScoresForVideoFile(0, totalScoreForVideos);

}


static void reportScoresForVideoFile(int videoFileIndex, long score)
{
    QFile file("testScore.txt");

    if (file.open(QIODevice::Append) )
    {
        QTextStream out(&file);

        if(videoFileIndex == 0)
        {
            //total score for videos
            out<<"Total Score    :"<<score;
        }
        else
        {
                out<<videoFileIndex<<".avi    :"<<score;
        }

        file.close();
    }

}

static long processVideoAndGetScores(QString &videoFileName)
{
    AlignmentMatrixCalc alignMatrixcalc;
    FrameAlignment frameAlligner;

    cv::VideoCapture videoCap;
    cv::Mat videoFrame;
    cv::Mat sequentalImageDiff;
    cv::Mat sequentalImageDiffBinary;

    cv::Mat prevFrameAlligned;
    long sumNonZero = 0;
    cv::Mat homograpyMatrix;

    long frameCount = 0;

   // alignMatrixcalc.setHomographyMethod(flowBased);
    alignMatrixcalc.setDetectorSimple("SURF");
    alignMatrixcalc.setDescriptorSimple("SURF");
    alignMatrixcalc.setHomographyCalcMethod(CV_LMEDS);
    alignMatrixcalc.setMatchingType(knnMatch);

    videoCap.open(videoFileName.toStdString());

    //burası düzeltilecek düzgün init fonksiyonu koyulacak.
    videoCap.read(videoFrame);
    frameCount ++;

    cv::cvtColor(videoFrame, videoFrame, CV_BGR2GRAY);
    cv::resize(videoFrame, videoFrame, cv::Size(640,480));

    //buna neden gerek var. sadece getHomography olsa olmuyor mu?
    alignMatrixcalc.process(videoFrame);

    while (videoCap.read(videoFrame))
    {
        cv::resize(videoFrame, videoFrame, cv::Size(640,480));

        cv::cvtColor(videoFrame, videoFrame, CV_BGR2GRAY);

        alignMatrixcalc.process(videoFrame);

        if(alignMatrixcalc.getHomography(homograpyMatrix) == true)
        {

            cv::Mat mask(videoFrame.size(), CV_8U);
            mask = cv::Scalar(255);

            cv::Mat maskedVideoFrame(videoFrame.size(), CV_8U);
            maskedVideoFrame = cv::Scalar(255);

            frameAlligner.process(videoFrame, homograpyMatrix, prevFrameAlligned);

            frameAlligner.process(mask, homograpyMatrix, mask);

            maskedVideoFrame = videoFrame & mask;

            /*
             *yerine calculateBinaryDiffImageAccording2pixelNeighborhood yazıldı.
            //cv::absdiff(prevFrameAlligned, maskedVideoFrame, sequentalImageDiff);
            //cv::imshow("sequentalImageDiff", sequentalImageDiff);


            cv::threshold(sequentalImageDiff, sequentalImageDiffBinary, _CVS_IS_PIXEL_DIFFERENT_THRES,
                          255, cv::THRESH_BINARY);
                          */

            sequentalImageDiffBinary.create(prevFrameAlligned.size(), prevFrameAlligned.type());
            frameAlligner.calculateBinaryDiffImageAccording2pixelNeighborhood(prevFrameAlligned,
                                                                              maskedVideoFrame,
                                                                              sequentalImageDiffBinary);


            /* Şimdilik bu dursun
            cv::erode(alignedImage, alignedImage, cv::Mat());
            cv::erode(alignedImage, alignedImage, cv::Mat());
            cv::dilate(alignedImage, alignedImage, cv::Mat());
            cv::dilate(alignedImage, alignedImage, cv::Mat());
            */

            cv::imshow("Result", sequentalImageDiffBinary);
            cv::imshow("input", videoFrame);
            cv::waitKey(5);
            sumNonZero += cv::countNonZero(prevFrameAlligned);

        }

        frameCount++;
        qDebug()<<videoFileName<<frameCount;
    }

    return sumNonZero;
}

