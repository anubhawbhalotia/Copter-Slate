#include <opencv2/highgui.hpp>
#include <bits/stdc++.h>
#include <opencv2/imgproc.hpp>
#include <fstream>
#include "copterLib.h" //includes slateLib.h
VideoCapture cap("v4l2:///dev/video0");
void thresh_callback(int, void* ,Mat src)
{
	RNG rng(12345);
  int thresh1=100;
  int thresh2=200;
  Mat canny_output;

  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  /// Detect edges using canny
  Canny(src, canny_output, thresh1, thresh2, 3 );
  /// Find contours
  findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  /// Draw contours
  Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
  for( int i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
     }

  /// Show in a window
  namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
  imshow( "Contours", drawing );
}
void drawCircles(Mat src)
{
	Mat crc(ROW,COLUMN,CV_8UC1);
	initializeMatObject(crc);
	vector<Vec3f> circles;
    HoughCircles(src, circles, CV_HOUGH_GRADIENT,2, COLUMN, 200, 25 );
    for( size_t i = 0; i < circles.size(); i++ )
    {
         Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
         int radius = cvRound(circles[i][2]);
         // draw the circle center
         circle( crc, center, 3, 250, -1, 8, 0 );
         // draw the circle outline
         circle( crc, center, 10, 250, 3, 8, 0 );
    }
    namedWindow( "circles", 1 );
    imshow( "circles", crc );
}
int main()
{
	if(cap.isOpened()==false)
	{
		cout<<"Cannot open the video file"<<endl;
		cin.get();
		return -1;
	}
	Mat original(ROW,COLUMN,CV_8UC3);
	Mat canavas(ROW,COLUMN,CV_8UC1);
	Mat canavas2(ROW,COLUMN,CV_8UC1);
	// Mat original;
	// Mat canavas;
	vector <Mat> spl;
	int type;
	cin>>type;
	
	while(1)
	{
		// bool bSuccess=cap.read(original);
		// if(bSuccess==false)		break;
		cap>>original;

		//initializeMatObject(canavas);
		CvSeq* contours;
		CvSeq* result; 
		CvMemStorage *storage = cvCreateMemStorage(0);
		invertImage(original);
		split(original,spl);
		imshow("ORIGINAL",original);
		imshow("spl0",spl[0]);
		imshow("spl1",spl[1]);
		imshow("spl2",spl[2]);
		initializeMatObject(canavas);
		binarise(spl,canavas);
		imshow("canavas",canavas);
		//blur(spl[2], spl[2], Size(10,10) );
		//GaussianBlur( spl[2], spl[2], Size(9, 9), 2, 2 );
		
		//thresh_callback( 0, 0 ,spl[2]);
		 //imshow("spl red",canavas);
		 //drawCircles(canavas);
		// 

		//cvFindContours(&canavas, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
		// while(contours)
		// {
		// 	result = cvApproxPoly(contours, sizeof(CvContour), storage, CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0);
		// 	cout<<result->total<<endl;
		// }
		//cvThreshold(&spl[2],&canavas,250,250,CV_THRESH_BINARY);
		// imshow("binarise",canavas);
		pair <int,int> centre=getCentre(canavas,100);
		drawSprite(2,centre,canavas2,20);
		imshow("canavas2",canavas2);
		// imshow("canavas",canavas);
		
		//cout<<spl[2];
		waitKey(10);
	}
	

}
