#include "cvs_uavtargetdetectionapp.h"
//
#include "framealigner.h"
#include "alignmentmatrixcalc.h"
#include "framealignment.h"

void Test()
{
    frameAligner fAlgn;
#ifdef WIN32
    cv::Mat img_1 = cv::imread("D:/cvs/data/egt2/frame00790.jpg",CV_LOAD_IMAGE_GRAYSCALE);
    cv::Mat img_2 = cv::imread("D:/cvs/data/egt2/frame00791.jpg", CV_LOAD_IMAGE_GRAYSCALE );
#else
    cv::Mat img_1 = cv::imread("../uavVideoDataset/egtest02/frame00790.jpg",CV_LOAD_IMAGE_GRAYSCALE);
    cv::Mat img_2 = cv::imread("../uavVideoDataset/egtest02/frame00791.jpg", CV_LOAD_IMAGE_GRAYSCALE );
#endif
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
    out=in.clone();
}

void Test2()
{
    AlignmentMatrixCalc calc;
    FrameAlignment aligner;
#ifdef WIN32
    cv::Mat img_1 = cv::imread("D:/cvs/data/egt2/frame00790.jpg",CV_LOAD_IMAGE_GRAYSCALE);
    cv::Mat img_2 = cv::imread("D:/cvs/data/egt2/frame00791.jpg", CV_LOAD_IMAGE_GRAYSCALE );
#else
    cv::Mat img_1 = cv::imread("../uavVideoDataset/egtest02/frame00790.jpg",CV_LOAD_IMAGE_GRAYSCALE);
    cv::Mat img_2 = cv::imread("../uavVideoDataset/egtest02/frame00799.jpg", CV_LOAD_IMAGE_GRAYSCALE );
#endif
    calc.process(img_1);
    calc.process(img_2);

    cv::Mat aPrev;
    cv::Mat H;
    cv::imshow("Img 1",img_1);
    cv::imshow("Img 2",img_2);

    if(calc.getHomography(H) == true)
    {
        aligner.process(img_1,H,aPrev);
        cv::imshow("Aligned Img 1",aPrev);
        cv::absdiff(aPrev,img_2,aPrev);
        cv::imshow("AbsDiff ",aPrev);
    }

    cv::waitKey(0);
}

void FindCandidate(cv::Mat in, cv::Mat frame, cv::Mat &out)
{
    cv::RNG rng;

    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;

    //
    cv::Mat element = cv::getStructuringElement( cv::MORPH_RECT,cv::Size( 3, 3 ),cv::Point( 1, 1 ) );
   // cv::erode( in,in, element,cv::Point(-1,-1),1 );
    cv::dilate( in,in, element,cv::Point(-1,-1),4 );

    cv::findContours( in, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );
    /// Approximate contours to polygons + get bounding rects and circles
    std::vector<std::vector<cv::Point> > contours_poly( contours.size() );
    std::vector<cv::Rect> boundRect( contours.size() );
    std::vector<cv::Point2f>center( contours.size() );
    std::vector<float>radius( contours.size() );
    for(unsigned int i = 0; i < contours.size(); i++ )
    {
        cv::approxPolyDP( cv::Mat(contours[i]), contours_poly[i], 3, true );
        boundRect[i] = cv::boundingRect( cv::Mat(contours_poly[i]) );
        cv::minEnclosingCircle( contours_poly[i], center[i], radius[i] );
    }
    /// Draw polygonal contour + bonding rects + circles

    cv::cvtColor(frame,out,CV_GRAY2BGR);

    for(unsigned int i = 0; i< contours.size(); i++ )
    {
        if(cv::contourArea(contours[i])<100 || cv::contourArea(contours[i])>2500) continue;
    //    cv::Scalar color = cv::Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        cv::Scalar color = cv::Scalar( 0, 0, 255 );
    //    cv::drawContours( out, contours_poly, i,cv::Scalar( 255, 0, 0 ), 2, 8, std::vector<cv::Vec4i>(), 0, cv::Point() );
        cv::rectangle( out, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
  //      cv::circle( out, center[i], (int)radius[i], color, 2, 8, 0 );
    }

}

void Test3()
{
    char Buf[1024];
    char *wName = (char *)"Test";
    cv::Mat frame;
    cv::Mat pFrame;
    cv::namedWindow(wName);
#ifdef WIN32
    frame=cv::imread("D:/cvs/data/egt2/frame00000.jpg",CV_LOAD_IMAGE_GRAYSCALE);
#else
    frame=cv::imread("../uavVideoDataset/egtest02/frame00000.jpg",CV_LOAD_IMAGE_GRAYSCALE);
#endif
    testPP(frame,pFrame);

    cv::imshow(wName,pFrame);
    AlignmentMatrixCalc calc;
    FrameAlignment aligner;

    cv::Mat prev;
    calc.setDetectorSimple("SURF");
    calc.setDescriptorSimple("SURF");
    cv::waitKey(0);
  //  calc.setDetectorSimple("GridORB");

   // calc.setHomographyMethod(flowBased);  // featurebased a göre çok hızlı

    calc.setHomographyCalcMethod(CV_LMEDS);

    calc.setMatchingType(knnMatch);

    calc.process(pFrame);
    prev=pFrame;

    for(int i=0;i<1820;i+=10)
    {
        double t = (double)cv::getTickCount();

#ifdef WIN32
        sprintf(Buf,"D:/cvs/data/egt2/frame%05d.jpg%c",i,0);
#else
        sprintf(Buf,"../uavVideoDataset/egtest02/frame%05d.jpg%c",i,0);
#endif
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
            cv::Mat mask(prev.size(),CV_8U);
            mask=cv::Scalar(255);
            aligner.process(prev,H,aPrev);
            aligner.process(mask,H,mask);
            mask=pFrame&mask;
            cv::absdiff(aPrev,mask,aPrev);
            cv::threshold(aPrev,aPrev,0,255,cv::THRESH_BINARY|cv::THRESH_OTSU);
            t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
            std::cout<<"Processing Time :"<<t<<"\n\n";
            cv::Mat cFrame;
        //    FindCandidate(aPrev,frame,cFrame);
            cv::imshow(wName,aPrev);
            cv::imshow("Out",mask);
            cv::waitKey(1);
            std::cout<<i<<"\n";
        }
        else
        {
            i-=9;
        }



        prev.~Mat();
        prev=pFrame;
    }

}

