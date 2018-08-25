#include "slateLib.h"
using namespace cv;
using namespace std;
#define ROW 720
#define COLUMN 1080
pair <long,long> getCentre(Mat src,long terminatePixelCount=ROW*COLUMN)
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
		unsigned char *p=src.ptr(i);
		unsigned char *g=src.ptr(i);
		for(j=0;j<src.cols;j++)
		{
			if(*(p+j)>=250)
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


void drawSprite(int type,pair <int,int> centre,Mat obj,int radius=20)
{
	static pair <int,int> prevCentre=make_pair(-2,-2);
	static long long int delay=0;
	
	switch(type)
	{
		case 1: //points a + symbol around centre
			if(centre.first==-1 || centre.second==-1)
			{
				break;
			}
			initializeMatObject(obj);
			centre.first=(centre.first-radius<0)?0+radius:centre.first;
			centre.first=(centre.first+radius>ROW-1)?ROW-1-radius :centre.first;
			centre.second=(centre.second-radius<0)?0+radius:centre.second;
			centre.second=(centre.second+radius>COLUMN-1)?COLUMN-1-radius :centre.second;
			for(int i=centre.first-radius;i<centre.first+radius;i++)
			{
				*(obj.ptr(i)+centre.second)=250;
			}
			for(int i=centre.second-radius;i<centre.second+radius;i++)
			{
				*(obj.ptr(centre.first)+i)=250;
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
				return;
			}
			delay=0;	
			if(prevCentre.first!=-2 && prevCentre.second!=-2)
			{
				vector < pair <int,int> > points=getLine(centre,prevCentre);
				drawLine(points,obj);
			}
			*(obj.ptr(centre.first)+centre.second)=250;
			break;
		case 3: // points a square of desied thickness
			initializeMatObject(obj);
			if(centre.first==-1 || centre.second==-1)
			{
				break;
			}
			
			centre.first=(centre.first-radius<0)?0+radius:centre.first;
			centre.first=(centre.first+radius>ROW-1)?ROW-1-radius :centre.first;
			centre.second=(centre.second-radius<0)?0+radius:centre.second;
			centre.second=(centre.second+radius>COLUMN-1)?COLUMN-1-radius :centre.second;
			for(int i=centre.first-radius;i<centre.first+radius;i++)
			{
				for(int j=centre.second-radius;j<centre.second+radius;j++)
				{	
					*(obj.ptr(i)+j)=250;
				}
			}
			break;
	}
	prevCentre=centre;
}
void binarise(vector <Mat> &spl,Mat canavas,int p1,int p2,int p3)
{
	//converts a BGR image to binary image where red part 
	for(int i=0;i<spl[2].rows;i++)
	{
		unsigned char *pr1=spl[p1].ptr(i);
		unsigned char *pr2=spl[p2].ptr(i);
		unsigned char *pr3=spl[p3].ptr(i);
		unsigned char *q=canavas.ptr(i);
		for(int j=0;j<spl[2].cols;j++)
		{
			if(*(pr1+j)>=250 && *(pr2+j)<250 && *(pr3+j)<250)
			{
				*(q+j)=250;
			}

		}
	}
}