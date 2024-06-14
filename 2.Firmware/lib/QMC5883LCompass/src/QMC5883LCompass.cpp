/*
===============================================================================================================
QMC5883LCompass.h
Library for using QMC5583L series chip boards as a compass.
Learn more at [https://github.com/mprograms/QMC5883LCompass]

Supports:

- Getting values of XYZ axis.
- Calculating Azimuth.
- Getting 16 point Azimuth bearing direction (0 - 15).
- Getting 16 point Azimuth bearing Names (N, NNE, NE, ENE, E, ESE, SE, SSE, S, SSW, SW, WSW, W, WNW, NW, NNW)
- Smoothing of XYZ readings via rolling averaging and min / max removal.
- Optional chipset modes (see below)

===============================================================================================================

v1.0 - June 13, 2019
Written by MPrograms 
Github: [https://github.com/mprograms/]

Release under the GNU General Public License v3
[https://www.gnu.org/licenses/gpl-3.0.en.html]

===============================================================================================================



FROM QST QMC5883L Datasheet [https://nettigo.pl/attachments/440]
-----------------------------------------------
 MODE CONTROL (MODE)
	Standby			0x00
	Continuous		0x01

OUTPUT DATA RATE (ODR)
	10Hz        	0x00
	50Hz        	0x04
	100Hz       	0x08
	200Hz       	0x0C

FULL SCALE (RNG)
	2G          	0x00
	8G          	0x10

OVER SAMPLE RATIO (OSR)
	512         	0x00
	256         	0x40
	128         	0x80
	64          	0xC0 
  
*/



#include "Arduino.h"
#include "QMC5883LCompass.h"
#include <Wire.h>

QMC5883LCompass::QMC5883LCompass() {
}


/**
	INIT
	Initialize Chip - This needs to be called in the sketch setup() function.
	
	@since v0.1;
**/
void QMC5883LCompass::init(){
	Wire.begin();
	_writeReg(0x0B,0x01);
	setMode(0x01,0x0C,0x10,0X00);
}


/**
	SET ADDRESS
	Set the I2C Address of the chip. This needs to be called in the sketch setup() function.
	
	@since v0.1;
**/
// Set I2C Address if different then default.
void QMC5883LCompass::setADDR(byte b){
	_ADDR = b;
}




/**
	REGISTER
	Write the register to the chip.
	
	@since v0.1;
**/
// Write register values to chip
void QMC5883LCompass::_writeReg(byte r, byte v){
	Wire.beginTransmission(_ADDR);
	Wire.write(r);
	Wire.write(v);
	Wire.endTransmission();
}


/**
	CHIP MODE
	Set the chip mode.
	
	@since v0.1;
**/
// Set chip mode
void QMC5883LCompass::setMode(byte mode, byte odr, byte rng, byte osr){
	_writeReg(0x09,mode|odr|rng|osr);
}


/**
 * Define the magnetic declination for accurate degrees.
 * https://www.magnetic-declination.com/
 * 
 * @example
 * For: Londrina, PR, Brazil at date 2022-12-05
 * The magnetic declination is: -19º 43'
 * 
 * then: setMagneticDeclination(-19, 43);
 */
void QMC5883LCompass::setMagneticDeclination(int degrees, uint8_t minutes) {
	_magneticDeclinationDegrees = degrees + minutes / 60;
}


/**
	RESET
	Reset the chip.
	
	@since v0.1;
**/
// Reset the chip
void QMC5883LCompass::setReset(){
	_writeReg(0x0A,0x80);
}

// 1 = Basic 2 = Advanced
void QMC5883LCompass::setSmoothing(byte steps, bool adv){
	_smoothUse = true;
	_smoothSteps = ( steps > 10) ? 10 : steps;
	_smoothAdvanced = (adv == true) ? true : false;
}

