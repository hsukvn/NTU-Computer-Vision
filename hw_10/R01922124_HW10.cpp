#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <vector>

#define PI acos(-1.0)

using namespace cv;
using namespace std;

double laplacian_mask1[3][3] = {0,1,0,1,-4,1,0,1,0};
double laplacian_mask2[3][3] = {1/3.0,1/3.0,1/3.0,1/3.0,-8/3.0,1/3.0,1/3.0,1/3.0,1/3.0};
double mvl_mask[3][3] =  {2/3.0,-1/3.0,2/3.0,-1/3.0,-4/3.0,-1/3.0,2/3.0,-1/3.0,2/3.0};
double lg_mask[11][11] = {0,  0,  0,  -1, -1, -2, -1, -1, 0,  0,  0,
                          0,  0,  -2, -4, -8, -9, -8, -4, -2, 0,  0,
                          0,  -2, -7, -15,-22,-23,-22,-15,-7, -2, 0,
                          -1, -4, -15,-24,-14,-1, -14,-24,-15,-4,-1,
                          -1, -8, -22,-14,52, 103,52, -14,-22,-8,-1,
                          -2, -9, -23,-1, 103,178,103,-1, -23,-9,-2,
                          -1, -8, -22,-14,52, 103,52, -14,-22,-8,-1,
                          -1, -4, -15,-24,-14,-1, -14,-24,-15,-4,-1,
                          0,  -2, -7, -15,-22,-23,-22,-15,-7, -2, 0,
                          0,  0,  -2, -4, -8, -9, -8, -4, -2, 0,  0,
                          0,  0,  0,  -1, -1, -2, -1, -1, 0,  0,  0};

//new functions
int GetValue(vector<vector<int> >&, int, vector<vector<double> >&);
void ZeroCrossingEdgeDetector( Mat&, Mat&, int, vector<vector<double> >&, int);
void DifferenceOfGaussian ( Mat&, Mat&, int, int , int, int);


int main(){
  Mat image = imread("lena.bmp", 0); //origin
  Mat des_image;
  vector<vector<double> > mask;

  //Laplacian mask_1_25
  mask.resize(3,vector<double>());
  for (int i=0;i<3;i++){
    mask[i].insert(mask[i].end(),laplacian_mask1[i],laplacian_mask1[i]+3);
  }
  ZeroCrossingEdgeDetector(image,des_image,3,mask,25);
  imwrite("Lap_mask1_25.bmp", des_image);

  //Laplacian mask_2_16
  mask.clear();
  mask.resize(3,vector<double>());
  for (int i=0;i<3;i++){
    mask[i].insert(mask[i].end(),laplacian_mask2[i],laplacian_mask2[i]+3);
  }
  ZeroCrossingEdgeDetector(image,des_image,3,mask,16);
  imwrite("Lap_mask2_16.bmp", des_image);

  //minimum-variance Laplacian_12
  mask.clear();
  mask.resize(3,vector<double>());
  for (int i=0;i<3;i++){
    mask[i].insert(mask[i].end(),mvl_mask[i],mvl_mask[i]+3);
  }
  ZeroCrossingEdgeDetector(image,des_image,3,mask,12);
  imwrite("mv_Lap_12.bmp", des_image);


  //Laplacian of Gaussian with threshold 8000
  mask.clear();
  mask.resize(11,vector<double>());
  for (int i=0;i<11;i++){
    mask[i].insert(mask[i].end(),lg_mask[i],lg_mask[i]+11);
  }
  ZeroCrossingEdgeDetector(image,des_image,11,mask,8000);
  imwrite("Lap_of_Gaussian_8000.bmp", des_image);

  //Difference of Gaussian
  DifferenceOfGaussian(image, des_image,11,1,3,7);
  imwrite("Diff_of_Gaussian_7.bmp", des_image);
}

int GetValue(vector<vector<int> >& neighbour, int mask_size, vector<vector<double> >& mask){
  double res=0;
  for ( int i=0 ; i<mask_size ; i++ ){
    for ( int j=0 ; j<mask_size ; j++ ){ 
      res+= neighbour[i][j]*mask[i][j];
    }
  }
  return (int)res;
}

void ZeroCrossingEdgeDetector ( Mat& src, Mat& dest,int mask_size, vector<vector<double> >& mask , int threshold) {
  vector<vector<int> > temp(src.rows,vector<int>(src.cols,0));
  vector<vector<int> > neighbour(mask_size,vector<int>(mask_size,0));
  Mat res(src.rows,src.cols,0);
  int rows = src.rows, cols = src.cols;
  uchar* row_pointer;
  int offset = mask_size/2;
  for ( int i=offset ; i<rows-offset ; i++ ){
    for ( int j=offset ; j<cols-offset; j++) {
      //get neighbours
      for ( int r=i-offset ; r<=i+offset ; r++ ) {
        for ( int c = j-offset; c<=j+offset; c++ ) {
          neighbour[r-i+offset][c-j+offset] = src.ptr(r)[c];
        }
      }
      temp[i][j] = GetValue(neighbour,mask_size,mask); 
    }
  }
  
  for ( int i = 0 ; i<rows ; i++ ){
    row_pointer = res.ptr(i);
    for ( int j=0 ; j<cols; j++ ){
      if ( i<offset || j<offset || i>=rows-offset || j>=cols-offset ) row_pointer[j] = 255;
      else {
        int t = temp[i][j];
        if ( abs(t)<=threshold ) {
          row_pointer[j] = 255;
          continue;
        }
        int find = 0;
        for ( int r = i-offset ; r<=i+offset && !find ; r++ ) {
          for ( int c = j-offset ; c<=j+offset && !find; c++ ){
            int n = temp[r][c];
            if ( t>threshold && n<-threshold || t<-threshold && n>threshold ) {
              find=1;
              row_pointer[j]=0;
            }
            else
              row_pointer[j]=255;
          }
        }
      }
    }
  }
  dest = res;
}

void DifferenceOfGaussian ( Mat& src, Mat& dest, int kernel_size,int var1, int var2,int threshold ) {
  //create gaussian mask
  vector<vector<double> > k1(kernel_size,vector<double>(kernel_size,0));
  vector<vector<double> > k2(kernel_size,vector<double>(kernel_size,0));
  vector<vector<double> > kernel(kernel_size,vector<double>(kernel_size,0));
  double sum1=0,sum2=0;
  for ( int i=0 ; i<kernel_size; i++ ) {
    for ( int j=0 ; j<kernel_size; j++ ){
      k1[i][j] = 1.0/(2*PI*var1*var1)*exp(-1.0*(i*i+j*j)/(2.0*var1*var1));
      k2[i][j] = 1.0/(2*PI*var2*var2)*exp(-1.0*(i*i+j*j)/(2.0*var2*var2));
      sum1+=k1[i][j];
      sum2+=k2[i][j];
    }
  }
  //normalize and minus
  for ( int i=0 ; i<kernel_size ; i++ ){
    for (int j=0 ; j<kernel_size ; j++ ){
      if ( j ) printf(" ");
      kernel[i][j] = k1[i][j]/sum1-k2[i][j]/sum2;
      printf("%.3lf",kernel[i][j]);
    }
    printf("\n");
  }
  ZeroCrossingEdgeDetector(src,dest,kernel_size,kernel,threshold);
}