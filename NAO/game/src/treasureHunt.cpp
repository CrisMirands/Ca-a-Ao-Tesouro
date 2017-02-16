#include "treasureHunt.hpp"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <string>
#include <Conversions.hpp>
#include <cmath>
#include <ledAuxiliarFunctions.hpp>
#include <motionNAOqiFunction.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

//#include "UDP_client.hpp"
//#include "UdpInterface.hpp"


TreasureRobot::TreasureRobot()
{

         //Thresholds de segmentacao
        iLowH = 0;
        iHighH = 176;

        iLowS = 180 ;
        iHighS = 255;

        iLowV = 100;
        iHighV = 220;

        ballLost = 0;

        AL::ALVideoDeviceProxy camProxy("127.0.0.1");

        camera = camProxy.subscribe("Camera", AL::kVGA, AL::kBGRColorSpace, 30);
        camProxy.setParam(AL::kCameraSelectID, 1);
        std::cout << camera << std::endl;

}

TreasureRobot::~TreasureRobot()
{
    camProxy.unsubscribe(camera);
}


char TreasureRobot::manageConnection(int port)
{
    UdpInterface UDP(port);
    char comm;
    int size = sizeof(char);
    struct sockaddr_in target;
    socklen_t addrlen = sizeof(target);
    int recvlen;
    struct sockaddr_in targetReceive;

    memset((char *)&targetReceive, 0, sizeof(targetReceive));
    targetReceive.sin_family = AF_INET;
    targetReceive.sin_addr.s_addr = inet_addr("10.0.255.255");
    targetReceive.sin_port = htons(8001);


    recvlen = UDP.receive(&comm, size, (struct sockaddr *)&target, &addrlen);

    readData(comm);

    return comm;



}

void TreasureRobot::readData(char comm)
{
    LEDAux::changeColor(0, 0, 0);
    //UDP.recv(&comm, size);
    std::cout <<  comm << std::endl;
    if(comm == 'q')
    {
        LEDAux::changeColor(Conversions::RED, Conversions::RED, Conversions::RED);
        std::cout << "Quente" << std::endl;

    }
    if(comm == 'f')
    {
        LEDAux::changeColor(Conversions::RED, Conversions::BLUE, Conversions::BLUE);
        std::cout << "Frio" << std::endl;
    }
    if(comm == 'a')
    {
        LEDAux::changeColor(Conversions::BLUE, Conversions::BLUE, Conversions::BLUE);
    }
    else
    {
        LEDAux::changeColor(Conversions::RED, Conversions::GREEN, Conversions::GREEN);
        std::cout << "Nothing" << std::endl;
    }

}

ball TreasureRobot::GetBall()
{

    ball Ball;


    AL::ALValue image = camProxy.getImageRemote(camera);

    cv::Mat frame(cv::Size(640,480), CV_8UC3);

    frame.data = (uchar*) image[6].GetBinary();
     cv::imwrite("frame2.jpg", frame);

    Ball = IsBall(frame);


    camProxy.releaseImage(camera);
    return Ball;

}

ball TreasureRobot::IsBall(cv::Mat cap)
{
        ball Ball;
        cv::Mat imgTmp;
        imgTmp = cap;

        cv::Mat imgLines = cv::Mat::zeros( imgTmp.size(), CV_8UC3 );

        cv::Mat imgHSV;
        cv::cvtColor(cap, imgHSV, cv::COLOR_BGR2HSV);


        cv::Mat imgThresholded;


       cv::inRange(imgHSV, cv::Scalar(iLowH, iLowS, iLowV), cv::Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

        cv::imwrite("frame.jpg", imgThresholded);

       //morphological opening
       erode(imgThresholded, imgThresholded, getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)) );
       dilate( imgThresholded, imgThresholded, getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)) );

       //morphological closing
       dilate( imgThresholded, imgThresholded, getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)) );
       erode(imgThresholded, imgThresholded, getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)) );

       // cv::imwrite("frame1.jpg", imgThresholded);


       //Momentos da imagem
       cv::Moments oMoments = cv::moments(imgThresholded);

       double dM01 = oMoments.m01;
       double dM10 = oMoments.m10;
       double dArea = oMoments.m00;

       //std::cout << dArea << std::endl;
       Ball.isBall = false;
       Ball.x = -1;
       Ball.y = -1;
       // if the area <= 10000, I consider that the there are no object in the image and it's because of the noise, the area is not zero
       if (dArea > 50000)
       {
           //std::cout << "Entrou" << std::endl;
           Ball.isBall = true;
       //calculate the position of the ball
            int posX = dM10 / dArea;
            int posY = dM01 / dArea;

            Ball.x = posX;
            Ball.y = posY;

        }



       return Ball;

}

ball TreasureRobot::moveFrame()
{
    ball Ball;
    Ball = GetBall();
    //Ball.x = Ball.x - 320;
    Ball.y = -(Ball.y - 240);

    return Ball;
}

//state machine
int TreasureRobot::initial()
{
    std::cout << "INITIAL STATE" << std::endl;
    MotionNAOqi::say("Starting");
    MotionNAOqi::Stand();

    return LOOKING;
}

