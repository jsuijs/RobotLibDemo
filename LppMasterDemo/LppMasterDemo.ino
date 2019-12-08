#include <Wire.h>
#include "LppMaster.h"
#include "JsShared.h"
TLpp Lpp;

void setup() {
  Serial.begin(115200);      // start serial
  fdevopen( &my_putc, 0);    // Koppel printf uitvoer (stdout) via my_putc() aan de serial port.

  Wire.begin();              // start I2C

  Lpp.begin();

  // Lees en print status (ter informatie)
  boolean r = Lpp.ReadStatus();
  if (r) {
    Lpp.PrintStatus();
  } else {
    printf("I2C error reading status from Lpp\n");
  }

  Lpp.ArraySetup(90, 20, 9);    // vanaf 90 graden, segmenten van 20 graden, 9 stuks
  Lpp.SensorSetup(0, 90, 180);  // Sensor 0, vanaf 90 graden, segment van 180 graden

  Lpp.Start();
}

void loop()
{

  Lpp.ReadSensors();
  printf("Sensor 0, graden: %d, afstand: %d\n", Lpp.Sensor[0].Degrees32/32, Lpp.Sensor[0].Distance);

  Lpp.ReadArray();
  printf("Array - eerste 3 segmenten: %d %d %d\n", Lpp.Array[0].Distance, Lpp.Array[1].Distance, Lpp.Array[2].Distance);

  delay(500);
}
