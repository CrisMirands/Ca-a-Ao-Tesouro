#pragma once
#include <iostream>
#include <alproxies/alrobotpostureproxy.h>
#include <alproxies/almotionproxy.h>
#include <alproxies/altexttospeechproxy.h>


namespace MotionNAOqi
{


   void Stand();

   void moveArm(float x, float y);

   void move(float x, float y, float theta);

   void move2(float x, float y, float theta);

   void say(std::string text);



}
