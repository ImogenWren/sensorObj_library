# sensorObj_library
 Object Orientated Library for linear & non-linear sensor scaling & calculations


Example: for best results view in serial plotter
- Blue: Input Values (0, 100);
- Orange: Calculated output values using lookup table
  
```
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

  ```
 ![image](https://github.com/user-attachments/assets/26753759-6cf5-4f9e-a957-ed4e13df817a)

