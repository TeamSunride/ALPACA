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

#define LED_PIN 19

extern Servo fuel;
extern Servo ox;
extern Servo fill;
extern Servo dump;

void init_servos();

//sequences
void estop_sequence();
void abort_sequence();
void launch_conv_sequence(int t_ign1_ms, int t_ign2_ms, int t_fuel_ms, int t_ox_ms);
void launch_poppet_sequence(int t_ign1_ms, int t_ign2_ms, int t_prop_ms);
void ignition_test_sequence(int t_ign1_ms, int t_ign2_ms);
