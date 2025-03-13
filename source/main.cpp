#include <stdio.h>
#include <iostream>
#include <nds.h>
#include <nf_lib.h>
#include <cmath>
#include <filesystem.h>
#include "initvis.h"

#define MAP_WIDTH 3092
#define MAP_HEIGHT 600

using namespace std;



double cosDegrees(double degrees) {
  double radians = degrees * (M_PI / 180.0); // Convert degrees to radians
  return cos(radians);
}

// Function to calculate sine using degrees
double sinDegrees(double degrees) {
  double radians = degrees * (M_PI / 180.0); // Convert degrees to radians
  return sin(radians);
}

class Sonic {
  public:
    float XSpeed, TopXSpeed, MaxXSpeed, XAcceleration, SlopeDeceleration, YSpeed, MaxYSpeed,
        YAcceleration, LandingConversion, Landed, Action, Angle, SmoothAngle, Layer,
        Direction, Hurt, LandingSpeed, FloatX, FloatY;
    float JumpStrength, JumpReleaseStrength, LookUpShiftAmount, CrouchDownShiftAmount, SkidMinimumAmount,
        SkidRate, SpindashRevCharge, SpindashRevLose, SpindashMinSpeed, SpindashMaxSpeed, RollingDeceleration,
        RollingBraking, RollingSlopeUpFactor, RollingSlopeDownFactor, RollingConversion, Peel_outRevCharge,
        Peel_outMinSpeed, Peel_outMaxSpeed, Peel_outRevLose;
    int Animation, AnimationFrame, AnimationWait, AnimationRepeat, AnimationLoop, AnimationEnd, AnimationStart,
        AnimationLock, AnimationTarget, AnimationLength, Collision;
    bool Ground = 0, AllowCommonInput = 1, AllowX = 1, AllowY = 1, AllowLanding = 1, Attacking = 0,
         JumpVariable = 0;

  public:
    Sonic() {
      XSpeed = 0;
      TopXSpeed = 6;
      MaxXSpeed = 15;
      XAcceleration = 0.046875;
      SlopeDeceleration = 0.203125;
      YSpeed = 0;
      MaxYSpeed = 15;
      YAcceleration = 0.21875;
      Angle = 0;
      LandingConversion = 0.84;
      JumpStrength = -6.52;
      JumpReleaseStrength = -3.78125;
      LookUpShiftAmount = -170;
      CrouchDownShiftAmount = 170;
      SkidMinimumAmount = 1.4;
      SkidRate = 0.5;
      SpindashRevCharge = 1;
      SpindashRevLose = 0.01;
      SpindashMinSpeed = 6;
      SpindashMaxSpeed = 12;
      RollingDeceleration = 0.0234;
      RollingBraking = 0.12;
      RollingSlopeUpFactor = 0.035;
      RollingSlopeDownFactor = 0.25;
      RollingConversion = 0.7;
      Peel_outRevCharge = 1;
      Peel_outRevLose = 0.01;
      Peel_outMinSpeed = 8;
      Peel_outMaxSpeed = 12;
      FloatX = 0;
      FloatY = 0;
      Action = 0;
      Direction = 1;
      Animation = 0;
      AnimationFrame = 0;
      AnimationWait = 1;
      AnimationRepeat = 1;
      AnimationLoop = 0;
      AnimationEnd = 0;
      AnimationStart = 0;
      AnimationLock = 0;
      AnimationTarget = 0;
      AnimationLength = 1;
      Collision = 0;
    }

