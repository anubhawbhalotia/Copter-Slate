using namespace cv;
using namespace std;
#define ROW 720
#define COLUMN 1280
void invertImage(Mat obj)
{
	int i,j,temp;
	for(i=0;i<obj.rows;i++)
	{
		unsigned char *p=obj.ptr(i);
		for(j=0;j<obj.cols/2;j++)
		{
			temp=*(p+(3*j));
			*(p+(3*j))=*(p+(3*(obj.cols-j-1)));
			*(p+(3*(obj.cols-j-1)))=temp;
			temp=*(p+(3*j)+1);
			*(p+(3*j)+1)=*(p+(3*(obj.cols-j-1))+1);
			*(p+(3*(obj.cols-j-1))+1)=temp;
			temp=*(p+(3*j)+2);
			*(p+(3*j)+2)=*(p+(3*(obj.cols-j-1))+2);
			*(p+(3*(obj.cols-j-1))+2)=temp;
		}
	}
}
pair <long,long> thresholding(Mat obj,vector <Mat> spl,long terminatePixelCount=ROW*COLUMN)
{
	int i,j; 
	pair <int,int> centre;
	centre.first=-1;
	centre.second=-1;
	long n;
	static int callCount=0;
	callCount++;
	long count=0;
	for(i=0;i<spl[2].rows;i++)
	{
		unsigned char *p=spl[2].ptr(i);
		unsigned char *g=obj.ptr(i);
		for(j=0;j<spl[2].cols;j++)
		{
			if(*(p+j)>250)
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
	if(i==spl[2].rows)
		cout<<"scanned completely"<<callCount<<" Count="<<count<<endl;
	return centre;
}
void initializeMatObject(Mat obj)
{
	int i,j;
	for(i=0;i<obj.rows;i++)
	{
		unsigned char *p=obj.ptr(i);
		for(j=0;j<obj.cols;j++)
			*(p+j)=0;
	}
}
vector < pair <int,int> > getLine(pair <int,int> a,pair <int,int> b)
{
	vector < pair <int,int> > points;
	int dx=abs(b.first-a.first);
	int dy=abs(b.second-a.second);
	//direction :-  1 for increment, 0 for decrement
	int dirx=(b.first-a.first>=0)?1:-1;
	int diry=(b.second-a.second>=0)?1:-1;
	if(dx>=dy)
	{
		float slope=(float)(dx)/(float)(dy);
		int x=0,y;
		while(x!=dx)
		{
			x+=1;
			y=floor((float)x/slope);
			points.push_back(make_pair(a.first+(dirx*x),a.second+(diry*y)));
		}
	}
	else
	{
		float slope=(float)(dy)/(float)(dx);
		int y=0,x;
		while(y!=dy)
		{
			y+=1;
			x=floor((float)y/slope);
			points.push_back(make_pair(a.first+(dirx*x),a.second+(diry*y)));
		}
	}
	return points;
}
void drawLine(vector < pair < int,int>> points,Mat obj)
{
	for(vector <pair <int,int> > :: iterator it=points.begin();it!=points.end();it++)
	{
		*(obj.ptr((*it).first)+((*it).second))=250;
	}
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
				for(int j=centre.second-radius;j<centre.second+radius;j++)
				{	
					*(obj.ptr(i)+j)=250;
				}
			}
			break;
	}
	prevCentre=centre;
}