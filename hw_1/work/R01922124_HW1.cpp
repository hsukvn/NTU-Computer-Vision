#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include<iostream>
#include <cstring>

using namespace std;
using namespace cv;

void pixel_set(Mat *, int, int ,int);
uchar pixel_get(Mat *, int ,int);
void pixel_swap(Mat *, int , int , int , int);
void up_side_down(char *, char *);
void right_side_left(char *, char *); 
void diagonally_mirrored(char *, char *); 
void ano_diagonally_mirrored(char *, char *);



int main(){
	up_side_down("lena.bmp","up_side_down.bmp");	
	right_side_left("lena.bmp","right_side_left.bmp");
	diagonally_mirrored("lena.bmp","diagonally_mirrored.bmp");
	//ano_diagonally_mirrored("lena.bmp","diagonally_mirrored_2.bmp");

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

void up_side_down(char *input, char *output){
	Mat image=imread(input,0); 			//0: gray image
	
	for (int i=0; i<image.rows/2; i++){ 
		for (int j=0; j<image.cols; j++){
			pixel_swap(&image,i,j,image.rows-1-i,j);
		}
	}

	imwrite(output,image);				//write image
} 

void right_side_left(char *input, char *output){
	Mat image=imread(input,0); 			//0: gray image
	
	for (int i=0; i<image.rows; i++){ 		
		for (int j=0; j<image.cols/2; j++){
			pixel_swap(&image,i,j,i,image.cols-1-j);
		}
	}

	imwrite(output,image);				//write image
}

void diagonally_mirrored(char *input, char *output){
	Mat image=imread(input,0); 			//0: gray image
	
	for (int i=0; i<image.rows; i++){ 		//up side down
		for (int j=i; j<image.cols; j++){
			pixel_swap(&image,i,j,j,i);
		}
	}

	imwrite(output,image);		
}

void ano_diagonally_mirrored(char *input, char *output){
	Mat image=imread(input,0); 			//0: gray image
	
	for (int i=0; i<image.rows; i++){ 		//up side down
		for (int j=0; j<image.cols-i; j++){
			pixel_swap(&image,i,j,image.rows-1-j,image.cols-1-i);
		}
	}
    
	imwrite(output,image);
}