    void sonicUpdate(uint16_t keys_held, uint16_t keys_down)
    {
      if (AllowX){
        FloatX = FloatX + cos(Angle) * XSpeed;
        FloatY = FloatY - sin(Angle) * YSpeed;
      }

      if (AllowY && !Ground){
        FloatY = FloatY + YSpeed;
      }

      if (!Ground) YSpeed = YSpeed + YAcceleration;
      if (Ground) YSpeed = 0;

      // Decel and accel X
      // Left Decel
      if (AllowCommonInput && Ground && (XSpeed >= 0) && (!(keys_held & KEY_RIGHT))) {
        XSpeed -= XAcceleration;  // Decrease XSpeed
        if (XSpeed < 0.0f) {      // Ensure XSpeed doesn't go below 0
          XSpeed = 0.0f;
        }
      }

      // Right Decel
      if (AllowCommonInput && Ground && (XSpeed <= 0) && (!(keys_held & KEY_LEFT))) {
        XSpeed += XAcceleration;  // Increase XSpeed (decelerate left)
        if (XSpeed > 0.0f) {      // Ensure XSpeed doesn't go above 0
          XSpeed = 0.0f;
        }
      }

      // Right Accel
      if (AllowCommonInput && (keys_held & KEY_RIGHT) && (XSpeed < TopXSpeed) && ((Action != 2) && Action != 3 && Action != 4
      && Action != 5 && Action != 6 && Action != 10)) 
      {
        XSpeed += XAcceleration;
      }

      // Left Accel
      if (AllowCommonInput && (keys_held & KEY_LEFT) && (XSpeed > -TopXSpeed) && ((Action != 2) && Action != 3 && Action != 4
      && Action != 5 && Action != 6 && Action != 10)) 
      {
        XSpeed -= XAcceleration;
      }

      // Right Accel Air
      if (AllowCommonInput && (keys_held & KEY_RIGHT) && (XSpeed < 0) && (!Ground))
      {
        XSpeed += XAcceleration;
      }

      // Left Accel Air
      if (AllowCommonInput && (keys_held & KEY_LEFT) && (XSpeed > 0) && (!Ground))
      {
        XSpeed -= XAcceleration;
      }

      // Common Decel
      if (Ground && (Action != 6) && (abs(XSpeed) < TopXSpeed) && (Angle > 35) && (Angle < 325))
      {
         XSpeed = XSpeed - sin(Angle) * SlopeDeceleration * 0.38;
      }

      if (Ground && (Action != 6) && (abs(XSpeed) >= TopXSpeed) && (Angle > 35) && (Angle < 325))
      {
        XSpeed = XSpeed - sin(Angle) * SlopeDeceleration;
      }



      // Actions: Jump
      // Jump Trigger
      if (Ground && (keys_down & KEY_A))
      {
        YSpeed = (cos(Angle) * JumpStrength) - (sin(Angle) * XSpeed);
        XSpeed = (sin(Angle) * JumpStrength) + (cos(Angle) * XSpeed);
        Ground = 0;
        Angle = 0;
        Action = 1;
        JumpVariable = 1;
      }

      if (Action == 1){
        // Jump Limit
          if ((!(keys_held & KEY_A)) && (YSpeed < JumpReleaseStrength) && (JumpVariable == 1))
          {
            YSpeed = JumpReleaseStrength;
          }

          // Action Reset 1: Jump
          if (Ground){
            Action = 0;
            AllowCommonInput = 1;
          } 
        }
      // Roll Trigger
      if (Ground && (keys_down & KEY_DOWN) && (abs(XSpeed) >= 0.2))
      {
          Action = 6;
          AllowCommonInput = 0;
      }

      if (Ground && Landed && (keys_held & KEY_DOWN) && (abs(XSpeed) >= 0.2))
      {
          Action = 6;
          AllowCommonInput = 0;
      }

      // Roll Action

      if (Action == 6){
          // Left Decel
          if (XSpeed < 0.0 && (keys_held & KEY_RIGHT))
          {
              XSpeed = (XSpeed + RollingBraking < 0) ? XSpeed + RollingBraking : 0;
          }

          if (XSpeed < 0.0 && (!(keys_held & KEY_RIGHT)))
          {
              XSpeed = (XSpeed + RollingBraking < 0) ? XSpeed + RollingDeceleration : 0;
          }

          // Right Decel
          if (XSpeed > 0.0 && (keys_held & KEY_LEFT))
          {
              XSpeed -= RollingBraking;
              if (XSpeed < 0.0)
                  XSpeed = 0.0;
          }

          if (XSpeed > 0.0 && (!(keys_held & KEY_LEFT)))
          {
              XSpeed -= RollingDeceleration;
              if (XSpeed < 0.0)
                  XSpeed = 0.0;
          }

          // Angle-based Deceleration
          // Slopes Up
          if (Ground && XSpeed > 0.0 && Angle < 180) XSpeed = XSpeed - sin(Angle) * RollingSlopeUpFactor;

          if (Ground && XSpeed < 0.0 && Angle > 180) XSpeed = XSpeed - sin(Angle) * RollingSlopeUpFactor;

          // Slopes Down
          if (Ground && XSpeed < 0.0 && Angle < 180) XSpeed = XSpeed - sin(Angle) * RollingSlopeDownFactor;

          if (Ground && XSpeed > 0.0 && Angle > 180) XSpeed = XSpeed - sin(Angle) * RollingSlopeDownFactor;

          // Roll: Jump out of roll
          if (!Ground){
            AllowCommonInput = 1;
            JumpVariable = 0;
            Action = 1;
            }

          if (Ground && (keys_down & KEY_A)){
            YSpeed = (cos(Angle) * JumpStrength) - (sin(Angle) * XSpeed);
            XSpeed = (sin(Angle) * JumpStrength) + (cos(Angle) * XSpeed);
            Ground = 0;
            Angle = 0;
            AllowCommonInput = 0;
            JumpVariable = 1;
            Action = 1;
            }

          // End roll if stopped
          if (XSpeed == 0.0){
            AllowCommonInput = 1;
            Action = 0;
            }
          }

        switch (NF_GetTile(0, FloatX, FloatY))
        {
            case 0: // Check additional conditions inside the case
                if (YSpeed > 0.0 && !Ground && FloatY > 0.0) {
                    Collision = 1;
                    Ground = 1;
                    Landed = 1;
                }
            break;

            case 2:
                Collision = 0;
            Ground = 0;
            break;

            default:
                break;
        }



    }

