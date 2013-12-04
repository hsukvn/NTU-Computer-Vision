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
void binary_image(char *, char *,int);
int* histogram(char *);
void histo_show(int *);
void binary_image_show(char *,char *,int **);       //write arr into txt
int** binary_image_arr(char *, int);
//adjust
void classify(char *, int **);
void eq_class(int, int);
void eq_class(int);
void show_eq_class();
void eq_count(char *, int **);
void block_comp(char *,char *, int);

class comp{
	public:
		int x;
		comp* next;
		comp(int num){
			x=num;
			next=NULL;
		}
};

class eq_class_head{
	private:
		void insert(comp *p){
			if(start==NULL || start->x>p->x){
				p->next=start;
				start=p;
				return;
			}
			else{
				comp *ptr=start;
				while(ptr!=NULL){
					if(ptr->next==NULL || ptr->next->x>p->x){
						p->next=ptr->next;
						ptr->next=p;
						return;
					}
					ptr=ptr->next;
				}
			}
		}
	public:
		comp* start;
		int count,x_l,x_r,y_u,y_d;
		eq_class_head* next;
		eq_class_head(){
			start=NULL;
			next=NULL;
			count=x_r=y_d=0;
			x_l=y_u=10000;
		}
		void eq_class_insert(int num){
			comp *p = new comp(num);
			insert(p);
		}
};

char *name="lena.bmp";
eq_class_head *eq_table=NULL;     //equivilent table


