#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#define Q 1
#define R 0
#define S 2

using namespace cv;
using namespace std;

void pixel_set(Mat *, int, int ,int);
uchar pixel_get(Mat *, int ,int);
void binary_image(Mat *, int);
void resize(Mat *, int, int);
int H4(int ,int , int , int );
int F4(int , int , int , int );
void YokoiConnectivityNumber (Mat * , vector<string>&);

int main(){
	Mat image=imread("lena.bmp",0);
	resize(&image,64,64);  //default: 64x64
	vector<string> result;
	YokoiConnectivityNumber(&image, result);
	fstream fp;
	fp.open("result.txt", ios::out);
	if(fp.is_open()){
		for(int i=0;i<result.size();i++){
			cout << result[i] << endl;
			fp << result[i] << endl;
		}
	}
}

void pixel_set(Mat *p, int x, int y, int value){	//set B_PIX(p,x,y) to 'value'
	uchar* tp = p->data+x*p->cols+y;
	*tp = value;
}

uchar pixel_get(Mat *p, int x,int y){			//return B_PIX(p,x,y)
	uchar* tp = p->data+x*p->cols+y;
	return *tp;
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

void resize(Mat *p, int rows, int cols){
	Mat res(rows, cols, p->type());
	int row_scale = p->rows/rows;
	int col_scale = p->cols/cols;
	for(int i=0;i<rows;i++){
		for(int j=0;j<cols;j++){
			pixel_set(&res,i,j,pixel_get(p,i*row_scale,j*col_scale));
		}
	}
	*p=res.clone();
}

int H4(int b,int c, int d, int e){
	if(b==c && ( d!=b || e!=b )){
		return Q;
	}
	if(b==c && d==b && e==b){
		return R;
	}
	if(b!=c){
		return S;
	}
	return -1;
}

int F4(int a1, int a2, int a3, int a4){
	if (a1 == a2 && a1 == a3 && a1==a4 && a1==R){
		return 5;
	}
	int n=0;
	if (a1 == Q) n++;
	if (a2 == Q) n++;
	if (a3 == Q) n++;
	if (a4 == Q) n++;
	return n;
}

void YokoiConnectivityNumber(Mat *image , vector<string>& res){
	Mat t_image;
	image->copyTo(t_image);
	binary_image(&t_image,128);
	int mv[8][2] = { 0,1, -1,0, 0,-1, 1,0, 1,1,-1,1, -1,-1, 1,-1 };
	int x[9],a[4];
	int rows = t_image.rows;
	int cols = t_image.cols;
	int tr,tc;

	res.resize(rows,string(cols,' '));

	for (int i=0;i<rows;i++){
    	for (int j=0;j<cols;j++){
      		x[0] = pixel_get(&t_image,i,j)/255;
      		if (x[0] == 0){
     			continue;
      		}
	      	for (int k=0;k<8;k++){
	        	tr = i+mv[k][0];
	        	tc = j+mv[k][1];
	        	if (tr< 0 || tr>=rows || tc<0 || tc>=cols){
	        		x[k+1] = 0;
	        	}
	       		else{
	          		x[k+1] = pixel_get(&t_image,tr,tc)/255;
	      		}
	      	}
			a[0] = H4(x[0],x[1],x[6],x[2]);
			a[1] = H4(x[0],x[2],x[7],x[3]);
			a[2] = H4(x[0],x[3],x[8],x[4]);
			a[3] = H4(x[0],x[4],x[5],x[1]);
			res[i][j] = F4(a[0],a[1],a[2],a[3])+'0';
    	}
	}
}
