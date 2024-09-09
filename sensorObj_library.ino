/*  sensorObj

 Object Orientated Library for linear sensor scaling using:
 - 5 datapoints (process_min, process_max, adc_min, adc_max, offset)

 & non-linear scaling using lookup tables & interpolation:
  { process_min, adc_min },
  {        xxxx, yyyy    },
  {        iiii, jjjj    },
  {        mmmm, nnnn    }
  ....
  { process_max, adc_max },

*/

#include "sensorObj.h"

sensorObj mySensor;

const std::map<int16_t, uint16_t> sensor_map = {  // (process_val, adc_val)
  { 0, 0 },
  { 10, 5 },
  { 20, 10 },
  { 30, 20 },
  { 40, 40 },
  { 50, 80 },
  { 60, 220 },
  { 70, 300 },
  { 80, 310 },
  { 90, 320 },
  { 100, 340 }
};


uint16_t DAC_value;
//DAC_value = flowValve.do_conversionTable(flowValveValue, flowValve_map);



void setup() {
  Serial.begin(115200);
  //Serial.print("sensorObj_library Example Sketch");
  //Serial.println("Input, Output,");
  delay(1000);
  char buffer[128];
  int outputVal;
  for (int i = 0; i < 100; i++) {                            // Using a for loop to simulate gathering data from a sensor
    outputVal = mySensor.do_conversionTable(i, sensor_map);  // (input_value, lookup_table);
    sprintf(buffer, "%4i, %4i,", i, outputVal);
    Serial.println(buffer);
  }
}

void loop() {
}
