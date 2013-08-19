#include <Test/TEST_frameAllignment.h>

#define TEST_VIDEO_FILE_CNT 5

static void processVideoAndGetScores();
static void reportScoresForVideoFile();

/*
void TEST_frameAllignment()
{
    //open videos sequentialy.

    for(int i = 0; i < TEST_VIDEO_FILE_CNT; i++)
    {

        //determine video fileName

        //process video and get results
        processVideoAndGetScores();

        //save scores to file
        reportScoresForVideoFile();

        //reset scores
    }
}


static void reportScoresForVideoFile()
{

}

static void processVideoAndGetScores()
{
    AlignmentMatrixCalc calc;
    FrameAlignment aligner;

    calc.setDetectorSimple("SURF");
    calc.setDescriptorSimple("SURF");

    calc.setHomographyCalcMethod(CV_LMEDS);
    calc.setMatchingType(knnMatch);

    //get Frame from video and process it
    while()
    {
        testPP(frame,pFrame);
        //
        calc.process(pFrame);

        cv::Mat aPrev;
        cv::Mat H;

        if(calc.getHomography(H) == true)
        {
            aligner.process(prev,H,aPrev);
            cv::absdiff(aPrev,pFrame,aPrev);
            cv::threshold(aPrev,aPrev,0,255,cv::THRESH_BINARY|cv::THRESH_OTSU);
            t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
            std::cout<<"Processing Time :"<<t<<"\n\n";
            cv::Mat cFrame;

            //get scores for frame
    }

}
*/
