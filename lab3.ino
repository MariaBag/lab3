#include <Arduino.h>
#include <MD_TCS230.h>
#include <LedControl.h>

#define  S0_OUT  2
#define  S1_OUT  3
#define  S2_OUT  4
#define  S3_OUT  5

MD_TCS230 ColorSensor(S2_OUT, S3_OUT, S0_OUT, S1_OUT);
LedControl ledDisplay = LedControl(26, 22, 24, 1);  // (DIN, CLK, CS, Количество дисплеев)

void setup() {
    sensorData whiteCalibration;
    whiteCalibration.value[TCS230_RGB_R] = 120060;
    whiteCalibration.value[TCS230_RGB_G] = 117520;
    whiteCalibration.value[TCS230_RGB_B] = 157590;
    
    sensorData blackCalibration;
    blackCalibration.value[TCS230_RGB_R] = 11280;
    blackCalibration.value[TCS230_RGB_G] = 10270;
    blackCalibration.value[TCS230_RGB_B] = 13230;

    ColorSensor.begin();
    ColorSensor.setDarkCal(&blackCalibration);
    ColorSensor.setWhiteCal(&whiteCalibration);

    ledDisplay.shutdown(0, false);
    ledDisplay.setIntensity(0, 10);
    ledDisplay.clearDisplay(0);
}

void loop() {
    colorData rgb;
    ColorSensor.read();
    while (!ColorSensor.available())
        ;
    ColorSensor.getRGB(&rgb);

    updateDisplay(rgb);
}

void updateDisplay(colorData rgb) {
  ledDisplay.clearDisplay(0);

  setValues(rgb.value[TCS230_RGB_R] / 32, 0);
  setValues(rgb.value[TCS230_RGB_G] / 32, 3);
  setValues(rgb.value[TCS230_RGB_B] / 32, 6);
}

void setValues(int value, int column) {
  int width = 2;

  for (int i = 0; i <= value; i++) {
    ledDisplay.setLed(0, 7 - i, column, true);
    ledDisplay.setLed(0, 7 - i, column + width - 1, true);
  }
}
