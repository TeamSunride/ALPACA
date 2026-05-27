#include "actuation.h"

Servo fuel;
Servo ox;
Servo fill;
Servo dump;

void init_servos(){
  fill.attach(FILL_PIN, 500, 2500);
  ox.attach(OX_PIN, 500, 2500);
  fuel.attach(FUEL_PIN, 500, 2500);
  dump.attach(DUMP_PIN, 500, 2500);

  fill.writeMicroseconds(CLOSE_ANGLE_MS);
  ox.writeMicroseconds(CLOSE_ANGLE_MS);
  fuel.writeMicroseconds(CLOSE_ANGLE_MS);
  dump.writeMicroseconds(CLOSE_ANGLE_MS);
}

//=================SEQUENCES=================

void estop_sequence(){
  fill.writeMicroseconds(CLOSE_ANGLE_MS);
  ox.writeMicroseconds(CLOSE_ANGLE_MS);
  fuel.writeMicroseconds(CLOSE_ANGLE_MS);
  dump.writeMicroseconds(CLOSE_ANGLE_MS);
}

void abort_sequence(){
  fuel.writeMicroseconds(CLOSE_ANGLE_MS);
  ox.writeMicroseconds(CLOSE_ANGLE_MS);
  fill.writeMicroseconds(CLOSE_ANGLE_MS);
  dump.writeMicroseconds(OPEN_ANGLE_MS);
}

void launch_conv_sequence(int t_ign1_ms, int t_ign2_ms, int t_fuel_ms, int t_ox_ms){
  fill.writeMicroseconds(CLOSE_ANGLE_MS);

  delay(abs(t_ign1_ms) + 1);

  digitalWrite(IGNITER1_PIN, HIGH);

  delay(abs(t_ign2_ms - t_ign1_ms) + 1);
  
  digitalWrite(IGNITER2_PIN, HIGH);
  
  delay(abs(t_fuel_ms - t_ign2_ms - t_ign1_ms) + 1);

  fuel.writeMicroseconds(OPEN_ANGLE_MS);
  
  delay(abs(t_ox_ms - t_fuel_ms - t_ign2_ms - t_ign1_ms) + 1);

  ox.writeMicroseconds(OPEN_ANGLE_MS);
  
  delay(10000);

  fuel.writeMicroseconds(CLOSE_ANGLE_MS);
  ox.writeMicroseconds(CLOSE_ANGLE_MS);
  digitalWrite(IGNITER1_PIN, LOW);
  digitalWrite(IGNITER2_PIN, LOW);
}

void launch_poppet_sequence(int t_ign1_ms, int t_ign2_ms, int t_prop_ms){
  
  fill.writeMicroseconds(CLOSE_ANGLE_MS);
  
  delay(abs(t_ign1_ms) + 1);
  
  digitalWrite(IGNITER1_PIN, HIGH);
  
  delay(abs(t_ign2_ms - t_ign1_ms) + 1);
  
  digitalWrite(IGNITER2_PIN, HIGH);
  
  delay(abs(t_prop_ms - t_ign2_ms - t_ign1_ms) + 1);

  dump.writeMicroseconds(OPEN_ANGLE_MS);
  
  delay(10000);

  dump.writeMicroseconds(CLOSE_ANGLE_MS);
  digitalWrite(IGNITER1_PIN, LOW);
  digitalWrite(IGNITER2_PIN, LOW);

}

void ignition_test_sequence(int t_ign1_ms, int t_ign2_ms){
  delay(abs(t_ign1_ms) + 1);

  digitalWrite(IGNITER1_PIN, HIGH);

  delay(abs(t_ign2_ms - t_ign1_ms) + 1);

  digitalWrite(IGNITER2_PIN, HIGH);

  delay(2000);
  digitalWrite(IGNITER1_PIN, LOW);
  digitalWrite(IGNITER2_PIN, LOW);
}