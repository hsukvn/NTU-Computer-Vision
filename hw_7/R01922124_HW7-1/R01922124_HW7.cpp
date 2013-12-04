#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

#define Q 1
#define R 0
#define S 2

#define P 1
#define Q_2 0

using namespace cv;
using namespace std;

void pixel_set(Mat &, int, int ,int);
uchar pixel_get(Mat &, int ,int);
void binary_image(Mat *, int);
void resize(Mat *, int, int);
int F4(int , int , int , int );
int F4(int , int , int , int,int,int );
int H4(int,int, int, int); 
void YokoiConnectivityNumber (Mat&, Mat &);
bool ConnectedShrink (const Mat& ,Mat& , Mat&  );
void PairRelationship ( const Mat&  , Mat&  );
void Thinning ( const Mat& , Mat& );

int main(){
	Mat image=imread("lena.bmp",0);
	Mat result;
	resize(&image,64,64);  //default: 64x64
	Thinning(image, result);
	imwrite("thinning_lena.bmp",result);
}

void pixel_set(Mat& image, int x, int y, int value){
	uchar* tp = image.data+x*image.cols+y;
	*tp = value;
}

uchar pixel_get(Mat& image, int x, int y){
	uchar* tp = image.data+x*image.cols+y;
	return *tp;
}

void binary_image(Mat *p, int treshold){
	for(int i=0;i<p->rows;i++){
		for(int j=0;j<p->cols;j++){
			if((int)pixel_get(*p,i,j)>=128){
				pixel_set(*p,i,j,255);
			}
			else{
				pixel_set(*p,i,j,0);
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
			pixel_set(res,i,j,pixel_get(*p,i*row_scale,j*col_scale));
		}
	}
	*p=res.clone();
}

int F4(int a1, int a2, int a3, int a4) {
	if ( a1 == a2 && a1 == a3 && a1==a4 && a1==R ) return 5;
	int n=0;
	if ( a1 == Q ) n++;
	if ( a2 == Q ) n++;
	if ( a3 == Q ) n++;
	if ( a4 == Q ) n++;
	return n;
}

int F4(int a1, int a2, int a3, int a4, int g, int x){
	if(a1+a2+a3+a4==1)
		return g;
	else
		return x;
}

int H4(int b,int c, int d, int e) {
	if ( b==c && ( d!=b || e!=b ) )
		return Q;
	if ( b==c && d==b && e==b )
		return R;
	if ( b!=c ) 
		return S;
	return -1;
}

void YokoiConnectivityNumber(Mat &image , Mat &res){
	Mat t_image;
	image.copyTo(t_image);
	int mv[8][2] = { 0,1, -1,0, 0,-1, 1,0, 1,1,-1,1, -1,-1, 1,-1 };
	int x[9],a[4];
	int rows = t_image.rows;
	int cols = t_image.cols;
	int tr,tc;

	image.copyTo(res);

	for(int i=0;i<rows;i++){
    	for(int j=0;j<cols;j++){
      		pixel_set(res,i,j,255);
    	}
 	 }

 	for (int i=0;i<rows;i++){
    	for (int j=0;j<cols;j++){
      		x[0] = pixel_get(t_image,i,j)/255;
      		if (x[0] == 0 ) continue;
      		for (int k=0;k<8;k++){
        		tr = i+mv[k][0];
        		tc = j+mv[k][1];
        	if (tr< 0||tr>=rows||tc<0||tc>=cols) 
          		x[k+1] = 0;
        	else
          		x[k+1] = pixel_get(t_image,tr,tc)/255;
      	}
      	a[0] = H4(x[0],x[1],x[6],x[2]);
      	a[1] = H4(x[0],x[2],x[7],x[3]);
      	a[2] = H4(x[0],x[3],x[8],x[4]);
      	a[3] = H4(x[0],x[4],x[5],x[1]);
      	pixel_set(res,i,j,F4(a[0],a[1],a[2],a[3]));
    }
  }
}

bool ConnectedShrink (const Mat& src ,Mat& data, Mat& dest){
	Mat t_image;
	src.copyTo(t_image);
	bool flag = false;
	int mv[8][2] = { 0,1, -1,0, 0,-1, 1,0, 1,1,-1,1, -1,-1, 1,-1 };
	int x[9],a[4];
	int rows = t_image.rows;
	int cols = t_image.cols;
	int tr,tc;

	for ( int i=0 ; i<rows ; i++ ) {
		for ( int j=0 ; j<cols ; j++ ) {
	  		if ( pixel_get(data,i,j) != P ) 
	  			continue;
	  		x[0]=255;
	  		for ( int k=0 ; k<8 ; k++ ) {
	    		tr = i+mv[k][0];
	    		tc = j+mv[k][1];
	    		if ( tr< 0 || tr>=rows || tc<0 || tc>=cols ) 
	      			x[k+1] = 0;
	    		else
	      			x[k+1] = pixel_get(t_image,tr,tc);
	 		}
	  		a[0] = H4(x[0],x[1],x[6],x[2])==1?1:0;
	  		a[1] = H4(x[0],x[2],x[7],x[3])==1?1:0;
	  		a[2] = H4(x[0],x[3],x[8],x[4])==1?1:0;
	  		a[3] = H4(x[0],x[4],x[5],x[1])==1?1:0;
	 		int t = F4(a[0],a[1],a[2],a[3],0,255);
	  		if ( t == 0 ) 
	  			flag = true; 
	 		 pixel_set(t_image,i,j,t);
	  
		}
	}
	t_image.copyTo(dest);
	return flag;
}

void PairRelationship ( const Mat& src , Mat& dest ){
	Mat t_image;
	src.copyTo(t_image);
	int mv[8][2] = { 0,1, -1,0, 0,-1, 1,0, 1,1,-1,1, -1,-1, 1,-1 };
	int x[9],a[4];
	int rows = t_image.rows;
	int cols = t_image.cols;
	int tr,tc;

	src.copyTo(dest);
	for ( int i=0 ; i<rows ; i++ ) {
		for ( int j=0 ; j<cols ; j++ ) {
			pixel_set(dest,i,j,255);
		}
	}

	for ( int i=0 ; i<rows ; i++ ) {
		for ( int j=0 ; j<cols ; j++ ) {
			x[0] = pixel_get(t_image,i,j);
			if ( x[0] == 255 ) 
				continue;
			if ( x[0] == 1 ) {
				int k;
				for ( k=0 ; k<4 ; k++ ) {
	  				tr = i+mv[k][0];
	  				tc = j+mv[k][1];
	  				if ( tr>= 0 && tr<rows && tc>=0 && tc<cols && pixel_get(t_image,tr,tc) == 1)
	    			break;
				}
				if ( k<4 ){
	  				pixel_set(dest,i,j,P); 
	  				continue;
				}
			}
			pixel_set(dest,i,j,Q_2); 
		}
	}
}


void Thinning(const Mat& src , Mat& dest){
	Mat data;
	Mat t_image = src.clone();
	bool flag = true;
	binary_image(&t_image,128);
	imwrite("binary_lena.bmp",t_image);
	while ( flag ){
		YokoiConnectivityNumber(t_image,data);
		PairRelationship(data,data);
		flag = ConnectedShrink(t_image,data,dest);
		t_image = dest.clone();
	}
}
