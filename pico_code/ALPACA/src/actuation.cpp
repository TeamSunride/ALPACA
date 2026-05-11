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

//=================HELPER FUNCTIONS=================

void moveServo(const char* valve, const char* pos){
  if (strcmp(valve, "fill") == 0 )
  {
    fill.writeMicroseconds( (strcmp(pos, "open") == 0 ) ? OPEN_ANGLE_MS : CLOSE_ANGLE_MS ); 
  }

  if (strcmp(valve, "ox") == 0 )
  {
    ox.writeMicroseconds( (strcmp(pos, "open") == 0) ? OPEN_ANGLE_MS : CLOSE_ANGLE_MS ); 
  }

  if (strcmp(valve, "fuel") == 0 )
  {
    fuel.writeMicroseconds( (strcmp(pos, "open") == 0) ? OPEN_ANGLE_MS : CLOSE_ANGLE_MS ); 
  }
  
  if (strcmp(valve, "dump") == 0 )
  {
    dump.writeMicroseconds( (strcmp(pos, "open") == 0) ? OPEN_ANGLE_MS : CLOSE_ANGLE_MS ); 
  }

}


void triggerSequence(const char* sequence, const char* param1, const char* param2){
  if (strcmp(sequence, "estop") == 0 )
  {
    estop_sequence();
  }

  if (strcmp(sequence, "abort") == 0 )
  {
    abort_sequence();
  }

  if (strcmp(sequence, "launch_conv") == 0 )
  {
    launch_conv_sequence((int)param1, (int)param2);
  }
  
  if (strcmp(sequence, "launch_poppet") == 0 )
  {
    launch_poppet_sequence((int)param1);
  }

  if (strcmp(sequence, "ignition_test") == 0 )
  {
    ignition_test_sequence();
  }
  
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

void launch_conv_sequence(int igniter_headstart_ms, int oxidiser_delay_ms){
  fill.writeMicroseconds(CLOSE_ANGLE_MS);

  digitalWrite(IGNITER1_PIN, HIGH);
  digitalWrite(IGNITER2_PIN, HIGH);
  delayMicroseconds(igniter_headstart_ms);

  fuel.writeMicroseconds(OPEN_ANGLE_MS);
  delayMicroseconds(oxidiser_delay_ms);

  ox.writeMicroseconds(OPEN_ANGLE_MS);
  delay(15);

  fuel.writeMicroseconds(CLOSE_ANGLE_MS);
  ox.writeMicroseconds(CLOSE_ANGLE_MS);
  digitalWrite(IGNITER1_PIN, LOW);
  digitalWrite(IGNITER2_PIN, LOW);
}

void launch_poppet_sequence(int igniter_headstart_ms){
  fill.writeMicroseconds(CLOSE_ANGLE_MS);
  
  digitalWrite(IGNITER1_PIN, HIGH);
  digitalWrite(IGNITER2_PIN, HIGH);
  delayMicroseconds(igniter_headstart_ms);

  dump.writeMicroseconds(OPEN_ANGLE_MS);
  delay(15);

  dump.writeMicroseconds(CLOSE_ANGLE_MS);
  digitalWrite(IGNITER1_PIN, LOW);
  digitalWrite(IGNITER2_PIN, LOW);

}

void ignition_test_sequence(){
  digitalWrite(IGNITER1_PIN, HIGH);
  delay(1000);
  digitalWrite(IGNITER1_PIN, LOW);
  delay(1000);
  digitalWrite(IGNITER2_PIN, HIGH);
  delay(1000);
  digitalWrite(IGNITER2_PIN, LOW);
  delay(1000);
}