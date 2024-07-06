# QMC5883L Compass Arduino Library

by [MPrograms](https://github.com/mprograms/QMC5883LCompass/)
| 
[Github Project Page](https://github.com/mprograms/QMC5883LCompass/)

---
## Overview
QMC5883L Compass is a Arduino library for using QMC5583L series chip boards as a compass.

Supports:

- Getting values of XYZ axis.
- Calculating Azimuth.
- Getting 16 point Azimuth bearing direction (0 - 15).
- Getting 16 point Azimuth bearing Names (N, NNE, NE, ENE, E, ESE, SE, SSE, S, SSW, SW, WSW, W, WNW, NW, NNW)
- Smoothing of XYZ readings via rolling averaging and min / max removal.
- Optional chipset modes (see below)


---

## Required Libraries

- Wire.h - No need to included it at the top of your sketch. QMC5883L Compass will included it for you. Just ensure that it is installed in your IDE.

---


## Board Hookup Reference

QMC5883L hookup to Arduino Uno / Nano. For other boards see  [Arduino Wire Reference](https://www.arduino.cc/en/Reference/Wire)

```
VCC  O ---- O +5v
GND  O ---- O GND
SCL  O ---- O A5
SDA  O ---- O A4
DRDY O ---- X NOT CONNECTED
```

---

## Arduino Code

### Getting Started
To begin, include the QMC5883L Compass Library at the top of your sketch.

```
#include <QMC5883LCompass.h>
QMC5883LCompass compass;
```

Then in the setup() function add:
```
void setup(){
  compass.init();
}
```


### Getting Values

QMC5883L Compass Library makes it easy to get sensor values. Call any of the following within the loop.

#### Getting X, Y, or Z Axis
To get the X, Y, or Z sensor readings, simply call the desired function.

```
void loop(){
   int x = compass.getX();
   int y = compass.getY();
   int z = compass.getZ();
}
```

#### Getting Azimuth
To get the calculated azimuth (compass degree) value, simply call `getAzimuth();`.

```
void loop(){
   int a = compass.getAzimuth();
}
```

#### Getting Direction / Bearings
QMC5883L Compass Library calculates the direction range and direction in which the sensor is pointing. There are two functions you can call.

To get a 16 point value of the direction the sensor is facing you can call `getBearing(azimuth)`. This will divide the 360 range of the compass into 16 parts and return a value of 0-15 in clockwise order. In this case 0 = N, 4 = E, 8 = S, 12 = W. This function is helpful if you wish to roll your own direction output function without the need for calculations.

```
void loop(){
   azimuth = compass.getAzimuth();
   byte b = compass.getBearing(azimuth);
}
```

To get a 16 point text representation of the direction the sensor is facing you can call `getDirection(azimuth);`. This will produce a char array[3] with letters representing each direction. Because we can't return an array we need to pass the values by reference.

```
void loop(){
   azimuth = compass.getAzimuth();
   char myArray[3];
   getDirection(myArray, azimuth);
}
```
If you want to print these values you can do so like this:

```
void loop(){
   azimuth = compass.getAzimuth();
   char myArray[3];
   
   getDirection(myArray, azimuth);
   
   Serial.print(myArray[0]);
   Serial.print(myArray[1]);
   Serial.print(myArray[2]);
   Serial.println();
}
```

---

## Example Sketch & Output

### Example Code

```
#include <QMC5883LCompass.h>

QMC5883LCompass compass;

void setup() {
  Serial.begin(9600);
  compass.init();
}

void loop() {

	int x, y, z, a, b;
	char myArray[3];
	
	compass.read();
  
	x = compass.getX();
	y = compass.getY();
	z = compass.getZ();
	
	a = compass.getAzimuth();
	
	b = compass.getBearing(a);

	compass.getDirection(myArray, a);
  
  
	Serial.print("X: ");
	Serial.print(x);

	Serial.print(" Y: ");
	Serial.print(y);

	Serial.print(" Z: ");
	Serial.print(z);

	Serial.print(" Azimuth: ");
	Serial.print(a);

	Serial.print(" Bearing: ");
	Serial.print(b);

	Serial.print(" Direction: ");
	Serial.print(myArray[0]);
	Serial.print(myArray[1]);
	Serial.print(myArray[2]);

	Serial.println();

	delay(250);
}
```

### Example Output


```
X: 1005 Y: -147 Z: 1281 Azimuth: 352 Bearing: 15 Direction: NNW
```


---

## Options & Settings


### Changing Chip Settings
The QMC5583L chip provides several different settings you can select.


#### Change I2C Address
To change the I2C address call `compass.setADDR(BYTE_VALUE);` before you call `compass.init();` like so:

```
void setup(){
  compass.setADDR(BYTE);
  compass.init();
}
```
#### Change Mode, Data Rate, Scale, Sample Ratio

You can also change the mode, sensitivity, sample rate and output rate of the QMC5583L chip. To do this, simply call `compass.setMode(MODE, ODR, RNG, OSR);` after you have called `compass.init()`. Note that each value must be a byte.

The values to set each mode are in the table below and were taken from the [QST QMC5583L datasheet](https://nettigo.pl/attachments/440).


| MODE CONTROL (MODE)     | Value |
| ----------------------- | ----- |
| Standby		          | 0x00  |
| Continuous	          | 0x01  |

| OUTPUT DATA RATE (ODR)  | Value |
| ----------------------- | ----- |
| 10Hz		              | 0x00  |
| 50Hz		              | 0x04  |
| 100Hz		              | 0x08  |
| 200Hz		              | 0x0C  |

| FULL SCALE (RNG)        | Value |
| ----------------------- | ----- |
| 2G			          | 0x00  |
| 8G			          | 0x10  |

| OVER SAMPLE RATIO (OSR) | Value |
| ----------------------- | ----- |
| 64			          | 0xC0  |
| 128			          | 0x80  |
| 256			          | 0x40  |
| 512			          | 0x00  |

---

## Smoothing Sensor Output

Smoothing can help in cases where sensor readings seem to bounce around. QMC5883L Compass Library uses a rolling average function to store (n) sensor readings and return the average of each axis. This averaging also places smoothing on azimuth and directional output as well.

If enabled, a second part of the function will take the current minimum and maximum values the current rolling average pass and remove them from the overall average. This can help remove unwanted highs and lows that might occur in an erroneous reading.

**It should be noted that the built-in smoothing function will result in extra processing time.**

To enable smoothing call `compass.setSmoothing(STEPS, ADVANCED);` before the loop.

- _STEPS_ : int, The number of steps to smooth the results by. Valid 1 to 10. Higher steps equals more smoothing but longer process time.
- _ADVANCED_ : bool, True will remove the max and min values from each step and then process as normal. Turning this feature on will results in even more smoothing but will take longer to process.
   *                     

```
void setup(){
  compass.init();
  compass.setSmoothing(10, true);
}
```


## Calibrating The Sensor

QMC5883LCompass library includes a calibration function and utility sketch to help you calibrate your QMC5883L chip. Calibration is a two-step process.

### Step 1: Run Calibration Sketch

1. Ensure that your QMC5883L chip is connected.
2. Locate the included calibration sketch under EXAMPLES > QMC5883LCOMPASS > CALIBRATION.
3. Upload the calibration sketch to your arduino and then open the serial monitor.
4. Follow the directions on the screen by moving your sensor around when the calibration process starts.
5. Once all calibration data has been collected, the sketch will tell provide you with some code that will look like `compass.setCalibration(-1537, 1266, -1961, 958, -1342, 1492);` Copy this code. You may want to save it for future reference.

### Step 2: Using Calibration Data

1. Open your project's sketch and paste the line of code you copied directly below the `compass.init()` call.
2. Use the QMC5883LCompass library as normal.

It is recommended that you use the provided calibration sketch to generate your sensor's min and max values but you can also add your own by using the `compass.setCalibration(X_MIN, X_MAX, Y_MIN, Y_MAX, Z_MIN, Z_MAX);` function.


## Contributions

Special thanks is given to the following individuals who have contributed to this library:

	- Claus Näveke : [TheNitek](https://github.com/TheNitek) for adding calibration functions to the library.
 	- Paulo C. B. Sincos : [paulosincos](https://github.com/paulosincos)https://github.com/paulosincos) for enhancements to the calibration script.
  	- ATsaruk : [ATsaruk](https://github.com/ATsaruk) for enhancements to the calibration script.
