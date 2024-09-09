/*  sensorObj.h - The Generic Sensor Conversion Library for Arduino

    - Convert Voltage, 4-20mA and 0-20mA  linear signals into process values
    - Deal with non-linear signals using lookup table &  linear interpolating between table values


    Imogen Heard
    Last Updated:
    16/08/2024


*/

#pragma once

#ifndef sensorObj_h
#define sensorObj_h

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "stdio.h"
#include <ArduinoSTL.h>
#include <iostream>
#include <map>


//#define VOLTAGE_SENSOR 0
//#define CURRENT_SENSOR 1
//#define OTHER_SENSOR 2

typedef enum {
  VOLTAGE_SENSOR,
  CURRENT_SENSOR,
  OTHER_SENSOR
} sensorType;

#define SENSOR_BUFFER_SIZE 3
#define DEBUG_CONVERSION_TABLE false
#define DEBUG_SENSOR_CALC false

//#define DISABLE_SENSOR_SCALING true   // disables sensor scaling and outputs raw ADC value - useful for calibrating sensors

//#define DISABLE_SENSOR_SCALING false   // disables sensor scaling and outputs raw DAC value - useful for calibrating sensors - defined in main sketch

class sensorObj {

public:

  sensorObj(int _sensorType, const char _sensorUnits[16], const char _sensorID[32] = { "xx_sensor" })
    : sensorType(_sensorType) {
    strcpy(sensorID, _sensorID);
    strcpy(process_units, _sensorUnits);
    if (sensorType == VOLTAGE_SENSOR) {
      strcpy(ADC_units, " V ");
    } else if (sensorType == CURRENT_SENSOR) {
      strcpy(ADC_units, " mA ");
    } else if (sensorType == OTHER_SENSOR) {
      strcpy(ADC_units, " n/a ");
    } else {
      strcpy(ADC_units, " ? ");
    }
  }

  sensorObj();

  int sensorType;

  // Linear Sensor Variables & Methods
  float adc_min;  // measured value min
  float adc_max;  // measured value max
  float process_min;
  float process_max;
  float input_range;
  float process_range;
  int rangeSet = 0;

  float currentVal = 0;  //current value of the sensor
  uint32_t timeStamp;    // timeStamp for last datapoint taken

  // Should be called before calcProcessVar() to set sensor calibration
  void setCalibration(float _process_min = 0, float _process_max = 100, float _adc_min = 0, float _daq_max = 65535, float _postoffset = 0);

  float calcProcessVar(float _adcVal);

  float returnVal();  // returns the current process val

  float sensorHistory[SENSOR_BUFFER_SIZE];
  //std::vector<float> sensorHistory;
  void updateHistory(float _sensorVal);

  // Non Linear Lookup Table Variables & Methods

  void printTable(const std::map<int16_t, float> &map);  // Prints the <map> object used to define the sensor conversion table

  uint16_t do_conversionTable(int16_t inputVal, const std::map<int16_t, uint16_t> &map);


  // Not Implemented yet
  float calAverage();
  float calcMin();
  float calcMax();
  float calcDxDy();
  float calcLMS();

  char sensorID[24] = { " sensor_xx " };
  char process_units[16] = { " unit " };
  char ADC_units[16] = { " unit " };

  bool calSet = false;
  struct calData {
    float preoffset;
    float factor;
    float postoffset;
  } cal;

private:

};


#endif