int TreasureRobot::looking()
{
    std::cout << "LOOKING STATE" << std::endl;
    ball Ball;
    MotionNAOqi::move(0.0f , 0.0f, 0.0f);
    Ball = GetBall();

    if(Ball.isBall == true)
    {
         std::cout << "ball found" << std::endl;
        ballLost = 0;
        return BALL_FOUND;
    }
    else if  (Ball.isBall == false && ballLost > 5)
    {
         std::cout << "go to message" << std::endl;
         ballLost = 0;
        return WAITING_MESSAGE;
    }
    if(Ball.isBall == false)
    {
        std::cout << "ball lost" << std::endl;
        ballLost++;
        return LOOKING;
    }
}

int TreasureRobot::ballFound()
{
     std::cout << "BALL FOUND STATE" << std::endl;
     ball Ball;
     Ball = moveFrame();
     if(Ball.isBall == true)
     {
         ballLost = 0;
         if(Ball.x < 40)
         {
             MotionNAOqi::move(0.0f , 0.0f, 0.0f);
             return GOAL;

         }
         else
         {
             float x = Ball.x/640;
             float y = -Ball.y/240;
             float theta = atan(x/y)/100;
             MotionNAOqi::move(0.5f , 0.0f, theta);
             sleep(5);

             return LOOKING;
         }

     }
     else if(Ball.isBall == false && ballLost > 5)
     {
         ballLost = 0;
         return LOOKING;
     }
     else if(Ball.isBall == false)
     {
         std::cout << "ball lost" << std::endl;
         ballLost++;
         return BALL_FOUND;
     }

}

int TreasureRobot::waitMessage()
{
    std::cout << "WAITING MESSAGE STATE" << std::endl;
    MotionNAOqi::say("Quente ou Frio?");
    char comm;
    comm = manageConnection(8001);
    if(comm == 'f')
        return MESSAGE_F;
    else if(comm == 'q')
        return MESSAGE_Q;
    else if(comm == 'a')
        return GOAL;
    else
        return WAITING_MESSAGE;
}

int TreasureRobot::quente()
{
    std::cout << "HOT STATE" << std::endl;
    MotionNAOqi::move2(0.3f, 0.0f, 0.0f);
    return LOOKING;
}

int TreasureRobot::frio()
{
    std::cout << "COLD STATE" << std::endl;
    MotionNAOqi::move2(0.0f, 0.0f, 1.5f);
    return LOOKING;
}

int TreasureRobot::goal()
{
    MotionNAOqi::say("Achei!");
    LEDAux::changeColor(Conversions::BLUE, Conversions::BLUE, Conversions::BLUE);
    sleep(1);
    LEDAux::changeColor(Conversions::GREEN, Conversions::GREEN, Conversions::GREEN);
    sleep(1);
    LEDAux::changeColor(Conversions::RED, Conversions::RED, Conversions::RED);
    sleep(1);
    LEDAux::changeColor(Conversions::BLUE, Conversions::BLUE, Conversions::BLUE);
    sleep(1);
    LEDAux::changeColor(Conversions::GREEN, Conversions::GREEN, Conversions::GREEN);
    sleep(1);
    LEDAux::changeColor(Conversions::RED, Conversions::RED, Conversions::RED);
    sleep(1);
    LEDAux::changeColor(Conversions::BLUE, Conversions::BLUE, Conversions::BLUE);
    sleep(1);
    LEDAux::changeColor(Conversions::GREEN, Conversions::GREEN, Conversions::GREEN);
    sleep(1);
    LEDAux::changeColor(Conversions::RED, Conversions::RED, Conversions::RED);
    sleep(1);
    return GOAL;
}



/*
void TransmitterRobot::locationChallenge()
{
    int readSize;
    int x, y, xOct, yOct, quadrant = 0;
    readSize = fromCommsTester.readData(&packet.payload.location, sizeof(SPLNoWifiLocationPayload));

    x = packet.payload.location.x;
    y = packet.payload.location.y;

    std::cout << "Transmitter robot: received location, x=" << x << ", y=" << y << std::endl;

    Conversions::MoveCenterToCorner(&x, &y);
    Conversions::ConvertToQuadrant(&x, &y, &quadrant);
    xOct = Conversions::decimal_octal(x);
    yOct = Conversions::decimal_octal(y);
    std::vector<int> vec = Conversions::num2vec(xOct, yOct, quadrant);
    for(int i =0; i < 9 ; i++)
    {
        std::cout << vec[i] << std::endl;
    }
    Transmit(vec);
}

void TransmitterRobot::Transmit(std::vector<int> vec)
{

    for(int i = 0; i<9; i++)
    {
        Conversions::transmitLED(vec[i]);
        boost::this_thread::sleep(boost::posix_time::milliseconds(2000));
        Conversions::turnOFF();
        boost::this_thread::sleep(boost::posix_time::milliseconds(1000));

    }
}

bool TransmitterRobot::ButtonPressed()
{
    ButtonData buttons;
    buttons = buttonBoard.load();
    if(buttons.button.pop(1))
    {
        //Chest Button pressed once
        buttonBoard.save(buttons);
        return true;
    }
    buttonBoard.save(buttons);
    return false;
}*/