    void sonicClamp(){
      if (XSpeed > MaxXSpeed) XSpeed = MaxXSpeed;
      if (XSpeed < -MaxXSpeed) XSpeed = -MaxXSpeed;
      if (YSpeed > MaxYSpeed) YSpeed = MaxYSpeed;
      if (FloatX < 0) FloatX = 0;
      if (FloatX > MAP_WIDTH) FloatX = MAP_WIDTH;
      Landed = 0;
      }

    void sonicSpriteMove(int camX, int camY){
        // Magic numbers (-18, -12) are for sprite positioning, TOFIX: Scroll issue
      NF_MoveSprite(0, 1, FloatX - camX - 18, FloatY - camY - 12);
      if (Direction == 1) {
        NF_HflipSprite(0, 1, false);
      } else {
        NF_HflipSprite(0, 1, true);
      }
      }

  void sonicSpriteAnim() {
    // General Sprite Frame Update
    NF_SpriteFrame(0, 1, AnimationFrame);

    // Increment the repeat counter
    AnimationRepeat++;

    // Advance animation frame after waiting
    if (AnimationRepeat >= AnimationWait) {
        AnimationFrame++;
        AnimationRepeat = 0;
    }

    // Loop animation
    if (AnimationFrame > AnimationEnd) {
        AnimationFrame = AnimationLoop;
    }

    // Determine Animation State
    if (Ground && (Action == 0)) {
        if (abs(XSpeed) == 0 && Animation != 0) {
            Animation = 0;
            AnimationLock = 0;
        } else if (abs(XSpeed) > 0 && abs(XSpeed) < 5 && Animation != 1) {
            Animation = 1;
            AnimationLock = 0;
        } else if (abs(XSpeed) >= 5 && Animation != 2) {
            Animation = 2;
            AnimationLock = 0;
        }
    }

    if ((Action == 1 || Action == 6) && Animation != 3) {
        Animation = 3;
        AnimationLock = 0;
    }

    // Adjust Walking Animation Speed
    if (Animation == 1 && abs(XSpeed) > 0) {
        AnimationWait = 20 / abs(XSpeed);
        if (AnimationWait < 5) {  // Ensure a minimum wait time
            AnimationWait = 5;
        }
    }

    // Set animation parameters when AnimationLock is off
    if (AnimationLock == 0) {
        if (Animation == 0) {  // Standing
            AnimationFrame = 0;
            AnimationTarget = 23;
            AnimationLength = 1;
            AnimationWait = 20;
        } else if (Animation == 1) {  // Walking
            AnimationFrame = 25;
            AnimationTarget = 25;
            AnimationLength = 5;
            AnimationWait = 7;
        } else if (Animation == 2) {  // Running
            AnimationFrame = 31;
            AnimationTarget = 31;
            AnimationLength = 3;
            AnimationWait = 3;
        } else if (Animation == 3) {  // Jumping or Rolling / Action
            AnimationFrame = 35;
            AnimationTarget = 35;
            AnimationLength = 7;
            AnimationWait = 7;
        }

        // Set Animation End and Lock Parameters
        AnimationEnd = AnimationTarget + AnimationLength;
        AnimationLoop = AnimationTarget;
        AnimationLock = 1;
    }
}