void Test4()
{
    Preprocess preProcess;
    AlignmentMatrixCalc calc;
    FrameAlignment aligner;
    cv::Mat aPrev;
    cv::Mat H;
    cv::Mat prev;
    cv::Mat pFrame;
    cv::Mat frame;
    char Buf[100];
    calc.setHomographyMethod(featureBased);  // featurebased a göre çok hızlı



    calc.process(pFrame);
    prev=pFrame;

    for(int i=0;i<1820;i+=10)
    {
        double t = (double)cv::getTickCount();

#ifdef WIN32
        sprintf(Buf,"D:/cvs/data/egt1/frame%05d.jpg%c",i,0);
#else
        sprintf(Buf,"../uavVideoDataset/egtest02/frame%05d.jpg%c",i,0);
#endif
        qDebug()<<Buf<<"\n";

        frame = cv::imread(Buf,CV_LOAD_IMAGE_GRAYSCALE);

        if(frame.empty())
        {
            break;
        }

        cv::imshow("beforePreprocess", frame );
        preProcess.process(frame);

        cv::imshow("afterPreprocess", frame );


        calc.process(frame);

        if(calc.getHomography(H) == true)
        {
            aligner.process(prev, H, aPrev);
            cv::absdiff(aPrev, frame, aPrev);
            cv::threshold(aPrev, aPrev, 0,255, cv::THRESH_BINARY|cv::THRESH_OTSU);
            t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
            std::cout<<"Processing Time :"<<t<<"\n\n";
            cv::Mat cFrame;
            FindCandidate(aPrev,frame,cFrame);

            cv::imshow("result", cFrame);
            cv::waitKey(10);
            std::cout<<i<<"\n";
        }
        prev.~Mat();
        prev = frame;
    }

}

void Vivid2Avi(const char *header,int max,const char*aviName)
{


    cv::Mat frame;
    char Buf[1024];

    cv::VideoWriter w;

    sprintf(Buf,"%s%5s.avi%c",header,aviName,0);

    w.open(Buf,-1,23,cv::Size(640,480));

    for(int i = 0 ; i < max+1 ; i++)
    {

        sprintf(Buf,"%sframe%05d.jpg%c",header,i,0);
        std::cout<<Buf<<"\n";
        frame=cv::imread(Buf,CV_LOAD_IMAGE_COLOR);
        w<<frame;
        cv::waitKey(20);
    }

}

void PlayAvi(const char * fAvi)
{
    cv::VideoCapture cap;
    cv::Mat frame;

    cap.open(fAvi);

    while (cap.read(frame))
    {
        cv::imshow("Video",frame);
        cv::waitKey(10);

    }


}