void QMC5883LCompass::calibrate() {
	clearCalibration();
	long calibrationData[3][2] = {{65000, -65000}, {65000, -65000}, {65000, -65000}};
  	long	x = calibrationData[0][0] = calibrationData[0][1] = getX();
  	long	y = calibrationData[1][0] = calibrationData[1][1] = getY();
  	long	z = calibrationData[2][0] = calibrationData[2][1] = getZ();

	unsigned long startTime = millis();

	while((millis() - startTime) < 10000) {
		read();

  		x = getX();
  		y = getY();
  		z = getZ();

		if(x < calibrationData[0][0]) {
			calibrationData[0][0] = x;
		}
		if(x > calibrationData[0][1]) {
			calibrationData[0][1] = x;
		}

		if(y < calibrationData[1][0]) {
			calibrationData[1][0] = y;
		}
		if(y > calibrationData[1][1]) {
			calibrationData[1][1] = y;
		}

		if(z < calibrationData[2][0]) {
			calibrationData[2][0] = z;
		}
		if(z > calibrationData[2][1]) {
			calibrationData[2][1] = z;
		}
	}

	setCalibration(
		calibrationData[0][0],
		calibrationData[0][1],
		calibrationData[1][0],
		calibrationData[1][1],
		calibrationData[2][0],
		calibrationData[2][1]
	);
}

/**
    SET CALIBRATION
	Set calibration values for more accurate readings
		
	@author Claus Näveke - TheNitek [https://github.com/TheNitek]
	
	@since v1.1.0

	@deprecated Instead of setCalibration, use the calibration offset and scale methods.
**/
void QMC5883LCompass::setCalibration(int x_min, int x_max, int y_min, int y_max, int z_min, int z_max){
	setCalibrationOffsets(
		(x_min + x_max)/2,
		(y_min + y_max)/2,
		(z_min + z_max)/2
	);

	float x_avg_delta = (x_max - x_min)/2;
	float y_avg_delta = (y_max - y_min)/2;
	float z_avg_delta = (z_max - z_min)/2;

	float avg_delta = (x_avg_delta + y_avg_delta + z_avg_delta) / 3;

	setCalibrationScales(
		avg_delta / x_avg_delta,
		avg_delta / y_avg_delta,
		avg_delta / z_avg_delta
	);
}

void QMC5883LCompass::setCalibrationOffsets(float x_offset, float y_offset, float z_offset) {
	_offset[0] = x_offset;
	_offset[1] = y_offset;
	_offset[2] = z_offset;
}

void QMC5883LCompass::setCalibrationScales(float x_scale, float y_scale, float z_scale) {
	_scale[0] = x_scale;
	_scale[1] = y_scale;
	_scale[2] = z_scale;
}

float QMC5883LCompass::getCalibrationOffset(uint8_t index) {
	return _offset[index];
}

float QMC5883LCompass::getCalibrationScale(uint8_t index) {
	return _scale[index];
}

void QMC5883LCompass::clearCalibration(){
	setCalibrationOffsets(0., 0., 0.);
	setCalibrationScales(1., 1., 1.);
}

/**
	READ
	Read the XYZ axis and save the values in an array.
	
	@since v0.1;
**/
void QMC5883LCompass::read(){
	Wire.beginTransmission(_ADDR);
	Wire.write(0x00);
	int err = Wire.endTransmission();
	if (!err) {
		Wire.requestFrom(_ADDR, (byte)6);
		_vRaw[0] = (int)(int16_t)(Wire.read() | Wire.read() << 8);
		_vRaw[1] = (int)(int16_t)(Wire.read() | Wire.read() << 8);
		_vRaw[2] = (int)(int16_t)(Wire.read() | Wire.read() << 8);

		_applyCalibration();
		
		if ( _smoothUse ) {
			_smoothing();
		}
		
		//byte overflow = Wire.read() & 0x02;
		//return overflow << 2;
	}
}

/**
    APPLY CALIBRATION
	This function uses the calibration data provided via @see setCalibration() to calculate more
	accurate readings
	
	@author Claus Näveke - TheNitek [https://github.com/TheNitek]
	
	Based on this awesome article:
	https://appelsiini.net/2018/calibrate-magnetometer/
	
	@since v1.1.0
	
**/
void QMC5883LCompass::_applyCalibration(){
	_vCalibrated[0] = (_vRaw[0] - _offset[0]) * _scale[0];
	_vCalibrated[1] = (_vRaw[1] - _offset[1]) * _scale[1];
	_vCalibrated[2] = (_vRaw[2] - _offset[2]) * _scale[2];
}


