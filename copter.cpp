#include <opencv2/highgui.hpp>
#include <bits/stdc++.h>
#include <opencv2/imgproc.hpp>
#include <fstream>
#include "copterLib.h"
#define ROW 720
#define COLUMN 1280
VideoCapture cap("v4l2:///dev/video0");
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
	vector <Mat> spl;
	while(1)
	{
		bool bSuccess=cap.read(original);
		if(bSuccess==false)		break;

		//initializeMatObject(canavas);
		invertImage(original);
		split(original,spl);
		pair <int,int> centre=thresholding(canavas,spl,100);
		drawSprite(2,centre,canavas,20);
		imshow("canavas",canavas);
		waitKey(10);
	}
	

}