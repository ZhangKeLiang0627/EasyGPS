/*
===============================================================================================================
QMC5883LCompass.h Library Bearing Example Sketch
Learn more at [https://github.com/mprograms/QMC5883Compas]

This example shows how to get the range that the current bearing is in. You can use this to roll
your very own direction output.

===============================================================================================================
Release under the GNU General Public License v3
[https://www.gnu.org/licenses/gpl-3.0.en.html]
===============================================================================================================
*/
#include <QMC5883LCompass.h>

QMC5883LCompass compass;

void setup() {
  Serial.begin(9600);
  compass.init();
}

void loop() {
  compass.read();

  byte a = compass.getAzimuth();
  // Output here will be a value from 0 - 15 based on the direction of the bearing / azimuth.
  byte b = compass.getBearing(a);
  
  Serial.print("B: ");
  Serial.print(b);
  Serial.println();
  
  delay(250);
}
