#include <Arduino.h>
#include <Switch.h>

#define ABORT 2

#define FILL 4
#define PROP_ARM 3
#define SWITCH_ARM A2

#define ESTOP 9
#define FUEL 8
#define OX 7
#define DUMP 16
#define LAUNCH 15

#define LAUNCH_LED A3

Switch abort_btn = Switch(ABORT, INPUT_PULLUP, LOW, 10);
Switch fill = Switch(FILL, INPUT_PULLUP, LOW, 50);
Switch prop_arm = Switch(PROP_ARM, INPUT_PULLUP, LOW, 50);
Switch switch_arm = Switch(SWITCH_ARM, INPUT_PULLUP, LOW, 50);
Switch estop = Switch(ESTOP, INPUT_PULLUP, LOW, 10);
Switch fuel = Switch(FUEL, INPUT_PULLUP, LOW, 50);
Switch ox = Switch(OX, INPUT_PULLUP, LOW, 50);
Switch dump = Switch(DUMP, INPUT_PULLUP, LOW, 50);
Switch launch = Switch(LAUNCH, INPUT_PULLUP, LOW, 10);


bool switches_armed = false;
bool prop_armed = false;


void abort_callback(void* ref){
  if (switches_armed)
  {
    Serial.println(0);
  }
}

void prop_arm_callback(void* ref){
  if (switches_armed)
  {
    prop_armed = true;
    Serial.println(1);
  }  
}

void prop_disarm_callback(void* ref){
  prop_armed = false;
  Serial.println(2);
}

void switch_arm_callback(void* ref){
  switches_armed = true;
  Serial.println(3);
}

void switch_disarm_callback(void* ref){
  switches_armed = false;
  Serial.println(4);
}

void estop_callback(void* ref){
  if (switches_armed)
  {
    Serial.println(5);
  }
}

void launch_callback(void* ref){
  if (switches_armed && prop_armed)
  {
    Serial.println(6);
  }
}

void fill_open_callback(void* ref){
  if (switches_armed)
  {
    Serial.println(7);
  }
}

void fill_close_callback(void* ref){
  if (switches_armed)
  {
    Serial.println(8);
  }
}

void fuel_open_callback(void* ref){
  if (switches_armed && prop_armed)
  {
    Serial.println(9);
  }
}

void fuel_close_callback(void* ref){
  if (switches_armed && prop_armed)
  {
    Serial.println(10);
  }
}

void ox_open_callback(void* ref){
  if (switches_armed && prop_armed)
  {
    Serial.println(11);
  }
}

void ox_close_callback(void* ref){
  if (switches_armed && prop_armed)
  {
    Serial.println(12);
  }
}

void dump_open_callback(void* ref){
  if (switches_armed && prop_armed)
  {
    Serial.println(13);
  }
}

void dump_close_callback(void* ref){
  if (switches_armed && prop_armed)
  {
    Serial.println(14);
  }
}


void setup(){
  Serial.begin(115200);
  pinMode(LAUNCH_LED, OUTPUT);

  abort_btn.setPushedCallback(&abort_callback, nullptr);
  prop_arm.setPushedCallback(&prop_arm_callback, nullptr);
  prop_arm.setReleasedCallback(&prop_disarm_callback, nullptr);
  switch_arm.setPushedCallback(&switch_arm_callback, nullptr);
  switch_arm.setReleasedCallback(&switch_disarm_callback, nullptr);
  estop.setPushedCallback(&estop_callback, nullptr);
  launch.setPushedCallback(&launch_callback, nullptr);
  fill.setPushedCallback(&fill_open_callback, nullptr);
  fill.setReleasedCallback(&fill_close_callback, nullptr);
  fuel.setPushedCallback(&fuel_open_callback, nullptr);
  fuel.setReleasedCallback(&fuel_close_callback, nullptr);
  ox.setPushedCallback(&ox_open_callback, nullptr);
  ox.setReleasedCallback(&ox_close_callback, nullptr);
  dump.setPushedCallback(&dump_open_callback, nullptr);
  dump.setReleasedCallback(&dump_close_callback, nullptr);

}

void loop(){
  abort_btn.poll();
  fill.poll();
  prop_arm.poll();
  switch_arm.poll();
  estop.poll();
  fuel.poll();
  ox.poll();
  dump.poll();
  launch.poll();
  digitalWrite(LAUNCH_LED, (switches_armed && prop_armed) ? HIGH : LOW );
  
  
}