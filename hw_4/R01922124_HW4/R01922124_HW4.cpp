#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <cstring>
#include <fstream>
#include "robotic_library.h"

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
void binary_image(Mat *, int);	
Mat * dilation(Mat *, Kernel *);
void di_ele(Mat *, Kernel *, int , int );
Mat * erosion(Mat *, Kernel *);
int ero_ele(Mat *, Kernel *, int , int );
Mat * hit_and_miss(Mat *);

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
	Mat *di_lena, *ero_lena, *open_lena, *close_lena, *hit_and_miss_lena;
	binary_image(&image, 128);
	di_lena=dilation(&image,&ker);
	imwrite("di_lena.bmp",*di_lena);
	ero_lena=erosion(&image,&ker);
	imwrite("ero_lena.bmp",*ero_lena);
	open_lena=dilation(ero_lena,&ker);
	imwrite("open_lena.bmp",*open_lena);
	close_lena=erosion(di_lena,&ker);
	imwrite("close_lena.bmp",*close_lena);
	hit_and_miss_lena=hit_and_miss(&image);
	imwrite("hit_and_miss_lena.bmp",*hit_and_miss_lena);
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

void binary_image(Mat *p, int treshold){
	for(int i=0;i<p->rows;i++){
		for(int j=0;j<p->cols;j++){
			if((int)pixel_get(p,i,j)>=128){
				pixel_set(p,i,j,255);
			}
			else{
				pixel_set(p,i,j,0);
			}
		}
	}
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
			if(pixel_get(p,i,j)==255){
				di_ele(result,k,i,j);
			}
		}
	}
	return result;
}

void di_ele(Mat *p, Kernel *k, int x, int y){
	int p_x=x-(k->rows/2);
	int p_y=y-(k->cols/2);
	for(int i=0;i<k->rows;i++){
		for(int j=0;j<k->cols;j++){
			if(p_x+i>=0 && p_x+i<p->rows && p_y+j>=0 && p_y+j<p->cols && k->ele[i][j]==1){
				pixel_set(p,p_x+i,p_y+j,255);
			}
		}
	}
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
			pixel_set(result,i,j,ero_ele(p,k,i,j));
		}
	}
	return result;
}

int ero_ele(Mat *p, Kernel *k, int x, int y){
	int p_x=x-(k->rows/2);
	int p_y=y-(k->cols/2);
	for(int i=0;i<k->rows;i++){
		for(int j=0;j<k->cols;j++){
			if(k->ele[i][j]==1){
				if(p_x+i<0 || p_x+i>=p->rows || p_y+j<0 || p_y+j>=p->cols){ //out of bound
					//continue;
					return 0;
				}
				if(pixel_get(p,p_x+i,p_y+j)!=255){
					return 0;
				}
			}
		}
	}
	return 255;
}

Mat * hit_and_miss(Mat *p){
	Kernel j(3,3);
	Kernel k(3,3);
	int j_arr[3][3]={0,0,0,1,1,0,0,1,0};
	int k_arr[3][3]={0,1,1,0,0,1,0,0,0};
	int **arr_j=new int *[3];
	int **arr_k=new int *[3];
	for(int i=0;i<3;i++){
		arr_j[i]=new int[3];
		arr_k[i]=new int[3];
	}
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			arr_j[i][j]=j_arr[i][j];
			arr_k[i][j]=k_arr[i][j];
		}
	}
	j.set_ele(arr_j);
	k.set_ele(arr_k);
	Mat *A=new Mat(p->rows,p->cols,0);
	Mat *A_c=new Mat(p->rows,p->cols,0);
	Mat *result=new Mat(p->rows,p->cols,0);
	for(int i=0;i<A_c->rows;i++){
		for(int j=0;j<A_c->cols;j++){	
			pixel_set(A,i,j,pixel_get(p,i,j));
			pixel_set(A_c,i,j,255-pixel_get(p,i,j));
		}
	}
	A=erosion(A,&j);
	A_c=erosion(A_c,&k);
	for(int i=0;i<p->rows;i++){
		for(int j=0;j<p->cols;j++){
			if(pixel_get(A,i,j)==255 && pixel_get(A_c,i,j)==255){
				pixel_set(result,i,j,255);
			}
			else{
				pixel_set(result,i,j,0);
			}
		}
	}
	return result;
}
