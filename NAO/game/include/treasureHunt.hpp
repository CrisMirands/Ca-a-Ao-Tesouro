#pragma once

#include <unBoard.hpp>
#include "UdpInterface.hpp"
//#include <ButtonData.hpp>

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <vector>

// Aldebaran includes.
#include <alproxies/alvideodeviceproxy.h>
#include <alvision/alimage.h>
#include <alvision/alvisiondefinitions.h>
#include <alerror/alerror.h>

struct ball
{
    int x, y;
    bool isBall;
};

enum States
{
    INITIAL,
    LOOKING,
    BALL_FOUND,
    WAITING_MESSAGE,
    MESSAGE_Q,
    MESSAGE_F,
    GOAL
};

class TreasureRobot
{

    public:
        TreasureRobot();
        ~TreasureRobot();


        //void locationChallenge();

        int initial();
        int looking();
        int ballFound();
        int waitMessage();
        int quente();
        int frio();
        int goal();



        int iLowH, iHighH,iLowS, iHighS, iLowV, iHighV;

    private:
        //SPLNoWifiPacket packet;
        //TcpConnection fromCommsTester;
        //unBoard<ButtonData> buttonBoard;
        //void Transmit(std::vector<int>);
        AL::ALVideoDeviceProxy camProxy;
        std::string camera;
        ball IsBall(cv::Mat);
        char manageConnection(int);
        void readData(char);
        int ballLost;

        ball GetBall();
        ball moveFrame();

};
