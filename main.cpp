#include "cvs_uavtargetdetectionapp.h"
//
#include "framealigner.h"
#include "alignmentmatrixcalc.h"
#include "framealignment.h"
#include "Test/TEST_frameAllignment.h"
#include <time.h>

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
    CandidateDetector cDet;

    cv::Mat prev;
    calc.setDetectorSimple("SIFT");
    calc.setDescriptorSimple("ORB");
   // cv::waitKey(0);
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
        qDebug()<<Buf<<"\n";
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
            qDebug()<<"Processing Time :"<<t<<"\n\n";
            cv::Mat cFrame;
        //    FindCandidate(aPrev,frame,cFrame);
            cDet.process(aPrev);
            cv::imshow(wName,aPrev);
            cv::imshow("Out",mask);
            cv::waitKey(1);
            qDebug()<<i<<"\n";
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
            qDebug()<<"Processing Time :"<<t<<"\n\n";
            cv::Mat cFrame;
            FindCandidate(aPrev,frame,cFrame);

            cv::imshow("result", cFrame);
            cv::waitKey(10);
            qDebug()<<i<<"\n";
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
        qDebug()<<Buf<<"\n";
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
        cv::waitKey(0);

    }


}

void produceArtificialDataset(cv::Mat &baseFrame,int n,std::vector<cv::Mat> &frameList)
{
    // baseFrame (1280x720) den bir dizi frame hazırlama (640x480) ( n + 1 adet )
    // by using random walk start from center (320,120)
    int walkX = 0;
    int walkY = 0;
    srand (time(NULL));
    qDebug()<<"1 \n";
    for(int i=0;i < n+1; i++)
    {
        cv::Mat temp(baseFrame, cv::Rect(walkX,walkY,640,480));
        // may be add a some noise
        frameList.push_back(temp);
        cv::imshow("Result", temp);
        cv::waitKey(100);
        walkX += ((rand()%5));
        walkY += ((rand()%3));

     //   walkX += (20-(rand()%40));
     //   walkY += (10-(rand()%20));
    }
  //  qDebug()<<"1 Ends \n";

}

double ApplyTest(std::vector<cv::Mat> frameList,const char *fName , const char *dName, const char * mName)
{

    // diziFrame üzerinde sistemi çalıştırma (n times )

    AlignmentMatrixCalc calc;
    FrameAlignment aligner;
    CandidateDetector cDet;

    cv::Mat alignedImage;
    cv::Mat homographMatrix;
    double sumNonZero = 0.0;
    double activeframes=0;

    calc.setDetectorSimple(fName);
    calc.setDescriptorSimple(dName);
    calc.setMatcherSimple(mName);

    calc.process(frameList[0]);

    for(unsigned int i = 1; i < frameList.size(); i++)
    {
    //    qDebug()<<" 2 ";

        calc.process(frameList[i]);

        if(calc.getHomography(homographMatrix) == true)
        {
   //         qDebug()<<" 3 ";
            activeframes += 1;

            cv::Mat mask(frameList[i].size(), CV_8U);

            mask = cv::Scalar(255);

            aligner.process(frameList[i-1], homographMatrix, alignedImage);
            aligner.process(mask, homographMatrix, mask);

            mask = frameList[i-1]&mask;

            cv::absdiff(alignedImage, mask, alignedImage);
            cv::threshold(alignedImage, alignedImage, 0, 255, cv::THRESH_BINARY|cv::THRESH_OTSU);
            cv::imshow("Result", alignedImage);

        //    cDet.process(alignedImage);

            // count non-zero pixels
            sumNonZero += cv::countNonZero(alignedImage);

            cv::waitKey(1);

        }
        else
        {
            qDebug()<<"4 improper matching... \n"; // improper matching...
        }


    }

 //   qDebug()<<" 5 \n";
    // return avarge of them (cout / n ) low is better...
    if( activeframes > 0)
      sumNonZero = sumNonZero / activeframes;
    else
      sumNonZero = -1;
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

    const char * featureDetectorNames[10] = {"SURF",
                                             "FAST",
                                             "STAR",
                                             "SIFT",
                                             "ORB",
                                             "MSER",
                                             "GFTT",
                                             "HARRIS",
                                             "Dense",
                                             "SimpleBlob"
                                           };
    const char *descriptorExtractorNames[6] = {"SURF",
                                               "SIFT",
                                               "ORB",
                                               "BRIEF",
                                               "BRISK",
                                               "FREAK"
                                               };

    //D:\OpenCv-2.4.6\opencv\modules\features2d\src\matchers.cpp - line 462
    const char *matcherNames[6]={"BruteForce",
                                "BruteForce-SL2",
                                "BruteForce-L1",
                                "BruteForce-Hamming",
                                "BruteForce-Hamming(2)",
                                "FlannBased"
                                };

    float perf=0;

#ifdef WIN32
    cv::Mat baseFrame = cv::imread("D:/cvs/data/bframes/frame01.png",CV_LOAD_IMAGE_GRAYSCALE);
#else
    cv::Mat baseFrame = cv::imread("../uavVideoDataset/egtest02/frame00.png",CV_LOAD_IMAGE_GRAYSCALE);
#endif
    // Base Frame 1280x720

    int nTimes=10;

    qDebug()<<"Test Started \n";
    std::vector<cv::Mat> frameList;

    produceArtificialDataset(baseFrame,nTimes,frameList);





    for ( int mtc = 0; mtc < 6 ; mtc++ ){
        for( int dsc = 0 ; dsc < 6 ; dsc++){
            for( int ftr = 0 ; ftr < 8; ftr++){
                //
                if(ftr == 3 && dsc == 2 )
                    continue;
                //
                QFile file("d:/ArtificalPeformanceTest.txt");
                file.open(QIODevice::Append);
                QTextStream logOut(&file);
                qDebug()<< "Test for "<<featureDetectorNames[ftr]<<" "<<descriptorExtractorNames[dsc]<<" "<<matcherNames[mtc]<<"\n";
                perf=ApplyTest(frameList,featureDetectorNames[ftr],descriptorExtractorNames[dsc],matcherNames[mtc]);
                qDebug()<<featureDetectorNames[ftr]<<" "<<descriptorExtractorNames[dsc]<<" "<<matcherNames[mtc]<<" : "<<perf<<"\n";
                logOut<<featureDetectorNames[ftr]<<" "<<descriptorExtractorNames[dsc]<<" "<<matcherNames[mtc]<<" "<<featureDetectorNames[ftr]<<"-"<<descriptorExtractorNames[dsc]<<"-"<<matcherNames[mtc]<<"  "<<perf<<"\r\n";
                logOut.atEnd();
                file.close();
            }
        }
    }



}

