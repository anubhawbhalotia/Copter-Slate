#include <opencv2/highgui.hpp>
#include <bits/stdc++.h>
#include <opencv2/imgproc.hpp>
#include <fstream>
#include "copterLib.h" //includes slateLib.h
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
	Mat livePoint(ROW,COLUMN,CV_8UC1);
	Mat canavas(ROW,COLUMN,CV_8UC1);
	vector <Mat> spl;
	while(1)
	{
		bool bSuccess=cap.read(original);
		if(bSuccess==false)		break;
		invertImage(original);
		split(original,spl);
		imshow("ORIGINAL",original);
		imshow("spl2",spl[2]);
		initializeMatObject(livePoint);
		binarise(spl,livePoint,2,1,0);
		imshow("livePoint",livePoint);
		pair <int,int> centre=getCentre(livePoint,100);
		int type=3;
		drawSprite(type,centre,canavas,20);
		imshow("canavas",canavas);
		waitKey(10);
	}
	

}
