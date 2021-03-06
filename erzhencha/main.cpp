#include<iostream>
#include <opencv2/opencv.hpp>
#include<opencv2/video/background_segm.hpp>
#include <string>
#include <iomanip>
using namespace cv;  
using namespace std;
const unsigned char FORE_GROUD = 255;  
int thresh =15;  

int main(int argc,char*argv[])  
{     
	VideoCapture video("../video.avi");   

	//判断如果video是否可以打开  
	if(!video.isOpened())  
		return -1;  

	//用于保存当前帧的图片  
	Mat currentBGRFrame;  

	//用来保存上一帧和当前帧的灰度图片  
	Mat previousGrayFrame;  
	Mat currentGaryFrame;  

	//用来保存帧差  
	Mat frameDifference;//CV_16SC1  

	//用来保存帧差的绝对值  
	Mat absFrameDifferece;  

	//用来显示前景  
	Mat segmentation;  

	//显示原视频  
	namedWindow("video",1);  

	//显示前景  
	namedWindow("segmentation",1);  
	createTrackbar("阈值:","segmentation",&thresh,FORE_GROUD,NULL);  

	//帧数  
	int numberFrame = 0;  

	//形态学处理用到的算子  
	Mat morphologyKernel = getStructuringElement(MORPH_RECT,Size(3,3),Point(-1,-1));  

	for(;;)  
	{  
		//读取当前帧  
		video >> currentBGRFrame;  

		//判断当前帧是否存在  
		if(!currentBGRFrame.data)  
			break;  

		numberFrame++;  

		if( numberFrame == 1)  
		{  
			//颜色空间的转换  
			cvtColor(currentBGRFrame,currentGaryFrame,COLOR_BGR2GRAY);  
			//保存当前帧的灰度图  
			previousGrayFrame = currentGaryFrame.clone();  
			imshow("video",currentBGRFrame);  
			continue;  
		}  
		else  
		{  
			//颜色空间的转换  
			cvtColor(currentBGRFrame,currentGaryFrame,COLOR_BGR2GRAY);  

			//src1-src2  
			subtract(currentGaryFrame,previousGrayFrame,frameDifference,Mat(),CV_16SC1);  

			//取绝对值  
			absFrameDifferece = abs(frameDifference);  

			//位深的改变  
			absFrameDifferece.convertTo(absFrameDifferece,CV_8UC1,1,0);  

			//阈值处理  
			threshold(absFrameDifferece,segmentation,double(thresh),double(FORE_GROUD),THRESH_BINARY);  

			//中值滤波  
			medianBlur(segmentation,segmentation,3);  

			//形态学处理(开闭运算)  
			//morphologyEx(segmentation,segmentation,MORPH_OPEN,morphologyKernel,Point(-1,-1),1,BORDER_REPLICATE);  
			morphologyEx(segmentation,segmentation,MORPH_CLOSE,morphologyKernel,Point(-1,-1),2,BORDER_REPLICATE);  

			//显示二值化图片  
			imshow("segmentation",segmentation);  

			//找边界  
			vector< vector<Point> > contours;  
			vector<Vec4i> hierarchy;  
			findContours( segmentation, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );//CV_RETR_TREE  
			vector< vector<Point> > contours_poly( contours.size() );  

			/*存储运动物体*/  
			vector<Rect> boundRect;  
			boundRect.clear();  

			//画出运动物体  
			//对视频中出现的运动物体，进行初次的筛选  
			for(int index = 0;index < contours.size() ;index++)  
			{  
				approxPolyDP( Mat(contours[index]), contours_poly[index], 3, true );  
				Rect rect =  boundingRect( Mat(contours_poly[index]) );  
				rectangle(currentBGRFrame,rect,Scalar(0,255,255),2);  
			}  

			//显示原视频  
			imshow("video",currentBGRFrame);  

			//保存当前帧的灰度图  
			previousGrayFrame = currentGaryFrame.clone();  
		}  

		if(waitKey(33) == 'q')  
			break;  

	}  
	return 0;  
}  
