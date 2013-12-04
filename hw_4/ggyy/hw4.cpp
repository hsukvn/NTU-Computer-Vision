#include <iostream>
#include <algorithm>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


using namespace cv;
using namespace std; 



int main(int argc, char *argv[]){
	int i,j,ii,jj;	
	int eroflag;

    char *imagename;
	imagename =(char*)malloc(sizeof(char)*1000);
	
	cout<<"please key the image location:"<<endl;
	gets(imagename);
	//imagename = "D:\\lena.bmp";


	Mat img = imread(imagename,0);
	Mat img2;
	Mat img2bar;
	Mat img3;

	if(img.empty())
    {
        fprintf(stderr, "Can not load image %s\n", imagename);
        return -1;
    }
 
    if( !img.data ) // 檢查是否正確載入圖像
        return -1;
	// at(y,x)

	int width = img.cols;
	int height = img.rows;
	imwrite("ori_image.bmp",img);

	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
				
				if(img.at<uchar>(i,j)<=127){
					img.at<uchar>(i,j) = 0;		
				}
				else{
					img.at<uchar>(i,j) = 255;	
				}
		}
	}

	img2=img.clone();
	//dilate(img,img3,1);

// dilation
	cout<<"dilation processing..."<<endl;
	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			if(img.at<uchar>(i,j)==255){
				for(ii=-2;ii<=2;ii++){
					for(jj=-2;jj<=2;jj++){
						if(abs(ii*jj)!=4){
							if(i+ii>=0 && i+ii<height && j+jj>=0 && j+jj<width)
								img2.at<uchar>(i+ii,j+jj)=255;
						}
					}
				}
			}
		}
	}
	/*
	cv::namedWindow("main", CV_WINDOW_AUTOSIZE); //創建視窗
	cv::imshow("main", img); //顯示圖像
	cv::namedWindow("dilation", CV_WINDOW_AUTOSIZE); //創建視窗
	cv::imshow("dilation", img2); //顯示圖像
	cv::waitKey();
	*/
	imwrite("dilation.bmp",img2);

// erosion
	cout<<"erosion processing..."<<endl;
	for(i=0;i<height;i++){
		for(j=0;j<width;j++){

			eroflag=1;
			for(ii=-2;ii<=2;ii++){
					for(jj=-2;jj<=2;jj++){
						if(abs(ii*jj)!=4){
							if(i+ii>=0 && i+ii<height && j+jj>=0 && j+jj<width)
								if(img.at<uchar>(i+ii,j+jj)==0)
								eroflag=0;
						}
					}
			}



			if(eroflag){
				img2.at<uchar>(i,j)=255;
			}
			else{
				img2.at<uchar>(i,j)=0;
			}
		}
	}

	/*
	cv::namedWindow("main", CV_WINDOW_AUTOSIZE); //創建視窗
	cv::imshow("main", img); //顯示圖像
	cv::namedWindow("erosion", CV_WINDOW_AUTOSIZE); //創建視窗
	cv::imshow("erosion", img2); //顯示圖像
	cv::waitKey();
	*/
	imwrite("erosion.bmp",img2);

// open
	cout<<"open processing..."<<endl;
	img3=img2.clone();

	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			if(img3.at<uchar>(i,j)==255){
				for(ii=-2;ii<=2;ii++){
					for(jj=-2;jj<=2;jj++){
						if(abs(ii*jj)!=4){
							if(i+ii>=0 && i+ii<height && j+jj>=0 && j+jj<width)
								img2.at<uchar>(i+ii,j+jj)=255;
						}
					}
				}
			}
		}
	}
	/*
	cv::namedWindow("main", CV_WINDOW_AUTOSIZE); //創建視窗
	cv::imshow("main", img); //顯示圖像
	cv::namedWindow("open", CV_WINDOW_AUTOSIZE); //創建視窗
	cv::imshow("open", img2); //顯示圖像
	cv::waitKey();
	*/
	imwrite("open.bmp",img2);
//close
	cout<<"close processing..."<<endl;
	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			if(img.at<uchar>(i,j)==255){
				for(ii=-2;ii<=2;ii++){
					for(jj=-2;jj<=2;jj++){
						if(abs(ii*jj)!=4){
							if(i+ii>=0 && i+ii<height && j+jj>=0 && j+jj<width)
								img2.at<uchar>(i+ii,j+jj)=255;
						}
					}
				}
			}
		}
	}
	img3=img2.clone();

	for(i=0;i<height;i++){
		for(j=0;j<width;j++){

			eroflag=1;
			for(ii=-2;ii<=2;ii++){
					for(jj=-2;jj<=2;jj++){
						if(abs(ii*jj)!=4){
							if(i+ii>=0 && i+ii<height && j+jj>=0 && j+jj<width)
								if(img3.at<uchar>(i+ii,j+jj)==0)
								eroflag=0;
						}
					}
			}



			if(eroflag){
				img2.at<uchar>(i,j)=255;
			}
			else{
				img2.at<uchar>(i,j)=0;
			}
		}
	}

	/*
	cv::namedWindow("main", CV_WINDOW_AUTOSIZE); //創建視窗
	cv::imshow("main", img); //顯示圖像
	cv::namedWindow("close", CV_WINDOW_AUTOSIZE); //創建視窗
	cv::imshow("close", img2); //顯示圖像
	cv::waitKey();

	*/
	imwrite("close.bmp",img2);
// hit and miss
	cout<<"hit and miss processing..."<<endl;
	img2=img.clone();
	img2bar=img.clone();
	
	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			img2bar.at<uchar>(i,j)=255-img2bar.at<uchar>(i,j);
		}
	}
	img3=img2.clone();

	for(i=0;i<height;i++){
		for(j=0;j<width;j++){

			eroflag=1;
			for(ii=0;ii<=1;ii++){
					for(jj=-1;jj<=0;jj++){
						if(abs(ii*jj)!=1){
							if(i+ii>=0 && i+ii<height && j+jj>=0 && j+jj<width)
								if(img3.at<uchar>(i+ii,j+jj)==0)
								eroflag=0;
						}
					}
			}

			if(eroflag){
				img2.at<uchar>(i,j)=255;
			}
			else{
				img2.at<uchar>(i,j)=0;
			}
		}
	}
	img3=img2bar.clone();

	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			eroflag=1;
			for(ii=-1;ii<=0;ii++){
					for(jj=0;jj<=1;jj++){
						if((abs(ii)+abs(jj)!=0)){
							if(i+ii>=0 && i+ii<height && j+jj>=0 && j+jj<width)
								if(img3.at<uchar>(i+ii,j+jj)==0)
								eroflag=0;
						}
					}
			}

			if(eroflag){
				img2bar.at<uchar>(i,j)=255;
			}
			else{
				img2bar.at<uchar>(i,j)=0;
			}
		}
	}

	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			if(img2.at<uchar>(i,j)==255 && img2bar.at<uchar>(i,j)==255)
				img3.at<uchar>(i,j)=255;
			else
				img3.at<uchar>(i,j)=0;
		}
	}


	/*
	cv::namedWindow("main", CV_WINDOW_AUTOSIZE); //創建視窗
	cv::imshow("main", img); //顯示圖像
	cv::namedWindow("hitandmiss", CV_WINDOW_AUTOSIZE); //創建視窗
	cv::imshow("hitandmiss", img3); //顯示圖像
	cv::waitKey();
	*/

	imwrite("hitandmiss.bmp",img3);

}