double ApplyTest(cv::Mat &baseFrame, int n, const char *fName , const char *dName, const char * mName)
{
    // baseFrame (1280x720) den bir dizi frame hazırlama (640x480) ( n + 1 adet )
    // by using random walk start from center (320,120)
    std::vector<cv::Mat> frameList;
    int walkX = 320;
    int walkY = 120;

    std::cout<<"1 \n";
    for(int i=0;i < n+1; i++)
    {
        cv::Mat temp(baseFrame, cv::Rect(walkX,walkY,640,480));
        // may be add a some noise
        frameList.push_back(temp);
        cv::imshow("Result", temp);
        cv::waitKey(100);
        walkX += (20-(rand()%40));
        walkY += (10-(rand()%20));
    }

    // diziFrame üzerinde sistemi çalıştırma (n times )

    AlignmentMatrixCalc calc;
    calc.setDetectorSimple(fName);
    calc.setDescriptorSimple(dName);
    calc.setMatcherSimple(mName);
    FrameAlignment aligner;
    double sumNonZero=0.0;


    calc.process(frameList[0]);
    for(int i = 0; i < frameList.size(); i++){
        std::cout<<"2 \n";
        calc.process(frameList[i]);
        cv::Mat aPrev;
        cv::Mat H;

        if(calc.getHomography(H) == true){
            std::cout<<"3 \n";
            cv::Mat mask(frameList[i].size(),CV_8U);
            mask=cv::Scalar(255);
            aligner.process(frameList[i-1],H,aPrev);
            aligner.process(mask,H,mask);
            mask=frameList[i-1]&mask;
            cv::absdiff(aPrev,mask,aPrev);
            cv::threshold(aPrev,aPrev,0,255,cv::THRESH_BINARY|cv::THRESH_OTSU);
            cv::imshow("Result", aPrev);
            cv::waitKey(100);
            // count non-zero pixels
            sumNonZero += cv::countNonZero(aPrev);
        }
        else
        {
            std::cout<<"4 \n";
        }


    }

    std::cout<<"5 \n";
    // return avarge of them (cout / n ) low is better...
    sumNonZero = sumNonZero / n;
    return sumNonZero;
}

void ArtificalPeformanceTester()
{
    /*
    FeatureDetectors;
    • "FAST" – FastFeatureDetector
    • "STAR" – StarFeatureDetector
    • "SIFT" – SIFT (nonfree module)
    • "SURF" – SURF (nonfree module)
    • "ORB" – ORB
    • "MSER" – MSER
    • "GFTT" – GoodFeaturesToTrackDetector
    • "HARRIS" – GoodFeaturesToTrackDetector with Harris detector enabled
    • "Dense" – DenseFeatureDetector
    • "SimpleBlob" – SimpleBlobDetector

    Also a combined format is supported: feature detector adapter name ( "Grid" – GridAdaptedFeatureDetector,
    "Pyramid" – PyramidAdaptedFeatureDetector ) + feature detector name (see above), for example: "GridFAST",
    "PyramidSTAR" .

    DescriptorExtractors;
    • "SIFT" – SIFT
    • "SURF" – SURF
    • "ORB" – ORB
    • "BRIEF" – BriefDescriptorExtractor
    • "FREAK" -
    A combined format is also supported: descriptor extractor adapter name ( "Opponent" –
    OpponentColorDescriptorExtractor ) + descriptor extractor name (see above), for example: "OpponentSIFT" .

    DescriptorMatchers;

    – BruteForce (it uses L2 )
    – BruteForce-L1
    – BruteForce-Hamming
    – BruteForce-Hamming(2)
    – FlannBased

    */

    const char * featureDetectorNames[10] = {"FAST",
                                           "STAR",
                                           "SIFT",
                                           "SURF",
                                           "ORB",
                                           "MSER",
                                           "GFTT",
                                           "HARRIS",
                                           "Dense",
                                           "SimpleBlob"
                                           };
    const char *descriptorExtractorNames[6] = {"SIFT",
                                               "SURF",
                                               "ORB",
                                               "BRIEF",
                                               "BRISK",
                                               "FREAK"
                                               };
    const char *matcherNames[5]={"BruteForce",
                                "BruteForce-L1",
                                "BruteForce-Hamming",
                                "BruteForce-Hamming(2)",
                                "FlannBased"
                                };

    float perf=0;

#ifdef WIN32
    cv::Mat baseFrame = cv::imread("D:/cvs/data/bframes/frame00.png",CV_LOAD_IMAGE_GRAYSCALE);
#else
    cv::Mat baseFrame = cv::imread("../uavVideoDataset/egtest02/frame00.png",CV_LOAD_IMAGE_GRAYSCALE);
#endif
    // Base Frame 1280x720

    int nTimes=10;

    std::cout<<"Test Started \n";

    for( int ftr = 0 ; ftr < 10; ftr++){
        for( int dsc = 0 ; dsc < 6 ; dsc++){
            for ( int mtc = 0; mtc < 5 ; mtc++ ){
                perf=ApplyTest(baseFrame,nTimes,featureDetectorNames[ftr],descriptorExtractorNames[dsc],matcherNames[mtc]);
                std::cout<<featureDetectorNames[ftr]<<" "<<descriptorExtractorNames[dsc]<<" "<<matcherNames[mtc]<<" : "<<perf<<"\n";
            }
        }
    }



}

int main(int argc, char *argv[])
{
    //CVS_UAVTargetDetectionApp targetDetection(argc, argv);
    //targetDetection.exec();

    // Test3();
     ArtificalPeformanceTester();

    return 0;
}

