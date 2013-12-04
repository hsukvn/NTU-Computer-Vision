#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;


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


void AddGaussianNoise ( const Mat&, Mat& , int );
void AddSaltAndPepperNoise ( const Mat& , Mat& , double );
void BoxFilter ( const Mat&, Mat& , int , const vector<vector<int> >& );
void MedianFilter ( const Mat &, Mat& , int  , const vector<vector<int> >& );
void Open_Close(Mat &, Mat& , Kernel *);
void Close_Open(Mat &, Mat& , Kernel *);

//previous hw
void pixel_set(Mat *, int, int ,int);
uchar pixel_get(Mat *, int ,int);
Mat * dilation(Mat *, Kernel *);
uchar di_ele(Mat *, Kernel *, int , int );
Mat * erosion(Mat *, Kernel *);
uchar ero_ele(Mat *, Kernel *, int , int );
Mat * opening(Mat *, Kernel *);
Mat * closing(Mat *, Kernel *);


int main(){
	//mask
	vector<vector<int> > mask_3(3, vector<int>(3));;
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			mask_3[i][j]=1;
		}
	}
	vector<vector<int> > mask_5(5, vector<int>(5));;
	for(int i=0;i<5;i++){
		for(int j=0;j<5;j++){
			mask_5[i][j]=1;
		}
	}

	//kernel
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

	Mat image = imread("lena.bmp", 0); //origin
	//GaussianNoise
	Mat GaussianNoise_10, GaussianNoise_30;
	AddGaussianNoise(image, GaussianNoise_10, 10);
	imwrite("GaussianNoise_10.bmp", GaussianNoise_10);
	AddGaussianNoise(image, GaussianNoise_30, 30);
	imwrite("GaussianNoise_30.bmp", GaussianNoise_30);

	//SaltandPepperNoise
	Mat SaltandPepper_05, SaltandPepper_1;
	AddSaltAndPepperNoise(image, SaltandPepper_05, 0.05);
	imwrite("SaltandPepper_05.bmp", SaltandPepper_05);
	AddSaltAndPepperNoise(image, SaltandPepper_1, 0.1);
	imwrite("SaltandPepper_1.bmp", SaltandPepper_1);

	//filters on GaussianNoise 10 image
	Mat Gaussian10_box3, Gaussian10_box5, Gaussian10_median3, Gaussian10_median5, GaussianNoise10_openclose, GaussianNoise10_closeopen;
	BoxFilter(GaussianNoise_10, Gaussian10_box3, 3, mask_3);
	imwrite("Gaussian10_box3.bmp", Gaussian10_box3);
	BoxFilter(GaussianNoise_10, Gaussian10_box5, 5, mask_5);
	imwrite("Gaussian10_box5.bmp", Gaussian10_box5);
	MedianFilter(GaussianNoise_10, Gaussian10_median3, 3, mask_3);
	imwrite("Gaussian10_median3.bmp", Gaussian10_median3);
	MedianFilter(GaussianNoise_10, Gaussian10_median5, 5, mask_5);
	imwrite("Gaussian10_median5.bmp", Gaussian10_median5);
	Open_Close(GaussianNoise_10, GaussianNoise10_openclose, &ker);
	imwrite("GaussianNoise10_openclose.bmp", GaussianNoise10_openclose);
	Close_Open(GaussianNoise_10, GaussianNoise10_closeopen, &ker);
	imwrite("GaussianNoise10_closeopen.bmp", GaussianNoise10_closeopen);

	//filters on GaussianNoise 30 image
	Mat Gaussian30_box3, Gaussian30_box5, Gaussian30_median3, Gaussian30_median5, GaussianNoise30_openclose, GaussianNoise30_closeopen;
	BoxFilter(GaussianNoise_30, Gaussian30_box3, 3, mask_3);
	imwrite("Gaussian30_box3.bmp", Gaussian30_box3);
	BoxFilter(GaussianNoise_30, Gaussian30_box5, 5, mask_5);
	imwrite("Gaussian30_box5.bmp", Gaussian30_box5);
	MedianFilter(GaussianNoise_30, Gaussian30_median3, 3, mask_3);
	imwrite("Gaussian30_median3.bmp", Gaussian30_median3);
	MedianFilter(GaussianNoise_30, Gaussian30_median5, 5, mask_5);
	imwrite("Gaussian30_median5.bmp", Gaussian30_median5);
	Open_Close(GaussianNoise_30, GaussianNoise30_openclose, &ker);
	imwrite("GaussianNoise30_openclose.bmp", GaussianNoise30_openclose);
	Close_Open(GaussianNoise_30, GaussianNoise30_closeopen, &ker);
	imwrite("GaussianNoise30_closeopen.bmp", GaussianNoise30_closeopen);

	//filters on SaltandPepper 0.05 image
	Mat SaltandPepper_05_box3, SaltandPepper_05_box5, SaltandPepper_05_median3, SaltandPepper_05_median5, SaltandPepper_05_openclose, SaltandPepper_05_closeopen;
	BoxFilter(SaltandPepper_05, SaltandPepper_05_box3, 3, mask_3);
	imwrite("SaltandPepper_05_box3.bmp", SaltandPepper_05_box3);
	BoxFilter(SaltandPepper_05, SaltandPepper_05_box5, 5, mask_5);
	imwrite("SaltandPepper_05_box5.bmp", SaltandPepper_05_box5);
	MedianFilter(SaltandPepper_05, SaltandPepper_05_median3, 3, mask_3);
	imwrite("SaltandPepper_05_median3.bmp", SaltandPepper_05_median3);
	MedianFilter(SaltandPepper_05, SaltandPepper_05_median5, 5, mask_5);
	imwrite("SaltandPepper_05_median5.bmp", SaltandPepper_05_median5);
	Open_Close(SaltandPepper_05, SaltandPepper_05_openclose, &ker);
	imwrite("SaltandPepper_05_openclose.bmp", SaltandPepper_05_openclose);
	Close_Open(SaltandPepper_05, SaltandPepper_05_closeopen, &ker);
	imwrite("SaltandPepper_05_closeopen.bmp", SaltandPepper_05_closeopen);


	//filters on SaltandPepper 0.1 image
	Mat SaltandPepper_1_box3, SaltandPepper_1_box5, SaltandPepper_1_median3, SaltandPepper_1_median5, SaltandPepper_1_openclose, SaltandPepper_1_closeopen;
	BoxFilter(SaltandPepper_1, SaltandPepper_1_box3, 3, mask_3);
	imwrite("SaltandPepper_1_box3.bmp", SaltandPepper_1_box3);
	BoxFilter(SaltandPepper_1, SaltandPepper_1_box5, 5, mask_5);
	imwrite("SaltandPepper_1_box5.bmp", SaltandPepper_1_box5);
	MedianFilter(SaltandPepper_1, SaltandPepper_1_median3, 3, mask_3);
	imwrite("SaltandPepper_1_median3.bmp", SaltandPepper_1_median3);
	MedianFilter(SaltandPepper_1, SaltandPepper_1_median5, 5, mask_5);
	imwrite("SaltandPepper_1_median5.bmp", SaltandPepper_1_median5);
	Open_Close(SaltandPepper_1, SaltandPepper_1_openclose, &ker);
	imwrite("SaltandPepper_1_openclose.bmp", SaltandPepper_1_openclose);
	Close_Open(SaltandPepper_1, SaltandPepper_1_closeopen, &ker);
	imwrite("SaltandPepper_1_closeopen.bmp", SaltandPepper_1_closeopen);


}