/**
	SMOOTH OUTPUT
	This function smooths the output for the XYZ axis. Depending on the options set in
	@see setSmoothing(), we can run multiple methods of smoothing the sensor readings.
	
	First we store (n) samples of sensor readings for each axis and store them in a rolling array.
	As each new sensor reading comes in we replace it with a new reading. Then we average the total
	of all (n) readings.
	
	Advanced Smoothing
	If you turn advanced smoothing on, we will select the min and max values from our array
	of (n) samples. We then subtract both the min and max from the total and average the total of all
	(n - 2) readings.
	
	NOTE: This function does several calculations and can cause your sketch to run slower.
	
	@since v0.3;
**/
void QMC5883LCompass::_smoothing(){
	byte max = 0;
	byte min = 0;
	
	if ( _vScan > _smoothSteps - 1 ) { _vScan = 0; }
	
	for ( int i = 0; i < 3; i++ ) {
		if ( _vTotals[i] != 0 ) {
			_vTotals[i] = _vTotals[i] - _vHistory[_vScan][i];
		}
		_vHistory[_vScan][i] = _vCalibrated[i];
		_vTotals[i] = _vTotals[i] + _vHistory[_vScan][i];
		
		if ( _smoothAdvanced ) {
			max = 0;
			for (int j = 0; j < _smoothSteps - 1; j++) {
				max = ( _vHistory[j][i] > _vHistory[max][i] ) ? j : max;
			}
			
			min = 0;
			for (int k = 0; k < _smoothSteps - 1; k++) {
				min = ( _vHistory[k][i] < _vHistory[min][i] ) ? k : min;
			}
					
			_vSmooth[i] = ( _vTotals[i] - (_vHistory[max][i] + _vHistory[min][i]) ) / (_smoothSteps - 2);
		} else {
			_vSmooth[i] = _vTotals[i]  / _smoothSteps;
		}
	}
	
	_vScan++;
}


/**
	GET X AXIS
	Read the X axis
	
	@since v0.1;
	@return int x axis
**/
int QMC5883LCompass::getX(){
	return _get(0);
}


/**
	GET Y AXIS
	Read the Y axis
	
	@since v0.1;
	@return int y axis
**/
int QMC5883LCompass::getY(){
	return _get(1);
}


/**
	GET Z AXIS
	Read the Z axis
	
	@since v0.1;
	@return int z axis
**/
int QMC5883LCompass::getZ(){
	return _get(2);
}

/**
	GET SENSOR AXIS READING
	Get the smoothed, calibration, or raw data from a given sensor axis
	
	@since v1.1.0
	@return int sensor axis value
**/
int QMC5883LCompass::_get(int i){
	if ( _smoothUse ) 
		return _vSmooth[i];
	
	return _vCalibrated[i];
}



/**
	GET AZIMUTH
	Calculate the azimuth (in degrees);
	Correct the value with magnetic declination if defined. 
	
	@since v0.1;
	@return int azimuth
**/
int QMC5883LCompass::getAzimuth(){
	float heading = atan2( getY(), getX() ) * 180.0 / PI;
	heading += _magneticDeclinationDegrees;
	return (int)heading % 360;
}


/**
	GET BEARING
	Divide the 360 degree circle into 16 equal parts and then return the a value of 0-15
	based on where the azimuth is currently pointing.

 
	@since v1.2.1 - function takes into account negative azimuth values. Credit: https://github.com/prospark
	@since v1.0.1 - function now requires azimuth parameter.
	@since v0.2.0 - initial creation
	
	@return byte direction of bearing
*/
byte QMC5883LCompass::getBearing(int azimuth){
	unsigned long a = ( azimuth > -0.5 ) ? azimuth / 22.5 : (azimuth+360)/22.5;
	unsigned long r = a - (int)a;
	byte sexdec = 0;	
	sexdec = ( r >= .5 ) ? ceil(a) : floor(a);
	return sexdec;
}


/**
	This will take the location of the azimuth as calculated in getBearing() and then
	produce an array of chars as a text representation of the direction.
	
	NOTE: This function does not return anything since it is not possible to return an array.
	Values must be passed by reference back to your sketch.
	
	Example:
	
	( if direction is in 1 / NNE)
	
	char myArray[3];
	compass.getDirection(myArray, azimuth);
	
	Serial.print(myArray[0]); // N
	Serial.print(myArray[1]); // N
	Serial.print(myArray[2]); // E
	
	
	@see getBearing();
	
	@since v1.0.1 - function now requires azimuth parameter.
	@since v0.2.0 - initial creation
*/
void QMC5883LCompass::getDirection(char* myArray, int azimuth){
	int d = getBearing(azimuth);
	myArray[0] = _bearings[d][0];
	myArray[1] = _bearings[d][1];
	myArray[2] = _bearings[d][2];
}
