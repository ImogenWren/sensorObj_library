/*  sensorObj.cpp - The Generic Sensor Conversion Library for Arduino

    Convert Voltage, 4-20mA and 0-20mA  linear signals into process values

    Imogen Heard
    21/12/23


*/

#include "sensorObj.h"

sensorObj::sensorObj() {
}

void sensorObj::setCalibration(float _process_min, float _process_max, float _adc_min, float _adc_max, float _postoffset) {
  adc_min = _adc_min;
  process_min = _process_min;
  adc_max = _adc_max;
  process_max = _process_max;
  input_range = adc_max - adc_min;
  process_range = process_max - process_min;
  cal.preoffset = adc_min;  // the initial offset is on the adc side, not the PROCESS side
  cal.factor = process_range / input_range;
  cal.postoffset = _postoffset;
  calSet = true;
#if DEBUG_SENSOR_CALC == true
  Serial.print(sensorID);
  Serial.print(F(": Sensor Calibration data saved"));
#endif
}


// Generic Method given values passed as min/max during startup. Non Generic methods coming soon!
float sensorObj::calcProcessVar(float _adcVal) {
  float processVarible = 0;
  // Disable sensor scaling to see raw ADC value for calibration
#if DISABLE_SENSOR_SCALING == false
  processVarible = ((_adcVal - cal.preoffset) * cal.factor) + cal.postoffset;
  currentVal = processVarible;
#elif DISABLE_SENSOR_SCALING == true
#pragma "Warning Sensor Scaling DISABLED"
  processVarible = _adcVal;
#endif
#if DEBUG_SENSOR_CALC == true
  Serial.print(sensorID);
  Serial.print(F(": "));
  Serial.print(_adcVal);
  Serial.print(ADC_units);
  Serial.print(processVarible);
  Serial.print(F(" "));
  Serial.println(process_units);
#endif
  return processVarible;
}

float sensorObj::returnVal() {
  return currentVal;
}

void sensorObj::updateHistory(float _sensorVal) {
  //currentVal = _sensorVal;  // this is done during calc process val
  for (int i = 0; i < SENSOR_BUFFER_SIZE - 1; i++) {  //make space in the array: inefficient for large arrays - better solution?
    sensorHistory[i] = sensorHistory[i + 1];
  }
  sensorHistory[SENSOR_BUFFER_SIZE - 1] = _sensorVal;

#if DEBUG_SENSOR_HISTORY == true
  for (int i = 0; i < SENSOR_BUFFER_SIZE; i++) {
    Serial.print(sensorHistory[i]);
    Serial.print(F(", "));
  }
  Serial.println();
#endif
}



// MAP FUNCTION METHODS
void sensorObj::printTable(const std::map<int16_t, float> &map) {
  for (int i = 0; i < 11; i++) {
    auto it = map.find(i * 10);
    if (it != map.end()) {
      std::cout << "Key: " << i * 10 << ", Value: " << it->second << std::endl;
    } else {
      std::cout << "Key: " << i * 10 << ", not found!" << std::endl;
    }
  }
  std::cout << std::endl;
}

uint16_t sensorObj::do_conversionTable(int16_t inputVal, const std::map<int16_t, uint16_t> &map) {
  // First check Size of map table
  uint16_t map_size = map.size();
  // first need to work out if value exists in table
  auto item = map.find(inputVal);  // This returns an iterator type
#if DEBUG_CONVERSION_TABLE == true
  std::cout << "Map Size: " << map_size << std::endl;
#endif
  if (map_size < 1) return 0;

  if (item != map.end()) {  // Technically this special case isnt required as the interpolation would return the same result
                            // item found -> return the value
#if DEBUG_CONVERSION_TABLE == true
    std::cout << "Item Found: " << item->second << std::endl;
    std::cout << "First Iterator (key): " << item->first << std::endl;
    std::cout << "Second Iterator (value): " << item->second << std::endl;
#endif
    return item->second;
  } else {
    // item not found
    // if we are beyond the limits, return the first/last element
    if (inputVal < map.begin()->first) {
#if DEBUG_CONVERSION_TABLE == true
      std::cout << "Item Under Range, returning lowest value " << std::endl;
#endif
      return map.begin()->second;
    }
    // if (inputVal > map.rbegin()->first) return map.rbegin()->second;  // This line causes compilation errors, possibly due to the use of const in arguments list and .rbegin()
    auto last = --map.end();  // working implementation
    if (inputVal > last->first) {
#if DEBUG_CONVERSION_TABLE == true
      std::cout << "Item Over Range, returning highest value " << std::endl;
#endif
      return last->second;
    }

    // else:
    // Interpolate between values above & below
    // I got this code from here https://stackoverflow.com/questions/26597983/stdmap-find-key-pair-and-interpolate-value
    //   auto lower = map.lower_bound(value) == map.begin() ? map.begin() : --( map.lower_bound(value)) ; This line equates to the if statement below, but trying to avoid the second call to table.lower_bound

    auto lower = map.lower_bound(inputVal);
    lower = (lower == map.begin()) ? map.begin() : --lower;  // which can be shortened to this if we pre allocate the local variable
    auto upper = map.upper_bound(inputVal);
    uint16_t returnVal = round(lower->second + (upper->second - lower->second) * float(inputVal - lower->first) / fabs(upper->first - lower->first));
#if DEBUG_CONVERSION_TABLE == true
    std::cout << "Item: " << inputVal << " Not Found, interpolating: lower: " << lower->first << " upper: " << upper->first << std::endl;
    std::cout << "Return Val: " << returnVal << std::endl;
#endif
    return returnVal;
  }
}




float sensorObj::calAverage() {
  return 0;
}
float sensorObj::calcMin() {
  return 0;
}
float sensorObj::calcMax() {
  return 0;
}
float sensorObj::calcDxDy() {
  return 0;
}
float sensorObj::calcLMS() {
  return 0;
}