#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

class ballDetector
{
public:
	int pX, pY;
	ballDetector();
	void getBall(char, int*, int*, VideoCapture);

	int iLowH;
	int iHighH;

	int iLowS; 
	int iHighS;

 	int iLowV;
 	int iHighV;

 	int iLastX; 
	int iLastY;
private: 
	
};