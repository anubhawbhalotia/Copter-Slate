#include "slateLib.h"
using namespace cv;
using namespace std;
#define ROW 720
#define COLUMN 1280
pair <int,int> displaySplashScreenWindow0(Mat obj,Vec3b colorBgWindow0,Vec3b colorRectangle)
{
	initializeMatObject(obj,colorBgWindow0);
	int rectangleWidth=100;
	int rectangleHeight=50;
	int rectangle[]={(ROW- rectangleHeight)/2,0,(ROW- rectangleHeight)/2 + rectangleHeight-1,rectangleWidth-1};
	
	//drawing the first rectangle
	for(int i=rectangle[1];i<=rectangle[3];i++)
	{
		for(int j=rectangle[0];j<=rectangle[2];j++)
		{
			obj.at<Vec3b>(j,i)=colorRectangle;
		}
	}
	//animating rctangle
	for(int i=0;i<(COLUMN- rectangleWidth)/2;i+=1)
	{
		for(int j=rectangle[0];j<=rectangle[2];j++)
		{
			obj.at<Vec3b>(j,i)=colorBgWindow0;
		}
		for(int j=rectangle[0];j<=rectangle[2];j++)
		{
			obj.at<Vec3b>(j,i+rectangleWidth+1)=colorRectangle;
		}
		waitKey(1);
		imshow("canavas",obj);
	}
	return make_pair(rectangleHeight,rectangleWidth);
}


pair <long,long> getCentre(Mat src,long terminatePixelCount,Vec3b colorCentre)
{
	//terminatePixelCount : scanning stops when total number of active 
	//pixels founde exceeds this parameter 
	int i,j; 
	pair <int,int> centre;
	centre.first=-1;
	centre.second=-1;
	long n;
	static int callCount=0;
	callCount++;
	long count=0;
	for(i=0;i<src.rows;i++)
	{
		for(j=0;j<src.cols;j++)
		{
			Vec3b color=src.at<Vec3b>(i,j);
			if(color==colorCentre)
			{	
				//*(g+j)=250;
				centre.first+=i;
				centre.second+=j;
				count++;
			}
		}
		 if(count>terminatePixelCount)
		 {
		 	break;
		 }
	}
	if(count)
	{
		centre.first/=count;
		centre.second/=count;
	}
	if(i==src.rows)
		cout<<"scanned completely"<<callCount<<" Count="<<count<<endl;
	return centre;
}


vector < pair <int,int> > getSprite(int type,pair <int,int> centre,int radius)
{
	static pair <int,int> prevCentre=make_pair(-2,-2);
	static long long int delay=0;
	vector < pair <int,int> > spritePoints;
	switch(type)
	{
		case 1: //points a + symbol around centre
			if(centre.first==-1 || centre.second==-1)
			{
				break;
			}
			//initializeMatObject(obj,bgColor);
			cout<<"done"<<endl;
			centre.first=(centre.first-radius<0)?0+radius:centre.first;
			centre.first=(centre.first+radius>ROW-1)?ROW-1-radius :centre.first;
			centre.second=(centre.second-radius<0)?0+radius:centre.second;
			centre.second=(centre.second+radius>COLUMN-1)?COLUMN-1-radius :centre.second;
			for(int i=centre.first-radius;i<centre.first+radius;i++)
			{
				spritePoints.push_back(make_pair(centre.first,i));
				//obj.at<Vec3b>(i,centre.second)=colorSprite;
			}
			for(int i=centre.second-radius;i<centre.second+radius;i++)
			{
				spritePoints.push_back(make_pair(centre.first,i));
				//obj.at<Vec3b>(centre.first,i)=colorSprite;
			}
			break;
		case 2: //draw as thin contnuous line
			if(delay>5)
			{
				prevCentre=make_pair(-2,-2);
				delay=0;
			}
			if(centre.first==-1 || centre.second==-1)
			{
				delay++;	
				return spritePoints;
			}
			delay=0;	
			if(prevCentre.first!=-2 && prevCentre.second!=-2)
			{
				vector < pair <int,int> > points=getLine(centre,prevCentre);
				spritePoints.insert(spritePoints.end(),points.begin(),points.end());
				//drawLine(points,obj,colorSprite);
			}
			spritePoints.push_back(make_pair(centre.first,centre.second));
			//obj.at<Vec3b>(centre.first,centre.second)=colorSprite;
			//*(obj.ptr(centre.first)+centre.second)=250;
			break;
		case 3: // points a square of desied thickness
			
			if(centre.first==-1 || centre.second==-1)
			{
				break;
			}
		//	initializeMatObject(obj,bgColor);
			centre.first=(centre.first-radius<0)?0+radius:centre.first;
			centre.first=(centre.first+radius>ROW-1)?ROW-1-radius :centre.first;
			centre.second=(centre.second-radius<0)?0+radius:centre.second;
			centre.second=(centre.second+radius>COLUMN-1)?COLUMN-1-radius :centre.second;
			for(int i=centre.first-radius;i<centre.first+radius;i++)
			{
				for(int j=centre.second-radius;j<centre.second+radius;j++)
				{	
					spritePoints.push_back(make_pair(i,j));
					//obj.at<Vec3b>(i,j)=colorSprite;
				}
			}
			break;
	}
	prevCentre=centre;
	return spritePoints;
}
int displayDynamicSplashScreenWindow0(pair <int,int> centre,Mat &obj,Mat &bufferWindow0, Vec3b colorBgWindow0,Vec3b colorRectangle,Vec3b colorPointer,pair <int,int> rectangleDimension)
{
	static long pointCount=0;
	obj=bufferWindow0.clone();
	cout<<"pointCount="<<pointCount<<" "<<rectangleDimension.first*rectangleDimension.second<<endl;
	if(pointCount>=(rectangleDimension.first*rectangleDimension.second))
	{
		return 1;
	}
	
	vector < pair <int,int> > spritePoints= getSprite(3,centre,10);
	for(vector < pair <int,int> > :: iterator it=spritePoints.begin();it!=spritePoints.end();it++)
	{
		if(obj.at<Vec3b>((*it).first,(*it).second)==colorRectangle)
		{
			obj.at<Vec3b>((*it).first,(*it).second)=colorBgWindow0;
			pointCount++;
		}
	}

	bufferWindow0=obj.clone();
	drawPoints(spritePoints,obj,colorPointer);
	return 0;
	
}
void binarise(Mat src,Mat obj,int p1,int p2,int p3,Vec3b colorCentre)
{
	//converts a BGR image to binary image where red part 
	for(int i=0;i<ROW;i++)
	{
		// unsigned char *pr1=spl[p1].ptr(i);
		// unsigned char *pr2=spl[p2].ptr(i);
		// unsigned char *pr3=spl[p3].ptr(i);
		// unsigned char *q=canavas.ptr(i);
		
		for(int j=0;j<COLUMN;j++)
		{
			Vec3b color = src.at<Vec3b>(i,j);
			// if(*(pr1+j)>=250 && *(pr2+j)<250 && *(pr3+j)<250)
			if(color[p1]>=250 && color[p2]<250 && color[p3]<250)
			{
				obj.at<Vec3b>(i,j)=colorCentre;
			}

		}
	}
}