void newPreProc(cv::Mat in, cv::Mat& out)
{
    cv::equalizeHist(in,out);
    cv::Mat element = cv::getStructuringElement( cv::MORPH_RECT,cv::Size( 3, 3 ),cv::Point( 1, 1 ) );
    cv::morphologyEx(out,out,CV_MOP_GRADIENT,element);
    cv::threshold(out,out,0,255,cv::THRESH_BINARY|cv::THRESH_OTSU);
}

// Perform in-place unsharp masking operation
void unsharpMask(cv::Mat& im)
{
    cv::Mat tmp;
    cv::GaussianBlur(im, tmp, cv::Size(5,5), 5);
    cv::addWeighted(im, 1.5, tmp, -0.5, 0, im);
}

void Test5()


{
    char Buf[1024];
    char *wName = (char *)"Test";
    cv::Mat frame;
    cv::Mat pFrame;
    cv::namedWindow(wName);
#ifdef WIN32
    frame=cv::imread("D:/cvs/data/egt1/frame00000.jpg",CV_LOAD_IMAGE_GRAYSCALE);
#else
    frame=cv::imread("../uavVideoDataset/egtest02/frame00000.jpg",CV_LOAD_IMAGE_GRAYSCALE);
#endif
  //  testPP(frame,pFrame);

    cv::imshow(wName,frame);
    AlignmentMatrixCalc calc;
    FrameAlignment aligner;
    CandidateDetector cDet;
    CandidateFilter cFilt;

    Preprocess preProcess;
    PreprocessSettings p;

    p.method=DoG;

    preProcess.set(p);
    //calc.setDetectorSimple("HARRIS");
   // calc.setHomographyMethod(featureBased);

    cv::Mat prev;
    pFrame=frame.clone();
    //preProcess.process(pFrame);
   // unsharpMask(pFrame);  // pek bir faydası olmadı
    calc.process(pFrame);

    prev=pFrame;

    for(int i=1;i<1820;i+=3)
    {
        double t = (double)cv::getTickCount();

#ifdef WIN32
        sprintf(Buf,"D:/cvs/data/egt1/frame%05d.jpg%c",i,0);
#else
        sprintf(Buf,"../uavVideoDataset/egtest02/frame%05d.jpg%c",i,0);
#endif
        qDebug()<<Buf<<"\n";
        frame=cv::imread(Buf,CV_LOAD_IMAGE_GRAYSCALE);
        if(frame.empty())
            break;
         pFrame=frame.clone();
        // newPreProc(pFrame,pFrame);
       // preProcess.process(pFrame);
       // unsharpMask(pFrame);
        calc.process(pFrame);

        cv::Mat aPrev;
        cv::Mat H;
        cv::Mat element = cv::getStructuringElement( cv::MORPH_RECT,cv::Size( 3, 3 ),cv::Point( 1, 1 ) );
        if(calc.getHomography(H) == true){
            cv::Mat mask(prev.size(),CV_8U);
            mask=cv::Scalar(255);
            aligner.process(prev,H,aPrev);
            aligner.process(mask,H,mask);
            mask=pFrame&mask;
            cv::absdiff(aPrev,mask,aPrev);
            cv::threshold(aPrev,aPrev,0,255,cv::THRESH_BINARY|cv::THRESH_OTSU);
            t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
            qDebug()<<"Processing Time :"<<t<<"\n\n";
            cv::Mat cFrame;

            cv::dilate(aPrev,aPrev, element,cv::Point(-1,-1),4 );
            cv::erode(aPrev,aPrev, element,cv::Point(-1,-1),4 );
            cDet.process(aPrev);
            cFilt.process(&cDet.candidateList);
          //  cDet.showCandidates(frame);
            cFilt.showTargets(frame);
            cv::imshow(wName,aPrev);
            cv::imshow("Out",mask);
            cv::waitKey(1);
            qDebug()<<i<<"\n";
        }
        else
        {
            // i-=2;
        }



       // prev.~Mat();
        prev=pFrame;
    }

}

