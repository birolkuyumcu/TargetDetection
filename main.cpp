#include "cvs_uavtargetdetectionapp.h"
//
#include "framealigner.h"
#include "alignmentmatrixcalc.h"
#include "framealignment.h"

void Test()
{
    frameAligner fAlgn;
    cv::Mat img_1 = cv::imread("D:/cvs/data/egt2/frame00790.jpg",CV_LOAD_IMAGE_GRAYSCALE);
    cv::Mat img_2 = cv::imread("D:/cvs/data/egt2/frame00791.jpg", CV_LOAD_IMAGE_GRAYSCALE );
    fAlgn.add(img_1);
    fAlgn.add(img_2);
    cv::Mat aPrev,aPrevMask;
    fAlgn.alignPrevFrame(aPrev);
    cv::absdiff(aPrev,img_2,aPrev);
    cv::imshow("Sonuç",aPrev);

//     cv::imshow("Sonuc",img_1);
     cv::waitKey(0);
}

void testPP(cv::Mat &in, cv::Mat &out)
{
//    cv::resize(in,out, cv::Size(),0.5,0.5);
    out=in;
}

void Test2()
{
    AlignmentMatrixCalc calc;
    FrameAlignment aligner;

    cv::Mat img_1 = cv::imread("D:/cvs/data/egt2/frame00790.jpg",CV_LOAD_IMAGE_GRAYSCALE);
    cv::Mat img_2 = cv::imread("D:/cvs/data/egt2/frame00799.jpg", CV_LOAD_IMAGE_GRAYSCALE );
    calc.process(img_1);
    calc.process(img_2);

    cv::Mat aPrev;
    cv::Mat H;
    cv::imshow("Img 1",img_1);
    cv::imshow("Img 2",img_2);

    if(calc.getHomography(H) == true){
        aligner.process(img_1,H,aPrev);
        cv::imshow("Aligned Img 1",aPrev);
        cv::absdiff(aPrev,img_2,aPrev);
        cv::imshow("AbsDiff ",aPrev);
    }


     cv::waitKey(0);
}

void Test3()
{
    char Buf[1024];
    char *wName="Test";
    cv::Mat frame;
    cv::Mat pFrame;
    cv::namedWindow(wName);
    frame=cv::imread("D:/cvs/data/egt2/frame00000.jpg",CV_LOAD_IMAGE_GRAYSCALE);

    testPP(frame,pFrame);

    cv::imshow(wName,pFrame);
    AlignmentMatrixCalc calc;
    FrameAlignment aligner;
    cv::Mat prev;
//    calc.setDetectorSimple("SURF");
//    calc.setDescriptorSimple("SURF");
//    calc.setHomographyMethod(flowBased);  // featurebased a göre çok hızlı
    calc.setHomographyCalcMethod(CV_LMEDS);
    calc.process(pFrame);
    prev=pFrame;

    for(int i=0;i<1300;i+=1)
    {
        double t = (double)cv::getTickCount();


        sprintf(Buf,"D:/cvs/data/egt2/frame%05d.jpg%c",i,0);
        std::cout<<Buf<<"\n";
        frame=cv::imread(Buf,CV_LOAD_IMAGE_GRAYSCALE);
        if(frame.empty()) break;
        //
        testPP(frame,pFrame);
        //
        calc.process(pFrame);

        cv::Mat aPrev;
        cv::Mat H;

        if(calc.getHomography(H) == true){
            aligner.process(prev,H,aPrev);
            cv::absdiff(aPrev,pFrame,aPrev);
            cv::threshold(aPrev,aPrev,0,255,cv::THRESH_BINARY|cv::THRESH_OTSU);
            t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
            std::cout<<"Processing Time :"<<t<<"\n\n";
            cv::imshow(wName,aPrev);
            cv::waitKey(10);
            std::cout<<i<<"\n";
        }
        prev=pFrame;




    }

}

int main(int argc, char *argv[])
{
 /*   CVS_UAVTargetDetectionApp targetDetection(argc, argv);

    targetDetection.exec();
*/
    Test3();

    return 0;
}

