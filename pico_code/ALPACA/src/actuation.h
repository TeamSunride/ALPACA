#pragma once
#include <Arduino.h>
#include <Servo.h>

#define FILL_PIN 9
#define OX_PIN 10
#define FUEL_PIN 11
#define DUMP_PIN 12
#define IGNITER1_PIN 13
#define IGNITER2_PIN 14

#define CLOSE_ANGLE_MS 500
#define OPEN_ANGLE_MS 1500


extern Servo fuel;
extern Servo ox;
extern Servo fill;
extern Servo dump;

void init_servos();
void moveServo(const char* valve, const char* pos);
void triggerSequence(const char* sequence, const char* param1, const char* param2);

//sequences
void estop_sequence();
void abort_sequence();
void launch_conv_sequence(int igniter_headstart_ms, int oxidiser_delay_ms);
//igniter_headstart is the delay between igniter firing and conventional fuel valve opening
//oxidiser_delay is the delay between fuel valve opening and oxidiser valve opening (allows fuel to make its way through the channels)

void launch_poppet_sequence(int igniter_headstart_ms);
void ignition_test_sequence();
