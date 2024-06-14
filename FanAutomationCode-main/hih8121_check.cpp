/* includes */
#include <Wire.h>

/* prototypes */
void delay_minutes(unsigned int);

/* defines */
#define BYTES2READ (4)
#define HIH8121addr (0x27)  /* slave I2c address */
#define EPOCH (10)  /* time between reads (mins) */

// FOR HEAT INDEX CALCULATION
const float c1 = -8.78469475556;
const float c2 = 1.61139411;
const float c3 = 2.33854893989;
const float c4 = -0.14611605;
const float c5 = -0.012308094;
const float c6 = -0.0164248277778;
const float c7 = 0.002211732;
const float c8 = 0.00072546;
const float c9 = -0.000003582;

void setup()
{
  Serial.begin(19200);
  Wire.begin(38, 39);
}

void loop()
{
  byte HIHdata[4];  // data read from the sensor
  byte HIHstatus;  // sensor status bits
  unsigned int RHraw, Tempraw; // raw RH (%) and temp (c) values
  float RH, TempC, TempF;  // converted RH (%) and Temp (C)
  byte dummydata = 0xff;  // dummy data for kick write
  unsigned int sample = 1;  // sample counter

  delay(3000);  // wait while opening monitor screen
  Serial.println("  HIH8121 Sensor Test");
  Serial.println();
  // the following delay is just a reminder that on power up you want to
  // wait before talking to the sensor so you can't put it into command mode
  delay(50);  // wait past the command window
  /* print header */
  Serial.println("Sample  RH  Temp (F)");
  /* read sensor and display result loop*/
  while (1) {
  /* kick the sensor */
  Wire.beginTransmission(HIH8121addr);
  Wire.write(dummydata);
  Wire.endTransmission(); // send it
  delay(50);  // 50 ms delay for measurement (~36.65 ms)
  // get the data
  Wire.requestFrom(HIH8121addr, BYTES2READ); // get 4 bytes of data
  HIHdata[0] = Wire.read();
  HIHdata[1] = Wire.read();
  HIHdata[2] = Wire.read();
  HIHdata[3] = Wire.read();
  HIHstatus = HIHdata[0] & 0b11000000;  // get status bits
  if (HIHstatus != 0) {
  Serial.println("Status Bits Error!");
  }
  HIHdata[0] = HIHdata[0] & 0b00111111;  //mask status
  Tempraw = (HIHdata[2] * 256) + HIHdata[3];
  Tempraw = Tempraw / 4;  //shift temperature
  RHraw = (HIHdata[0] * 256) + HIHdata[1];
  RH = ((float)RHraw / 16382.0) * 100.0;
  TempC = (((float)Tempraw / 16382.0) * 165.0) - 40;
  TempF = (TempC * 9/5) + 32;
  Serial.print(sample);
  Serial.print("  ");
  Serial.print(RH);
  Serial.print("  ");
  Serial.print(TempF);
  Serial.println();
  float heat_index = -42.379 + 2.04901523*TempF + 10.14333127*RH - .22475541*TempF*RH - .00683783*TempF*TempF - .05481717*RH*RH + .00122874*TempF*TempF*RH + .00085282*TempF*RH*RH - .00000199*TempF*TempF*RH*RH; 
  heat_index = (heat_index - 32) * 5/9;
  Serial.print("Heat index: ");
  Serial.println(heat_index);
  delay_minutes(EPOCH);
  sample++;
  }
}

void delay_minutes(unsigned int mins) {
  unsigned int count;

  for (count = 0; count < mins; count++) {
  //delay(60000);  // delay 60 seconds
  delay(1000); // use this if you want seconds epoch
  }
}
