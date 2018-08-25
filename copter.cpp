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
	Mat livePoint(ROW,COLUMN,CV_8UC3);
	Mat canavas(ROW,COLUMN,CV_8UC3);
	vector <Mat> spl;
	Vec3b colorSprite={51,153,255};
	Vec3b colorBgCopter={0,255,109};
	Vec3b colorCentre={255,255,255};
	// uchar spriteColor[]={255,255,0};
	 //uchar bgColor[]={0,204,0};
	Vec3b colorBlack={0,0,0};
	long long while1count=0;
	while(1)
	{
		bool bSuccess=cap.read(original);
		if(bSuccess==false)		break;
		invertImage(original);
		split(original,spl);
		imshow("ORIGINAL",original);
		imshow("spl2",spl[2]);
		initializeMatObject(livePoint,colorBlack);
		binarise(original,livePoint,2,1,0,colorCentre);
		imshow("livePoint",livePoint);
		pair <int,int> centre=getCentre(livePoint,100,colorCentre);
		int type=3;
		drawSprite(type,centre,canavas,colorSprite,colorBgCopter,20);
		imshow("canavas",canavas);

		waitKey(10);
	}
	

}