    void sonicDirection(uint16_t keys_held){
      if ((Action != 2) && (Action != 5) && (Action != 6) && (Action != 8) && (Action != 43) && (Action != 10)
      && (Action != 44) && (Action != 45) && (keys_held & KEY_LEFT)) Direction = -1;

      if ((Action != 2) && (Action != 5) && (Action != 6) && (Action != 8) && (Action != 43) && (Action != 10)
      && (Action != 44) && (Action != 45) && (keys_held & KEY_RIGHT)) Direction = 1;
      }

    void sonicSensor(){
      NF_MoveSprite(0, 2, FloatX-2, FloatY+8);
      }
};

int main(int argc, char **argv)
{
    consoleDemoInit(); // Initialize console



    Sonic sonic;

    initVis();

    initSonic();

    initSensor();

    initMap();

    int camX = 0;
    int camY = 0;

    bool running = true;



    while (running)
    {
      scanKeys();
      uint16_t keys_held = keysHeld();
      uint16_t keys_down = keysDown();

      // Calculate the target camera position
      // TOFIX: Scroll issue
      int targetCamX = sonic.FloatX - 128;
      int targetCamY = sonic.FloatY - 96 + 32 // +32 is for sprite positioning;

      // Prevent the camera from moving out of bounds
      if (targetCamX < 0) targetCamX = 0;
      if (targetCamY < 0) targetCamY = 0;
      if (targetCamX > (MAP_WIDTH - 256)) targetCamX = MAP_WIDTH - 256;
      if (targetCamY > (MAP_HEIGHT - 192)) targetCamY = MAP_HEIGHT - 192;

      // Update the camera position
      camX = targetCamX;
      camY = targetCamY;

      sonic.sonicUpdate(keys_held, keys_down);
      sonic.sonicClamp();
      sonic.sonicDirection(keys_held);
      sonic.sonicSpriteMove(camX, camY);
      sonic.sonicSpriteAnim();
      sonic.sonicSensor();


      // Apply scrolling to the background layer
      // TOFIX: Scroll Issue
      NF_ScrollBg(0, 2, camX, camY-40); // -40 magic number changes as Sonic gets away from the middle of the stage(?)


      consoleClear();
      cout << "XSpeed: " << sonic.XSpeed << ", YSpeed: " << sonic.YSpeed << endl;
      cout << "Float X: " << sonic.FloatX << ", FloatY: " << sonic.FloatY << endl;
      cout << "Direction :" << sonic.Direction << endl;
      cout << "Action: " << sonic.Action << endl;
      cout << "Angle: " << sonic.Angle << endl;
      cout << endl;;
      cout << "Animation Frame: " << sonic.AnimationFrame << endl;
      cout << "Animation Lock: " << sonic.AnimationLock << endl;
      cout << "Animation Wait: " << sonic.AnimationWait << endl;
      cout << "Animation End: " << sonic.AnimationEnd << endl;
      cout << "Animation: " << sonic.Animation << endl;
      cout << "Animation Repeat: " << sonic.AnimationRepeat << endl;
      cout << endl;
      cout << "Collision: " << sonic.Collision << endl;
      cout << "camY: " << camY << " FloatY: " << sonic.FloatY << "bgY: " << camY-32 << endl;


      if (running && keys_down & KEY_START) running = false;

      NF_SpriteOamSet(0);

      swiWaitForVBlank();

      oamUpdate(&oamMain);
    }

    while (!running){
      scanKeys();
      uint16_t keys_held = keysHeld();
      uint16_t keys_down = keysDown();
      if (!running && (keys_down & KEY_START)) running = true;

      NF_SpriteOamSet(0);

      swiWaitForVBlank();

      oamUpdate(&oamMain);
      }

    return 0;
}