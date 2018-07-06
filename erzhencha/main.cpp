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

	//�ж����video�Ƿ���Դ�  
	if(!video.isOpened())  
		return -1;  

	//���ڱ��浱ǰ֡��ͼƬ  
	Mat currentBGRFrame;  

	//����������һ֡�͵�ǰ֡�ĻҶ�ͼƬ  
	Mat previousGrayFrame;  
	Mat currentGaryFrame;  

	//��������֡��  
	Mat frameDifference;//CV_16SC1  

	//��������֡��ľ���ֵ  
	Mat absFrameDifferece;  

	//������ʾǰ��  
	Mat segmentation;  

	//��ʾԭ��Ƶ  
	namedWindow("video",1);  

	//��ʾǰ��  
	namedWindow("segmentation",1);  
	createTrackbar("��ֵ:","segmentation",&thresh,FORE_GROUD,NULL);  

	//֡��  
	int numberFrame = 0;  

	//��̬ѧ�����õ�������  
	Mat morphologyKernel = getStructuringElement(MORPH_RECT,Size(3,3),Point(-1,-1));  

	for(;;)  
	{  
		//��ȡ��ǰ֡  
		video >> currentBGRFrame;  

		//�жϵ�ǰ֡�Ƿ����  
		if(!currentBGRFrame.data)  
			break;  

		numberFrame++;  

		if( numberFrame == 1)  
		{  
			//��ɫ�ռ��ת��  
			cvtColor(currentBGRFrame,currentGaryFrame,COLOR_BGR2GRAY);  
			//���浱ǰ֡�ĻҶ�ͼ  
			previousGrayFrame = currentGaryFrame.clone();  
			imshow("video",currentBGRFrame);  
			continue;  
		}  
		else  
		{  
			//��ɫ�ռ��ת��  
			cvtColor(currentBGRFrame,currentGaryFrame,COLOR_BGR2GRAY);  

			//src1-src2  
			subtract(currentGaryFrame,previousGrayFrame,frameDifference,Mat(),CV_16SC1);  

			//ȡ����ֵ  
			absFrameDifferece = abs(frameDifference);  

			//λ��ĸı�  
			absFrameDifferece.convertTo(absFrameDifferece,CV_8UC1,1,0);  

			//��ֵ����  
			threshold(absFrameDifferece,segmentation,double(thresh),double(FORE_GROUD),THRESH_BINARY);  

			//��ֵ�˲�  
			medianBlur(segmentation,segmentation,3);  

			//��̬ѧ����(��������)  
			//morphologyEx(segmentation,segmentation,MORPH_OPEN,morphologyKernel,Point(-1,-1),1,BORDER_REPLICATE);  
			morphologyEx(segmentation,segmentation,MORPH_CLOSE,morphologyKernel,Point(-1,-1),2,BORDER_REPLICATE);  

			//��ʾ��ֵ��ͼƬ  
			imshow("segmentation",segmentation);  

			//�ұ߽�  
			vector< vector<Point> > contours;  
			vector<Vec4i> hierarchy;  
			findContours( segmentation, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );//CV_RETR_TREE  
			vector< vector<Point> > contours_poly( contours.size() );  

			/*�洢�˶�����*/  
			vector<Rect> boundRect;  
			boundRect.clear();  

			//�����˶�����  
			//����Ƶ�г��ֵ��˶����壬���г��ε�ɸѡ  
			for(int index = 0;index < contours.size() ;index++)  
			{  
				approxPolyDP( Mat(contours[index]), contours_poly[index], 3, true );  
				Rect rect =  boundingRect( Mat(contours_poly[index]) );  
				rectangle(currentBGRFrame,rect,Scalar(0,255,255),2);  
			}  

			//��ʾԭ��Ƶ  
			imshow("video",currentBGRFrame);  

			//���浱ǰ֡�ĻҶ�ͼ  
			previousGrayFrame = currentGaryFrame.clone();  
		}  

		if(waitKey(33) == 'q')  
			break;  

	}  
	return 0;  
}  
