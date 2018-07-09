#include<iostream>
#include <opencv2/opencv.hpp>  
#include <opencv2/video/background_segm.hpp>
#include <string>
#include <iomanip>
using namespace cv;  
using namespace std;
const unsigned char FORE_GROUD = 255;  
int thresh = 5;  
int main(int argc,char*argv[])  
{     
	VideoCapture video("smoke.avi");   
	if(!video.isOpened()){ 
		std::cout<<"no video detected!"<<std::endl;	
		return -1;
	}  
	Mat currentBGRFrame;  
	Mat previousSecondGrayFrame;  
	Mat previousFirstGrayFrame;  
	Mat currentGaryFrame;  
	Mat previousFrameDifference;  
	Mat currentFrameDifference;  
	Mat absFrameDifferece;  
	Mat previousSegmentation;  
	Mat currentSegmentation;  
	Mat segmentation;  
	namedWindow("segmentation",1);  
	createTrackbar("ãÐÖµ:","segmentation",&thresh,FORE_GROUD,NULL);  
	int numberFrame = 0;  
	Mat morphologyKernel = getStructuringElement(MORPH_RECT,Size(3,3),Point(-1,-1));  

	for(;;)  
	{  
		video >> currentBGRFrame;  
		if(!currentBGRFrame.data)  
			break;  
		numberFrame++;  
		cvtColor(currentBGRFrame,currentGaryFrame,COLOR_BGR2GRAY);  
		if( numberFrame == 1)  
		{  
			previousSecondGrayFrame = currentGaryFrame.clone();  
			imshow("video",currentBGRFrame);  
			continue;  
		}  
		else if( numberFrame == 2)  
		{  
			previousFirstGrayFrame = currentGaryFrame.clone();  
			subtract(previousFirstGrayFrame,previousSecondGrayFrame,previousFrameDifference,Mat(),CV_16SC1);  
			absFrameDifferece = abs(previousFrameDifference);  
			absFrameDifferece.convertTo(absFrameDifferece,CV_8UC1,1,0);  
			threshold(absFrameDifferece,previousSegmentation,double(thresh),double(FORE_GROUD),THRESH_BINARY);  
			imshow("video",currentBGRFrame);  
			continue;  
		}  

		else  
		{  
			subtract(currentGaryFrame,previousFirstGrayFrame,currentFrameDifference,Mat(),CV_16SC1);  
			absFrameDifferece = abs(currentFrameDifference);  
			absFrameDifferece.convertTo(absFrameDifferece,CV_8UC1,1,0);  
			threshold(absFrameDifferece,currentSegmentation,double(thresh),double(FORE_GROUD),THRESH_BINARY);  
			bitwise_and(previousSegmentation,currentSegmentation,segmentation);  
			medianBlur(segmentation,segmentation,3);  

			//morphologyEx(segmentation,segmentation,MORPH_OPEN,morphologyKernel,Point(-1,-1),1,BORDER_REPLICATE);  
			morphologyEx(segmentation,segmentation,MORPH_CLOSE,morphologyKernel,Point(-1,-1),2,BORDER_REPLICATE);  
			vector< vector<Point> > contours;  
			vector<Vec4i> hierarchy;  
			Mat tempSegmentation = segmentation.clone();  
			findContours( segmentation, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );//CV_RETR_TREE  
			vector< vector<Point> > contours_poly( contours.size() );  
			vector<Rect> boundRect;  
			boundRect.clear();  
			for(int index = 0;index < contours.size() ;index++)  
			{  
				approxPolyDP( Mat(contours[index]), contours_poly[index], 3, true );  
				Rect rect =  boundingRect( Mat(contours_poly[index]) );  
				rectangle(currentBGRFrame,rect,Scalar(0,255,255),2);  
			}  
			imshow("video",currentBGRFrame);  
			imshow("segmentation",segmentation);  
			previousFirstGrayFrame = currentGaryFrame.clone();  
			previousSegmentation = currentSegmentation.clone();  
		}  

		if(waitKey(33) == 'q')  
			break;  

	}  
	return 0;  
}  
