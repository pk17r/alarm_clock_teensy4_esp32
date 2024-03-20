#include "rgb_display.h"
#include "alarm_clock.h"
#include "rtc.h"

void RGBDisplay::Setup() {

  /* INITIALIZE DISPLAYS */

  // tft display backlight control PWM output pin
  pinMode(TFT_BL, OUTPUT);

#if defined(DISPLAY_IS_ST7789V)

  // OR use this initializer (uncomment) if using a 2.0" 320x240 TFT:
  // tft.init(TFT_HEIGHT, TFT_WIDTH);           // Init ST7789 320x240
  uint32_t SPI_Speed = 80000000;
  tft.init(kTftHeight, kTftWidth);           // Init ST7789 320x240
  tft.setSPISpeed(SPI_Speed);
  tft.invertDisplay(false);
  // make display landscape orientation
  #if defined(TOUCHSCREEN_IS_XPT2046)   // touchscreen version display has things rotated 180 deg
    tft.setRotation(3);
  #else
    tft.setRotation(1);
  #endif

#elif defined(DISPLAY_IS_ST7735)

  // Use this initializer if using a 1.8" ST7735 TFT screen:
  tft.initR(INITR_BLACKTAB);  // Init ST7735 chip, black tab
  // set col and row offset of display for ST7735S
  tft.setColRowStart(2, 1);
  // make display landscape orientation
  tft.setRotation(1);

#elif defined(DISPLAY_IS_ILI9341)

  // Use this initializer if using a 1.8" ILI9341 TFT screen:
  tft.begin();
  // make display landscape orientation
  tft.setRotation(1);

#elif defined(DISPLAY_IS_ILI9488)

  tft.begin();
  tft.setRotation(tft.getRotation() + 2);
  int16_t x, y;
  tft.getOrigin(&x, &y);
  x = 0, y = 0;
  tft.setOrigin(x, y);
  tft.setClipRect();
  tft.setFontAdafruit();
  tft.invertDisplay(true);

#endif

  // SPI speed defaults to SPI_DEFAULT_FREQ defined in the library, you can override it here
  // Note that speed allowable depends on chip and quality of wiring, if you go too fast, you
  // may end up with a black screen some times, or all the time.
  // tft.setSPISpeed(80000000);

  // clear screen
  tft.fillScreen(kDisplayColorBlack);
  tft.setTextWrap(false);

  // update TFT display
  DisplayTimeUpdate();


  // configure Photoresistor pin
  pinMode(PHOTORESISTOR_PIN, INPUT);
  analogReadResolution(kAdcResolutionBits);

  // set display brightness
  CheckPhotoresistorAndSetBrightness();

  PrintLn("Display Initialized!");
}

// set display brightness function
void RGBDisplay::SetBrightness(int brightness) {
  if(current_brightness_ != brightness) {
    analogWrite(TFT_BL, brightness);
    PrintLn("Display Brightness set to ", brightness);
  }
  current_brightness_ = brightness;
  show_colored_edge_screensaver_ = (brightness >= kBrightnessBackgroundColorThreshold);
}

void RGBDisplay::SetMaxBrightness() {
  if(current_brightness_ != kMaxBrightness)
    SetBrightness(kMaxBrightness);
}

void RGBDisplay::CheckPhotoresistorAndSetBrightness() {
  int photodiode_light_raw = analogRead(PHOTORESISTOR_PIN);
  int lcd_brightness_val = photodiode_light_raw * kBrightnessInactiveMax / kPhotodiodeLightRawMax;
  lcd_brightness_val = max((lcd_brightness_val / 10) * 10, 1);
  // Serial.printf("photodiode_light_raw = %d %0.2f%, lcd_brightness_val = %d\n", photodiode_light_raw, 100.0 * photodiode_light_raw / kPhotodiodeLightRawMax, lcd_brightness_val);
  SetBrightness(lcd_brightness_val);
}

void RGBDisplay::ScreensaverControl(bool turnOn) {
  if(!turnOn && my_canvas_ != NULL) {
    // delete screensaverCanvas;
    delete my_canvas_;
    my_canvas_ = NULL;
  }
  else
    refresh_screensaver_canvas_ = true;
  // clear screen
  tft.fillScreen(kDisplayColorBlack);
  screensaver_x1_ = 0;
  screensaver_y1_ = 20;
  redraw_display_ = true;
  PrepareTimeDayDateArrays();
}
