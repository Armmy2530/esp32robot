#include <motor_esp32.h>
#include <ESP32Servo.h>
#include <analogWrite.h>
#include <PS4Controller.h>

// Motor L
#define m2a 12
#define m2b 14
#define pwm2 13
// Motor R
#define m1a 27
#define m1b 26
#define pwm1 25

// Motor parameters
#define max_speed_percentage 70

Motor L_motor(m1a, m1b, pwm1);
Motor R_motor(m2a, m2b, pwm2);

// sensor_pin
#define L2_pin 33
#define L1_pin 32
#define C_pin 35
#define R1_pin 34
#define R2_pin 39

// Use for compare the analog value to digital value (B = analog > ref , W = analog <= ref)
int ref_sensor[5] = {1200, 2000, 2000, 2000, 1200};

// Sensor variable
boolean sensor_bool[5] = {0, 0, 0, 0, 0};
int sensor_val[5] = {1, 1, 1, 1, 1};

// Motor&PID parameter
int baseSpeed = 60;
int maxSpeed = 150;
float pid1_parameter[3] = {17, 1, 0.0000001};

void setup()
{
  pinMode(L2_pin, INPUT);
  pinMode(L1_pin, INPUT);
  pinMode(C_pin, INPUT);
  pinMode(R1_pin, INPUT);
  pinMode(R2_pin, INPUT);
  Serial.begin(115200);

  trackline_Cross(pid1_parameter, baseSpeed, 50, 1, 150);

  trackline_L(pid1_parameter, baseSpeed, 50, 1, 150);

  trackline_R(pid1_parameter, baseSpeed, 50, 1, 150);
}
void loop()
{
  trackline_pid(pid1_parameter, baseSpeed);
}
