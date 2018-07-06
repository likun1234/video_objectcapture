#include <string>
#include <opencv2/opencv.hpp>
#include <iomanip>

using namespace cv;
using namespace std;

class FrameProcessor
{
	public:
		virtual void process(Mat &input,Mat &ouput)=0;
};  

class BGFGSegmentor:public FrameProcessor
{  
	Mat gray;  
	Mat background;  
	Mat backImage;  
	Mat foreground;  
	double learningRate;  
	int Thres;
	public:
	BGFGSegmentor():Thres(20),learningRate(0.3){}

	void process(Mat &frame,Mat &output)
	{
		cvtColor (frame,gray,CV_BGR2GRAY);
		if(background.empty ())
			gray.convertTo(background,CV_32F);  
		background.convertTo(backImage,CV_8U);  
		absdiff (backImage,gray,foreground);  
		threshold(foreground,output,Thres,255,THRESH_BINARY_INV);  
		accumulateWeighted (gray,background,learningRate,output);
	}  
};  



class VideoProcessor
{
	private:
		VideoCapture caputure;  
		VideoWriter writer;  
		string Outputfile;  
		int currentIndex;  
		int digits;  
		string extension;  
		FrameProcessor *frameprocessor;  
		void (*process)(Mat &,Mat &);  
		bool callIt;  
		string WindowNameInput;  
		string WindowNameOutput;  
		int delay;  
		long fnumber;  
		long frameToStop;  
		bool stop;  
		vector<string> images;  
	public:  
		VideoProcessor():callIt(true),delay(0),fnumber(0),stop(false),digits(0),frameToStop(-1){}


		void setFrameProcessor(void (*process)(Mat &,Mat &)){  
			frameprocessor = 0;  
			this->process = process;  
			CallProcess ();  
		}  
		bool setInput(string filename){  
			fnumber = 0;  
			caputure.release ();  
			return caputure.open (filename);  
		}  
		void displayInput(string wn){  
			WindowNameInput = wn;  
			namedWindow (WindowNameInput);  
		}  
		void displayOutput(string wn){  
			WindowNameOutput = wn;  
			namedWindow (WindowNameOutput);  
		}  
		void dontDisplay(){  
			destroyWindow (WindowNameInput);  
			destroyWindow (WindowNameOutput);  
			WindowNameInput.clear ();  
			WindowNameOutput.clear ();  
		}  

		void run(){  
			Mat frame;  
			Mat output;  
			if(!isOpened())  
				return;  
			stop = false;  
			while(!isStopped()){  
				if(!readNextFrame(frame))  
					break;  
				if(WindowNameInput.length ()!=0)  
					imshow (WindowNameInput,frame);  
				if(callIt){  
					if(process)  
						process(frame,output);  
					else if(frameprocessor)  
						frameprocessor->process (frame,output);  
				}  
				else{  
					output = frame;  
				}  
				if(Outputfile.length ()){  
					cvtColor (output,output,CV_GRAY2BGR);  
					writeNextFrame (output);  
				}  
				if(WindowNameOutput.length ()!=0)  
					imshow (WindowNameOutput,output);  
				if(delay>=0&&waitKey (delay)>=0)  
					waitKey(0);  
				if(frameToStop>=0&&getFrameNumber()==frameToStop)  
					stopIt();  
			}  
		}  
		void stopIt(){  
			stop = true;  
		}  
		bool isStopped(){  
			return stop;  
		}  
		bool isOpened(){  
			return  caputure.isOpened ()||!images.empty ();  
		}  
		void setDelay(int d){  
			delay = d;  
		}  
		bool readNextFrame(Mat &frame){  
			if(images.size ()==0)  
				return caputure.read (frame);  
			else{  
				if(itImg!=images.end()){  
					frame = imread (*itImg);  
					itImg++;  
					return frame.data?1:0;  
				}  
				else  
					return false;  
			}  
		}  

		void CallProcess(){  
			callIt = true;  
		}  
		void  dontCallProcess(){  
			callIt = false;  
		}  
		void stopAtFrameNo(long frame){  
			frameToStop = frame;  
		}  
		long getFrameNumber(){  
			long fnumber = static_cast<long>(caputure.get ((CV_CAP_PROP_POS_FRAMES)));  
			return fnumber;  
		}  

		Size getFrameSize() {  
			if (images.size()==0) {  
				int w= static_cast<int>(caputure.get(CV_CAP_PROP_FRAME_WIDTH));  
				int h= static_cast<int>(caputure.get(CV_CAP_PROP_FRAME_HEIGHT));  
				return Size(w,h);  
			}  
			else {  
				cv::Mat tmp= cv::imread(images[0]);  
				return (tmp.data)?(tmp.size()):(Size(0,0));  
			}  
		}  

		double getFrameRate(){  
			return caputure.get(CV_CAP_PROP_FPS);  
		}  
		vector<string>::const_iterator itImg;  
		bool setInput (const vector<string> &imgs){  
			fnumber = 0;  
			caputure.release ();  
			images = imgs;  
			itImg = images.begin ();  
			return true;  
		}  

		void  setFrameProcessor(FrameProcessor *frameprocessor){  
			process = 0;  
			this->frameprocessor = frameprocessor;  
			CallProcess ();  
		}  

		int getCodec(char codec[4]) {  
			if (images.size()!=0)  
				return -1;  
			union {   
				int value;  
				char code[4];  
			} returned;  
			returned.value= static_cast<int>(  
					caputure.get(CV_CAP_PROP_FOURCC));  
			codec[0]= returned.code[0];  
			codec[1]= returned.code[1];  
			codec[2]= returned.code[2];  
			codec[3]= returned.code[3];  
			return returned.value;  
		}  


		bool setOutput(const string &filename,int codec = 0,double framerate = 0.0,bool isColor = true){  
			Outputfile = filename;  
			extension.clear ();  
			if(framerate ==0.0){  
				framerate = getFrameRate ();  
			}  
			char c[4];  
			if(codec==0){  
				codec = getCodec(c);  
			}  
			return writer.open(Outputfile,  
					codec,  
					framerate,  
					getFrameSize(),  
					isColor);  
		}  

		bool setOutput (const string &filename,  
				const string &ext,  
				int numberOfDigits=3,  
				int startIndex=0 ){  
			if(numberOfDigits<0)  
				return false;  
			Outputfile = filename;  
			extension = ext;  
			digits = numberOfDigits;  
			currentIndex = startIndex;  
			return true;  
		}  

		void writeNextFrame(Mat &frame){  
			if(extension.length ()){  
				stringstream ss;  
				ss<<Outputfile<<setfill('0')<<setw(digits)<<currentIndex++<<extension;  
				imwrite (ss.str (),frame);  
			}  
			else{  
				writer.write (frame);  
			}  
		}
};  

int main(int argc, char *argv[])  
{  
	VideoProcessor processor;  
	BGFGSegmentor segmentor;  
	processor.setInput ("14.avi");  
	processor.displayInput ("Current Frame");  
	processor.displayOutput ("Output Frame");  
	processor.setDelay (1000./processor.getFrameRate ());  
	processor.setFrameProcessor (&segmentor);  
	processor.setOutput ("walkout",".avi");  
	processor.setOutput ("walkout",".jpg");  
	processor.run ();  
	return 0;  
}  