/*
 ** Test for MHI
 **
 **/

void Test6()


{
    char Buf[1024];
    char *wName = (char *)"Test";
    cv::Mat currentFrame;
    cv::Mat copyCurrentFrame; // used for orginal current frame not changed
    cv::Mat prevFrame;
    cv::Mat alignedPrevFrame;
    cv::Mat currentDiffImage;
    cv::Mat mhiImage;
    std::vector<cv::Mat>diffImageList;
    int nHistory=5;
    float weights[5]={0.6,0.77,0.87,0.95,1};


    cv::namedWindow(wName);
#ifdef WIN32
 //   currentFrame=cv::imread("D:/cvs/data/egt2/frame00000.jpg",CV_LOAD_IMAGE_GRAYSCALE);
    currentFrame=cv::imread("D:/cvs/data/pktest1/frame00000.jpg",CV_LOAD_IMAGE_GRAYSCALE);
#else
    frame=cv::imread("../uavVideoDataset/egtest02/frame00000.jpg",CV_LOAD_IMAGE_GRAYSCALE);
#endif

    cv::imshow(wName,currentFrame);
    AlignmentMatrixCalc calc;
    FrameAlignment aligner;
    CandidateDetector cDet;
    CandidateDetector cDetMhi;
    CandidateFilter cFilt;
    CandidateFilter cFiltMhi;

    // SURF kadar iyisi yok
  //  calc.setDetectorSimple("ORB");
   // calc.setDescriptorSimple("FREAK");
    calc.setHomographyMethod(flowBased);
  //  calc.setDetectorSimple("GridFAST");

    // Init section
    copyCurrentFrame=currentFrame.clone();
    calc.process(copyCurrentFrame);

    prevFrame=copyCurrentFrame;

    for(int i=1;i<1820;i+=3)
    {
        double t = (double)cv::getTickCount();

#ifdef WIN32
        //sprintf(Buf,"D:/cvs/data/egt2/frame%05d.jpg%c",i,0);
        sprintf(Buf,"D:/cvs/data/pktest1/frame%05d.jpg%c",i,0);
#else
        sprintf(Buf,"../uavVideoDataset/egtest02/frame%05d.jpg%c",i,0);
#endif
        qDebug()<<Buf<<"\n";
        currentFrame=cv::imread(Buf,CV_LOAD_IMAGE_GRAYSCALE);
        if(currentFrame.empty())
            break;
         copyCurrentFrame=currentFrame.clone();

        calc.process(copyCurrentFrame);


        cv::Mat H;
        cv::Mat element = cv::getStructuringElement( cv::MORPH_RECT,cv::Size( 3, 3 ),cv::Point( 1, 1 ) );
        if(calc.getHomography(H) == true){
            // Düzgün dönüşüm matrisi bulunduysa
            cv::Mat mask(prevFrame.size(),CV_8U);
            mask=cv::Scalar(255);


            // Önceki frame aktif frame çevir
            aligner.process(prevFrame,H,alignedPrevFrame);
            // çevrilmiş önceki frame için maske oluştur
            aligner.process(mask,H,mask);
            mask=copyCurrentFrame&mask;
            cv::imshow("Current Frame",currentFrame);

      //      aligner.calculateBinaryDiffImageAccording2pixelNeighborhood(alignedPrevFrame,mask,currentDiffImage);

            cv::absdiff(alignedPrevFrame,mask,currentDiffImage);
            diffImageList.push_back(currentDiffImage);

            if(diffImageList.size()> nHistory)
            {
                diffImageList.erase(diffImageList.begin()); // FIFO
            }
            // homography'ye göre eski diffImageleri çevir
            for(int i=0;i<diffImageList.size()-1;i++) // no need for last inserted
            {
                aligner.process(diffImageList[i],H,diffImageList[i]);
            }

            if(diffImageList.size()==nHistory)
            {
         //       cv::BackgroundSubtractorMOG2 bg_model;
                mhiImage=currentDiffImage.clone();
                mhiImage=cv::Scalar(0);
                for(int i=0;i<diffImageList.size();i++) // no need for last inserted
                {
           //         bg_model(diffImageList[i],mhiImage);
                    mhiImage+=diffImageList[i]*weights[i];
                }

                cv::imshow("Out",mhiImage);

                cv::Scalar meanMhi, stdDevMhi;
                cv::meanStdDev(mhiImage, meanMhi, stdDevMhi);
                int dynamicThresholdMhi=meanMhi.val[0]+2*stdDevMhi.val[0];
                qDebug()<<"dynamicThresholdMhi :"<<dynamicThresholdMhi<<"\n";
                cv::threshold(mhiImage,mhiImage,dynamicThresholdMhi,255,cv::THRESH_BINARY);
                cv::imshow("Treshed Out",mhiImage);

            //    cv::morphologyEx(mhiImage,mhiImage,cv::MORPH_CLOSE, element,cv::Point(-1,-1),4 );
           //     cv::erode(mhiImage,mhiImage, element,cv::Point(-1,-1),4 );
           //     cv::dilate(mhiImage,mhiImage, element,cv::Point(-1,-1),4 );

                cDetMhi.process(mhiImage);
                cFiltMhi.process(&cDetMhi.candidateList);
                cFiltMhi.showTargets(currentFrame,"mhiTargets");
            }

            cv::Scalar meanCurrent, stdDevCurrent;
            cv::meanStdDev(currentDiffImage, meanCurrent, stdDevCurrent);
            int dynamicThresholdCurrent=meanCurrent.val[0]+5*stdDevCurrent.val[0];
            qDebug()<<"dynamicThresholdCurrent :"<<dynamicThresholdCurrent<<"\n";

            cv::threshold(currentDiffImage,currentDiffImage,dynamicThresholdCurrent,255,cv::THRESH_BINARY);
      /*     t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();

            qDebug()<<"Processing Time :"<<t<<"\n\n";
            */


        //    cv::dilate(alignedPrevFrame,alignedPrevFrame, element,cv::Point(-1,-1),4 );
        //    cv::erode(alignedPrevFrame,alignedPrevFrame, element,cv::Point(-1,-1),4 );
            cDet.process(currentDiffImage);
            cFilt.process(&cDet.candidateList);
            cFilt.showTargets(currentFrame);
            cv::imshow(wName,currentDiffImage);
            cv::waitKey(1);
            qDebug()<<i<<"\n";
        }
        else
        {
            // Eğer dönüşüm matrisi hesaplanamaz ise motion history sıfırlanıyor...
            diffImageList.clear();
            // i-=2;
        }



       // prev.~Mat();
        prevFrame=copyCurrentFrame;
    }

}

int main(int argc, char *argv[])
{
    //CVS_UAVTargetDetectionApp targetDetection(argc, argv);
    //targetDetection.exec();
    // cvUseOptimized(true); // faydası görünmüyor
     //Test5();
    //ArtificalPeformanceTester();
    //PlayAvi("D:/cvs/data/testavi/output2.avi");

    // TEST_frameAllignment();
   //  Test6();
   // TestforVideos("D:/cvs/data/testavi/output1.avi");
    DemoforVideos();

    return 0;
}

