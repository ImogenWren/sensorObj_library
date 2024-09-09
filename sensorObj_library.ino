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


uint16_t DAC_value;
DAC_value = flowValve.do_conversionTable(flowValveValue, flowValve_map);



void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}
