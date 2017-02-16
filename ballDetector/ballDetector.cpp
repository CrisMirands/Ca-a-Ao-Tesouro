// g++ ballDetector.cpp -o ballDetector `pkg-config --cflags --libs opencv`

#include "ballDetector.hpp"


ballDetector::ballDetector()
{
	pX = 0;
	pY = 0;

	 //Thresholds de segmentacao
    iLowH = 0;
	iHighH = 179;

	iLowS = 200; 
	iHighS = 255;

 	iLowV = 67;
 	iHighV = 199;

 	iLastX = -1; 
	iLastY = -1;
}

void ballDetector::getBall(char source, int *pX, int *pY, VideoCapture cap)
{
	if(source == 'c')
	{
		Mat image;
		image = imread("frame2.jpg", CV_LOAD_IMAGE_COLOR);
		Mat imgTmp;// = image;
		cap.read(imgTmp); 

		Mat imgLines = Mat::zeros( imgTmp.size(), CV_8UC3 );;


	    Mat imgOriginal;

	    bool bSuccess = cap.read(imgOriginal); 


	    if (!bSuccess) 
	    {
	         std::cout << "Erro" << std::endl;
	         return;
	    }

	    //imgOriginal = image;

	    Mat imgHSV;

	    cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV);
	 
	    Mat imgThresholded;

	   inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
	      
	   //morphological opening 
	   erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	   dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

	   //morphological closing 
	   dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
	   erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );


	   //Momentos da imagem
	   Moments oMoments = moments(imgThresholded);

	   double dM01 = oMoments.m01;
	   double dM10 = oMoments.m10;
	   double dArea = oMoments.m00;
	   std::cout << dArea << std::endl;
	   // if the area <= 10000, I consider that the there are no object in the image and it's because of the noise, the area is not zero 
	   if (dArea > 500000)
	   {
	   //calculate the position of the ball
	   		int posX = dM10 / dArea;
	   		int posY = dM01 / dArea;   

	   		*pX = posX;
	   		*pY = posY;     
	        
	   		if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0)
	   		{
	    		//Draw a red line from the previous point to the current point
	    		line(imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(0,0,255), 2);
	   		}

	    	iLastX = posX;
	   		iLastY = posY;
	  	}

	   	imshow("Thresholded Image", imgThresholded); //show the thresholded image

	   	imgOriginal = imgOriginal + imgLines;
	  	imshow("Original", imgOriginal); //show the original image
		


	   /* namedWindow("Ball",1);
	    while(cap.isOpened())
	    {
	        Mat frame;
	        cap >> frame;

	        imshow( "Ball", frame );
	        if(waitKey(30) >= 0) break;
	    }   */                
	}
}


int main(int argc, char const *argv[])
{
	ballDetector Ball;
	VideoCapture cap(0); 
    if(!cap.isOpened())  
        return 0;

 	namedWindow("Control", CV_WINDOW_AUTOSIZE); 

 	//Calibrador
 	cvCreateTrackbar("LowH", "Control", &Ball.iLowH, 179); 
 	cvCreateTrackbar("HighH", "Control", &Ball.iHighH, 179);

  	cvCreateTrackbar("LowS", "Control", &Ball.iLowS, 255); 
 	cvCreateTrackbar("HighS", "Control", &Ball.iHighS, 255);

  	cvCreateTrackbar("LowV", "Control", &Ball.iLowV, 255); 
 	cvCreateTrackbar("HighV", "Control", &Ball.iHighV, 255);

	while(true)
	{
		Ball.getBall('c', &Ball.pX, &Ball.pY, cap);
		std::cout << Ball.pX << " " << Ball.pY << std::endl;
		if (waitKey(30) == 27) //wesc
	    {
	        std::cout << "esc key is pressed by user" << std::endl;
	        break; 
	    }
	}
	return 0;
}