int main(){
	//Binarize Lena with the threshold 128
	binary_image(name, "bi_lena.bmp", 128);
	//histogram
	int *his=histogram(name);
	histo_show(his);
	//connected components
	int **arr=binary_image_arr(name,128);
    classify(name, arr);
	eq_count(name, arr);
	block_comp(name,"bi_lena_4connected.bmp",128);
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

void binary_image(char *input, char *output, int treshold){
	Mat image=imread(input,0);
	for(int i=0;i<image.rows;i++){
		for(int j=0;j<image.cols;j++){
			if((int)pixel_get(&image,i,j)>=128){
				pixel_set(&image,i,j,255);
			}
			else{
				pixel_set(&image,i,j,0);
			}
		}
	}
	imwrite(output,image);
}

int * histogram(char *input){
	Mat image=imread(input,0);
	int *histo=new int[256];
	for(int i=0;i<256;i++){		//array initial
		histo[i]=0;
	}
	for(int i=0;i<image.rows;i++){
		for(int j=0;j<image.cols;j++){
			histo[(int)pixel_get(&image,i,j)]++;
		}
	}
	histo_show(histo);
	return histo;
}

void histo_show(int *p){
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
    
    cvSaveImage("histogram.bmp",imgHistogram);
}


void binary_image_show(char *input, char *output, int **arr){
    Mat image=imread(input,0);
    ofstream ofile;
	ofile.open(output,ios::out);
    for(int i=0;i<image.rows;i++){
        for(int j=0;j<image.cols;j++){
            ofile << arr[i][j];
        }
        ofile << endl;
    }
    ofile.close();
    
}

int** binary_image_arr(char *input, int treshold){
    Mat image=imread(input,0);
    int **arr = new int*[image.rows];
    for(int i=0;i<image.rows;i++){
        arr[i]=new int[image.cols];
    }
    for(int i=0;i<image.rows;i++){
        for(int j=0;j<image.cols;j++){
            if((int)pixel_get(&image,i,j)>=128){
                arr[i][j]=1;
            }
            else{
                arr[i][j]=0;
            }
        }
    }
    return arr;
}

void classify(char *input, int **arr){
	Mat image=imread(input,0);
	int row=image.rows;
	int col=image.cols;
	int comp=0; // components
	for(int i=0;i<row;i++){
		for(int j=0;j<col;j++){
			if(arr[i][j]>0){ //a component
				if(i-1>=0 && arr[i-1][j]>0){  //up pixel is a component	
					arr[i][j]=arr[i-1][j];
					if(j-1>=0 && arr[i][j-1]>0 && arr[i][j-1]!=arr[i-1][j]){
						//cout << "eq_class " << arr[i][j] << " " << arr[i][j-1] << endl;
						eq_class(arr[i][j],arr[i][j-1]);//eq_class(arr[i][j],arr[i][j-1]); //make x and y same class
					}
				}
				else if(j-1>=0 && arr[i][j-1]>0){
					arr[i][j]=arr[i][j-1];
				}
				else{
					arr[i][j]=++comp;
					eq_class(arr[i][j]);//eq_class(arr[i][j]) create a new class
				}
			}
		}
	}
}

void eq_class(int x, int y){
	eq_class_head *p=eq_table;	
	eq_class_head *x_locate=NULL, *y_locate=NULL;
	while(p!=NULL){
		comp *ele=p->start;		
		while(ele!=NULL){
			if(ele->x==x){
				x_locate=p;
				break;
			}
			else if(ele->x==y){
				y_locate=p;
				break;
			}
			ele=ele->next;
		}
		if(x_locate!=NULL && y_locate!=NULL){
			break;
		}
		p=p->next;
	}

	if(x_locate==NULL || y_locate==NULL){
		return;
	}
	comp *y_num=y_locate->start;
	while(y_num!=NULL){
		x_locate->eq_class_insert(y_num->x);
		comp *temp=y_num;
		y_num=y_num->next;
		delete temp;
	}
		
	//delete class y;
	if(y_locate==eq_table){
		eq_table=eq_table->next;
		delete y_locate;
	}
	else{
		eq_class_head *ptr=eq_table;
		while(ptr!=NULL){
			if(ptr->next==y_locate){
				ptr->next=y_locate->next;
				delete y_locate;
				break;
			}
			ptr=ptr->next;
		}
	}
}

void eq_class(int x){
	eq_class_head *p=new eq_class_head;
	p->eq_class_insert(x);
	eq_class_head *ptr=eq_table;
	if(ptr==NULL){
		eq_table=p;
	}
	else{
		while(ptr->next!=NULL){
			ptr=ptr->next;
		}
		ptr->next=p;
	}
}

void show_eq_class(){
	eq_class_head *p=eq_table;
	while(p!=NULL){
		if(p->count>=500){
			comp *c=p->start;
			while(c!=NULL){
				cout << c->x << " ";
				c=c->next;
			}
			cout << "count: " << p->count << endl;
			cout << "x_l: " << p->x_l << "x_r: " << p->x_r << "y_u: " << p->y_u << "y_d: " << p->y_d << endl;
 		}
 		p=p->next;
	}
}

void eq_count(char *input, int **arr){
	Mat image=imread(input,0);
	int row=image.rows;
	int col=image.cols;
	eq_class_head *p=eq_table;
	while(p!=NULL){
		comp *ele=p->start;
		int l=ele->x;
		while(ele!=NULL){
			for(int i=0;i<row;i++){
				for(int j=0;j<col;j++){
					if(arr[i][j]==ele->x){
						arr[i][j]=l;
						p->count++;
						if(p->x_l>i){
							p->x_l=i;
						}
						if(p->x_r<i){
							p->x_r=i;
						}
						if(p->y_u>j){
							p->y_u=j;
						}
						if(p->y_d<j){
							p->y_d=j;
						}
					}
				}
			}
			ele=ele->next;
		}
		p=p->next;
	}
	
}

void block_comp(char *input,char *output, int treshold){
	cv::Mat3b image=imread(input);
	for(int i=0;i<image.rows;i++){
		for(int j=0;j<image.cols;j++){
			cv::Point point(i,j);
			const cv::Vec3b& bgr = image(point);
			int gray = (int)(0.299*(float)bgr[2]+0.587*(float)bgr[1]+0.114*(float)bgr[0]);
			if(gray>=128){
				image.at<cv::Vec3b>(j,i)[0] = 255;
				image.at<cv::Vec3b>(j,i)[1] = 255;
				image.at<cv::Vec3b>(j,i)[2] = 255;
			}
			else{
				image.at<cv::Vec3b>(j,i)[0] = 0;
				image.at<cv::Vec3b>(j,i)[1] = 0;
				image.at<cv::Vec3b>(j,i)[2] = 0;
			}
		}
	}
	eq_class_head *p=eq_table;
	while(p!=NULL){
		if(p->count>=500){
			for(int i=p->x_l;i<=p->x_r;i++){
				image.at<cv::Vec3b>(i,p->y_u)[0] = 200;
				image.at<cv::Vec3b>(i,p->y_u)[1] = 100;
				image.at<cv::Vec3b>(i,p->y_u)[2] = 0;
				image.at<cv::Vec3b>(i,p->y_d)[0] = 200;
				image.at<cv::Vec3b>(i,p->y_d)[1] = 100;
				image.at<cv::Vec3b>(i,p->y_d)[2] = 0;
			}
			for(int i=p->y_u;i<=p->y_d;i++){
				image.at<cv::Vec3b>(p->x_r,i)[0] = 200;
				image.at<cv::Vec3b>(p->x_r,i)[1] = 100;
				image.at<cv::Vec3b>(p->x_r,i)[2] = 0;
				image.at<cv::Vec3b>(p->x_l,i)[0] = 200;
				image.at<cv::Vec3b>(p->x_l,i)[1] = 100;
				image.at<cv::Vec3b>(p->x_l,i)[2] = 0;
			}
			int x_w=(p->x_l+p->x_r)/2;
			int y_w=(p->y_u+p->y_d)/2;
			for(int i=x_w-5;i<=x_w+5;i++){
				image.at<cv::Vec3b>(i,y_w)[0] = 0;
				image.at<cv::Vec3b>(i,y_w)[1] = 0;
				image.at<cv::Vec3b>(i,y_w)[2] = 255;
			}
			for(int i=y_w-5;i<=y_w+5;i++){
				image.at<cv::Vec3b>(x_w,i)[0] = 0;
				image.at<cv::Vec3b>(x_w,i)[1] = 0;
				image.at<cv::Vec3b>(x_w,i)[2] = 255;
			}
		}
		p=p->next;
	}
	imwrite(output,image);	
}
