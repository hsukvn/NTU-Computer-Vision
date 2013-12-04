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
void binary_image(Mat *, int);	
Mat * dilation(Mat *);
void di_ele(Mat *, int, int);
Mat * erotion(Mat *);
int ero_ele(Mat *, int, int);
Mat * hit_and_miss(Mat *);


char *name="lena.bmp";

int main(){
	Mat image=imread(name,0);
	Mat *di_lena, *ero_lena, *open_lena, *close_lena, *hit_and_miss_lena;
	binary_image(&image, 128);
	di_lena=dilation(&image);
	imwrite("di_lena.bmp",*di_lena);
	ero_lena=erotion(&image);
	imwrite("ero_lena.bmp",*ero_lena);
	open_lena=dilation(ero_lena);
	imwrite("open_lena.bmp",*open_lena);
	close_lena=erotion(di_lena);
	imwrite("close_lena.bmp",*close_lena);
	hit_and_miss_lena=hit_and_miss(di_lena);
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

Mat * dilation(Mat *p){
	Mat *di=new Mat(p->rows,p->cols,0);
	for(int i=0;i<p->rows;i++){
		for(int j=0;j<p->cols;j++){
			if(pixel_get(p,i,j)){ //pixel = 1
				di_ele(di,i,j);
 			}
 			else{
 				pixel_set(di,i,j,0); 
 			}
		}
	}
	return di;
}

void di_ele(Mat *p, int x, int y){
	for(int i=y-1;i<=y+1;i++){
		if(x-2>=0 && i>=0){
			pixel_set(p,x-2,i,255);
		}
		if(x+2>=0 && i>=0){
			pixel_set(p,x+2,i,255);
		}
	}
	for(int i=y-2;i<=y+2;i++){
		if(x-1>=0 && i>=0){
			pixel_set(p,x-1,i,255);
		}
		if(x>=0 && i>=0){
			pixel_set(p,x,i,255);
		}
		if(x+1>=0 && i>=0){
			pixel_set(p,x+1,i,255);
		}
	}
}

Mat * erotion(Mat *p){
	Mat *ero=new Mat(p->rows,p->cols,0);
	for(int i=0;i<p->rows;i++){
		for(int j=0;j<p->cols;j++){
			if(pixel_get(p,i,j)){ //pixel = 1
				pixel_set(ero,i,j,ero_ele(p,i,j));
 			}
 			else{
 				pixel_set(ero,i,j,0); 
 			}
		}
	}
	return ero;
}

int ero_ele(Mat *p, int x, int y){
	for(int i=y-1;i<=y+1;i++){
		if(x-2<0 || i<0){
			return 0;
		}
		else{
			if(pixel_get(p,x-2,i)!=255){
				return 0;
			}
		}
		if(x+2<0 || i<0){
			return 0;
		}
		else{
			if(pixel_get(p,x+2,i)!=255){
				return 0;
			}
		}
	}
	for(int i=y-2;i<=y+2;i++){
		if(x-1<0 || i<0){
			return 0;
		}
		else{
			if(pixel_get(p,x-1,i)!=255){
				return 0;
			}
		}
		if(x<0 || i<0){
			return 0;
		}
		else{
			if(pixel_get(p,x,i)!=255){
				return 0;
			}
		}
		if(x+1<0 || i<0){
			return 0;
		}
		else{
			if(pixel_get(p,x+1,i)!=255){
				return 0;
			}
		}
	}
	return 255;
}

Mat * hit_and_miss(Mat *p){
	Mat *A=new Mat(p->rows,p->cols,0);
	Mat *A_c=new Mat(p->rows,p->cols,0);
	Mat *temp=new Mat(p->rows,p->cols,0);
	Mat *temp_2=new Mat(p->rows,p->cols,0);
	Mat *result=new Mat(p->rows,p->cols,0);
	*A=*p;
	for(int i=0;i<p->rows;i++){         //A^c
		for(int j=0;j<p->cols;j++){
			if(pixel_get(A,i,j)==255){
				pixel_set(A_c,i,j,0);
			}
			else{
				pixel_set(A_c,i,j,255);	
			}
		}
	}
	*temp=*A;
	*temp_2=*A_c;
	for(int i=0;i<p->rows;i++){         
		for(int j=0;j<p->cols;j++){
			if(pixel_get(temp,i,j)==255){
				if(i+1>=0 && j-1>=0 && pixel_get(temp,i+1,j)==255 && pixel_get(temp,i,j-1)==255){
					pixel_set(A,i,j,255);
				}
				else{
					pixel_set(A,i,j,0);
				}
			}
			if(pixel_get(temp_2,i,j)==255){
				if(i-1>=0 && j+1>=0 && pixel_get(temp_2,i-1,j)==255 && pixel_get(temp_2,i,j-1)==255&& pixel_get(temp_2,i-1,j+1)==255){
					pixel_set(A_c,i,j,255);
				}
				else{
					pixel_set(A_c,i,j,0);
				}
			}
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