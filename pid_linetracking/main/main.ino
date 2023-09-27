#include <motor_esp32.h>
#include <ESP32Servo.h>
#include <analogWrite.h>
#include <PS4Controller.h>
#include <Ramp.h>

enum {HOME, STARTING, RUN} state = HOME;
enum {CENTER, RIGHT, LEFT} lastline_state = CENTER;

// Motor L
#define m2a 12
#define m2b 14
#define pwm2 13
// Motor R
#define m1a 27
#define m1b 26
#define pwm1 25

// Motor parameters
#define power_percentage 60.0

Motor L_motor(m1a, m1b, pwm1);
Motor R_motor(m2a, m2b, pwm2);

class Interpolation
{
public:
  rampInt myRamp;
  int interpolationFlag = 0;
  int savedValue;

  int go(int input, int duration)
  {

    if (input != savedValue)
    { // check for new data
      interpolationFlag = 0;
    }
    savedValue = input; // bookmark the old value

    if (interpolationFlag == 0)
    {                                                  // only do it once until the flag is reset
      myRamp.go(input, duration, LINEAR, ONCEFORWARD); // start interpolation (value to go to, duration)
      interpolationFlag = 1;
    }

    int output = myRamp.update();
    return output;
  }

  int update(){
    return (int)myRamp.update();
  }
};

Interpolation interp_motor_L;
Interpolation interp_motor_R;

// sensor_pin
#define L2_pin 33
#define L1_pin 32
#define C_pin 35
#define R1_pin 34
#define R2_pin 39

// Use for compare the analog value to digital value (B = analog > ref , W = analog <= ref)
int ref_sensor[5] = {2000, 2000, 2000, 2000, 2000};

// Sensor variable
boolean sensor_bool[5] = {0, 0, 0, 0, 0};
int sensor_val[5] = {1, 1, 1, 1, 1};

// Motor&PID parameter
int baseSpeed = 150;
int maxSpeed = 255;
float pid1_parameter[3] = {25, 6, 0.0000001}; // Pi 0.0000001
float pid_forward_parameter[3] = {13.5, 1, 0.0000001}; // Pi 0.0000001

bool debug = false;

struct Remote
{
  int L_X;
  int L_Y;
  int R_X;
  int R_Y;
  boolean L1;
  boolean R1;
  int L2;
  int R2;
  boolean L3;
  boolean R3;
  boolean Option;
  boolean Share;
  boolean Up;
  boolean Down;
  boolean Left;
  boolean Right;
  boolean Square;
  boolean Cross;
  boolean Circle;
  boolean Triangle;
};

Remote controller_data;

void setup()
{
  pinMode(L2_pin, INPUT);
  pinMode(L1_pin, INPUT);
  pinMode(C_pin, INPUT);
  pinMode(R1_pin, INPUT);
  pinMode(R2_pin, INPUT);
  Serial.begin(115200);
  // PS4.begin("14:85:7f:50:7a:61");

  delay(1500);

  // trackline_Cross(pid1_parameter, baseSpeed, 1, 50, 50);
  // trackline_R(pid1_parameter, baseSpeed, 1, 50, 50);
  // tr_sensor(60);
  // trackline_R(pid_forward_parameter, 200, 1, 50, 50);
  // tr_sensor(60);
  // trackline_R(pid_forward_parameter, 200, 1, 50, 50);
  // tr_sensor(60);
  // trackline_Cross(pid1_parameter, baseSpeed, 1, 50, 50);

}
void loop()
{
  readSensor();
  trackline_pid(pid1_parameter, baseSpeed);
  delayMicroseconds(9);
}
