#include <opencv2/highgui.hpp>
#include <bits/stdc++.h>
#include <opencv2/imgproc.hpp>
#include <fstream>
#include "copterLib.h" //includes slateLib.h
VideoCapture cap("v4l2:///dev/video0");
//Window details
//0 - Start Screen
int window0(pair <int,int> centre, Mat &obj,Mat &bufferWindow0,Vec3b colorBgWindow0)
{
	static long countWindow0Call=0;
	Vec3b colorRectangle={255,255,255};
	Vec3b colorPointer={255,255,255};
	static pair <int,int> rectangleDimension;
	if(countWindow0Call==0)
	{
		rectangleDimension=displaySplashScreenWindow0(obj,colorBgWindow0,colorRectangle);
		bufferWindow0=obj.clone();
		//imshow("a",bufferWindow0);
	}
	else
	{
		//imshow("b",bufferWindow0);
		return displayDynamicSplashScreenWindow0(centre,obj,bufferWindow0,colorBgWindow0,colorRectangle,colorPointer,rectangleDimension);
	}
	countWindow0Call++;
	return 0;
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
	Mat livePoint(ROW,COLUMN,CV_8UC3);
	Mat canavas(ROW,COLUMN,CV_8UC3);
	vector <Mat> spl;
	Vec3b colorSprite={51,153,255};
	Vec3b colorBgCopter={0,0,0};
	Vec3b colorCentre={255,255,255};
	// uchar spriteColor[]={255,255,0};
	 //uchar bgColor[]={0,204,0};
	Vec3b colorBlack={0,0,0};
	long long while1count=0;
	int currentWindow=0;
	//Window 0 declarations
	Vec3b colorBgWindow0={0,0,0};
	Mat bufferWindow0(ROW,COLUMN,CV_8UC3);
	//long  long countWindow0Call=0;
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
		switch(currentWindow)
		{
			case 0:
				
				currentWindow=window0(centre,canavas,bufferWindow0,colorBgWindow0);
				//imshow("a",bufferWindow0);
				break;
			default:
				int inp;
				cout<<"done"<<endl;
				cin>>inp;
				//countWindow0Call++;
		}
		//drawSprite(type,centre,canavas,colorSprite,colorBgCopter,20);
		imshow("canavas",canavas);

		waitKey(10);
	}
	

}
