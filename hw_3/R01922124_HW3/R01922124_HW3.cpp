#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;
using namespace cv;

void pixel_set(Mat *, int, int ,int);
uchar pixel_get(Mat *, int ,int);
void pixel_swap(Mat *, int , int , int , int); 	
int* histogram(Mat *, char *);
int* histogram_equlization(Mat *);
void histo_show(int *, char *);

char *name="lena.bmp";

int main(){
	Mat image=imread(name,0);
	//histogram
	int *his=histogram(&image, "histogram.bmp");
	delete []his;
	//histogram_equlization
	his=histogram_equlization(&image);
	imwrite("he_lena.bmp",image);
	//Binarize Lena with the threshold 128
	return 0;
}

void pixel_set(Mat *p, int x, int y, int value){	//set B_PIX(p,x,y) to 'value'
	uchar* tp = p->data+x*p->cols+y;
	*tp = value;
}

uchar pixel_get(Mat *p, int x,int y){			//return B_PIX(p,x,y)
	uchar* tp = p->data+x*p->cols+y;
	return *tp;
}

void pixel_swap(Mat *p, int x, int y, int xp, int yp){	//swap B_PIX(p,x,y) and B_PIX(p,xp,yp)
	uchar pix = pixel_get(p,xp,yp);
	pixel_set(p,xp,yp,pixel_get(p,x,y));
	pixel_set(p,x,y,pix);
}

int * histogram(Mat *image, char *name){
	int *histo=new int[256];
	for(int i=0;i<256;i++){		//array initial
		histo[i]=0;
	}
	for(int i=0;i<image->rows;i++){
		for(int j=0;j<image->cols;j++){
			histo[(int)pixel_get(image,i,j)]++;
		}
	}
	histo_show(histo,name);
	return histo;
}

int * histogram_equlization(Mat *image){
	int *intensity=histogram(image, "he_histogram.bmp");
	int s[256];
	float he[256];
	for(int i=0;i<256;i++){
		s[i]=0;	
		he[i]=0;	
		for(int j=0;j<=i;j++){
			he[i]+=(float)intensity[j];		
		}
		s[i]=255*he[i]/(image->rows*image->cols);
	}
	for(int i=0;i<image->rows;i++){
		for(int j=0;j<image->cols;j++){
			pixel_set(image, i, j, s[(int)pixel_get(image, i, j)]);
		}
	}
	delete []intensity;
	return histogram(image, "he_histogram.bmp");
}

void histo_show(int *p, char *name){
    IplImage* imgHistogram = 0;
    imgHistogram = cvCreateImage(cvSize(256, 200), 8, 1);
    cvRectangle(imgHistogram, cvPoint(0, 0), cvPoint(256, 200), CV_RGB(0, 0, 0), -1);
    int max_value=0;
    for(int i=0;i<256;i++){
        if(p[i]>max_value){
            max_value=p[i];
        }
    }
    for(int i=0;i<256;i++){
        int normalized = cvRound(p[i] * 200 / max_value);
        cvLine(imgHistogram, cvPoint(i, 200), cvPoint(i, 200 - normalized), CV_RGB(255, 255, 255));
    }
    
    cvSaveImage(name,imgHistogram);
}
