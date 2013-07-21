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
    for( int i = 0; i < contours.size(); i++ )
    {
        cv::approxPolyDP( cv::Mat(contours[i]), contours_poly[i], 3, true );
        boundRect[i] = cv::boundingRect( cv::Mat(contours_poly[i]) );
        cv::minEnclosingCircle( contours_poly[i], center[i], radius[i] );
    }
    /// Draw polygonal contour + bonding rects + circles

    cv::cvtColor(frame,out,CV_GRAY2BGR);

    for( int i = 0; i< contours.size(); i++ )
    {
        if(cv::contourArea(contours[i])<100 || cv::contourArea(contours[i])>2500) continue;
    //    cv::Scalar color = cv::Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        cv::Scalar color = cv::Scalar( 0, 0, 255 );
   //     cv::drawContours( out, contours_poly, i, color, 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point() );
        cv::rectangle( out, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
  //      cv::circle( out, center[i], (int)radius[i], color, 2, 8, 0 );
    }

}

void Test3()
{
    char Buf[1024];
    char *wName="Test";
    cv::Mat frame;
    cv::Mat pFrame;
    cv::namedWindow(wName);
#ifdef WIN32
    frame=cv::imread("D:/cvs/data/egt1/frame00000.jpg",CV_LOAD_IMAGE_GRAYSCALE);
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
  //  calc.setDetectorSimple("GridORB");

    calc.setHomographyMethod(featureBased);  // featurebased a göre çok hızlı

  //  calc.setHomographyCalcMethod(CV_LMEDS);
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
            cv::Mat cFrame;
            FindCandidate(aPrev,frame,cFrame);
          //  cv::imshow(wName,aPrev);


            cv::imshow(wName,cFrame);
            cv::waitKey(10);
            std::cout<<i<<"\n";
        }
        prev.~Mat();
        prev=pFrame;
    }

}

int main(int argc, char *argv[])
{
    CVS_UAVTargetDetectionApp targetDetection(argc, argv);
    targetDetection.exec();

   // Test3();

    return 0;
}

