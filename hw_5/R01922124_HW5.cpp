#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;
using namespace cv;

class Kernel{
	public:
		int cols,rows;
		int **ele;
		Kernel(int x, int y){
			rows=x;
			cols=y;
			ele = new int *[rows];
			for(int i=0;i<rows;i++){
				ele[i]=new int[cols];
			}
			for(int i=0;i<rows;i++){
				for(int j=0;j<cols;j++){
					ele[i][j]=0;
				}
			}
		}
		void set_ele(int **k){
			for(int i=0;i<rows;i++){
				for(int j=0;j<cols;j++){
					ele[i][j]=k[i][j];
				}
			}
		}
}; 

void pixel_set(Mat *, int, int ,int);
uchar pixel_get(Mat *, int ,int);
void pixel_swap(Mat *, int , int , int , int); 
Mat * dilation(Mat *, Kernel *);
uchar di_ele(Mat *, Kernel *, int , int );
Mat * erosion(Mat *, Kernel *);
uchar ero_ele(Mat *, Kernel *, int , int );
Mat * opening(Mat *, Kernel *);
Mat * closing(Mat *, Kernel *);

char *name="lena.bmp";

int main(){
	Mat image=imread(name,0);
	int oct[5][5]={0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0};
	int **p=new int*[5];
	for(int i=0;i<5;i++){
		p[i]=new int[5];
	}
	for(int i=0;i<5;i++){
		for(int j=0;j<5;j++){
			p[i][j]=oct[i][j];
		}
	}
	Kernel ker(5,5);
	ker.set_ele(p);
	Mat *di_lena, *ero_lena, *open_lena, *close_lena;
	di_lena=dilation(&image,&ker);
	imwrite("di_lena.bmp",*di_lena);
	ero_lena=erosion(&image,&ker);
	imwrite("ero_lena.bmp",*ero_lena);
	open_lena=opening(&image,&ker);
	imwrite("open_lena.bmp",*open_lena);
	close_lena=closing(&image,&ker);
	imwrite("close_lena.bmp",*close_lena);
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

Mat * dilation(Mat *p, Kernel *k){
	Mat *result=new Mat(p->rows,p->cols,0);
	for(int i=0;i<p->rows;i++){
		for(int j=0;j<p->cols;j++){
			pixel_set(result,i,j,0);
		}
	}
	for(int i=0;i<p->rows;i++){
		for(int j=0;j<p->cols;j++){
			pixel_set(result,i,j,(int)di_ele(p,k,i,j));
		}
	}
	return result;
}

uchar di_ele(Mat *p, Kernel *k, int x, int y){
	int p_x=x-(k->rows/2);
	int p_y=y-(k->cols/2);
	uchar num=pixel_get(p,x,y);
	for(int i=0;i<k->rows;i++){
		for(int j=0;j<k->cols;j++){
			if(p_x+i>=0 && p_x+i<p->rows && p_y+j>=0 && p_y+j<p->cols && k->ele[i][j]==1){
				num=(uchar)max((int)num, (int)pixel_get(p,p_x+i,p_y+j));
			}
		}
	}
	return num;
}

Mat * erosion(Mat *p, Kernel *k){
	Mat *result=new Mat(p->rows,p->cols,0);
	for(int i=0;i<p->rows;i++){
		for(int j=0;j<p->cols;j++){
			pixel_set(result,i,j,0);
		}
	}
	for(int i=0;i<p->rows;i++){
		for(int j=0;j<p->cols;j++){
			pixel_set(result,i,j,(int)ero_ele(p,k,i,j));
		}
	}
	return result;
}

uchar ero_ele(Mat *p, Kernel *k, int x, int y){
	int p_x=x-(k->rows/2);
	int p_y=y-(k->cols/2);
	uchar num=pixel_get(p,x,y);
	for(int i=0;i<k->rows;i++){
		for(int j=0;j<k->cols;j++){
			if(p_x+i>=0 && p_x+i<p->rows && p_y+j>=0 && p_y+j<p->cols && k->ele[i][j]==1){
				num=(uchar)min((int)num, (int)pixel_get(p,p_x+i,p_y+j));
			}
		}
	}
	return num;
}

Mat * opening(Mat *p, Kernel *k){
	return dilation(erosion(p,k),k);
}

Mat * closing(Mat *p, Kernel *k){
	return erosion(dilation(p,k),k);
}