void AddGaussianNoise ( const Mat& src, Mat& dest, int amp ) {
	Mat temp = src.clone();
	int rows = temp.rows;
	int cols = temp.cols;
	RNG rng;
	for ( int i = 0 ; i<rows ; i++ ) {
		uchar* row_pointer = temp.ptr(i);
		for ( int j=0 ; j<cols ; j++ ) {
			double rnd = rng.gaussian(1);
			int v = (int)row_pointer[j]+amp*rnd;
			row_pointer[j] = v;
		}
	}
	dest = temp;
}

void AddSaltAndPepperNoise ( const Mat& src, Mat& dest, double threshold ){
	Mat temp = src.clone();
	int rows = temp.rows;
	int cols = temp.cols;
	RNG rng;
	for ( int i = 0 ; i<rows ; i++ ) {
		uchar* row_pointer = temp.ptr(i);
		for ( int j=0 ; j<cols ; j++ ) {
			double rnd = rng.uniform((double)0,(double)1);
			if ( rnd<threshold ) {
				row_pointer[j]=0;
			}
			else if ( rnd > 1-threshold ) {
				row_pointer[j]=255;
			}
		}
	}
	dest = temp;
}

void BoxFilter ( const Mat& src, Mat& dest , int mask_size, const vector<vector<int> >& mask ){
	Mat temp = src.clone();
	int sum = 0;
	for ( int i=0 ; i<mask_size ; i++ ) {
		for ( int j=0 ;j<mask_size; j++ ) {
			sum+=mask[i][j];
		}
	}
	int rows = temp.rows;
	int cols = temp.cols;
	int tr,tc;
	for ( int r = 0 ; r<rows ; r++ ) {
		uchar* row_pointer = temp.ptr(r);
		for ( int c=0 ; c<cols ; c++ ) {
			double t = 0;
			for ( int i=0 ; i<mask_size ; i++ ) {
				int offset_r = mask_size/2-i;
				tr = offset_r + r;//find the neighbour in image
				if ( tr<0 || tr>=rows ) continue;
				uchar* mask_row_pointer =temp.ptr(tr);
				for ( int j=0; j<mask_size ; j++ ) { 
		  			int offset_c = mask_size/2-j;
		  			tc = offset_c + c;
		  			if ( tc>=0 && tc<cols ) {
		    		t+=(double)mask[i][j]/(double)sum * (int)mask_row_pointer[tc];
		  			}
				}
			}
			row_pointer[c] = (uchar)t;
		}
	}
	dest = temp;
}

void MedianFilter ( const Mat &src, Mat& dest, int mask_size , const vector<vector<int> >& mask){
	Mat temp = src.clone();
	dest = temp.clone();
	int rows = temp.rows;
	int cols = temp.cols;
	int tr,tc;
	for ( int r = 0 ; r<rows ; r++ ) {
		uchar* row_pointer = dest.ptr(r);
		for ( int c=0 ; c<cols ; c++ ) {
			vector<int> neighbours;
			for ( int i=0 ; i<mask_size ; i++ ) {
				tr = mask_size/2-i+r;
				if ( tr<0 || tr>=rows ) continue;
					uchar* mask_row_pointer = temp.ptr(tr);
				for ( int j=0; j<mask_size ; j++ ) {
					tc = mask_size/2-j+c;
					if ( tc>=0 && tc<cols ) {
						neighbours.push_back((int)mask_row_pointer[tc]); //find neighbours
					}	
				}
			}
			sort(neighbours.begin(),neighbours.end());
			row_pointer[c] = neighbours[neighbours.size()/2];
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

void Open_Close(Mat &src, Mat& dest, Kernel *k){
	Mat *pic;
	pic = opening(&src, k);
	pic = closing(pic, k);
	dest = pic->clone();
}

void Close_Open(Mat &src, Mat& dest, Kernel *k){
	Mat *pic;
	pic = closing(&src, k);
	pic = opening(pic, k);
	dest = pic->clone();
}