#ifndef RGB_DISPLAY_H
#define RGB_DISPLAY_H

#include "common.h"
#include <Adafruit_GFX.h>     // Core graphics library
#if defined(DISPLAY_IS_ST7789V)
  #include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#elif defined(DISPLAY_IS_ST7735)
  #include <Adafruit_ST7735.h>  // Hardware-specific library for ST7735
#elif defined(DISPLAY_IS_ILI9341)
  #include "Adafruit_ILI9341.h"
#elif defined(DISPLAY_IS_ILI9488)
  #include "ILI9488_t3.h"   // Teensy Hardware DMA accelerated library
#endif
#include <Fonts/ComingSoon_Regular70pt7b.h>   // from https://fonts.google.com/ and converted using https://rop.nl/truetype2gfx/
#include <Fonts/FreeSansBold48pt7b.h>
#include <Fonts/Satisfy_Regular24pt7b.h>     // from https://fonts.google.com/ and converted using https://rop.nl/truetype2gfx/
#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/FreeSans24pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/Satisfy_Regular18pt7b.h>     // from https://fonts.google.com/ and converted using https://rop.nl/truetype2gfx/
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeMono9pt7b.h>
#include <SPI.h>
#include <elapsedMillis.h>
#include <avr/pgmspace.h>


class RGBDisplay {

public:

// PUBLIC FUNCTIONS

  // screens
  void DisplayTimeUpdate();
  void Screensaver();
  void GoodMorningScreen();
  void SetAlarmScreen(bool process_user_input);
  void AlarmTriggeredScreen(bool first_time, int8_t button_press_seconds_counter);
  void DisplayWeatherInfo();
  void SettingsPage();
  void WiFiSettingsPage();
  bool GetUserOnScreenTextInput(char* label, char* return_text);

  // functions
  void Setup();
  void SetBrightness(int brightness);
  void SetMaxBrightness();
  void CheckTimeAndSetBrightness();
  void ScreensaverControl(bool turnOn);
  ScreenPage ClassifyUserScreenTouchInput();

// PUBLIC VARIABLES

  // display object
  #if defined(DISPLAY_IS_ST7789V)
    // Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_COPI, TFT_CLK, TFT_RST);
    Adafruit_ST7789 tft = Adafruit_ST7789(&SPI, TFT_CS, TFT_DC, TFT_RST);   // when multiple SPI Peripherals are present then only this works
  #elif defined(DISPLAY_IS_ST7735)
    // For 1.8" TFT with ST7735 using Hardware VSPI Pins COPI and SCK:
    Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
  #elif defined(DISPLAY_IS_ILI9341)
    Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_COPI, TFT_CLK, TFT_RST, TFT_CIPO);
  #elif defined(DISPLAY_IS_ILI9488)
    // Use hardware SPI (#13, #12, #11) and the above for CS/DC
    ILI9488_t3 tft = ILI9488_t3(&SPI, TFT_CS, TFT_DC, TFT_RST, TFT_COPI, TFT_CLK, TFT_CIPO);
  #endif

  // redraw full display flag
  bool redraw_display_ = false;

  // refresh screensaver canvas
  bool refresh_screensaver_canvas_ = true;

  // tft dimensions
  const uint16_t kTftWidth = 320, kTftHeight = 240;


private:

// PRIVATE FUNCTIONS

  void DrawSun(int16_t x0, int16_t y0, uint16_t edge);
  void DrawRays(int16_t &cx, int16_t &cy, int16_t &rr, int16_t &rl, int16_t &rw, uint8_t &rn, int16_t &degStart, uint16_t &color);
  void DrawDenseCircle(int16_t &cx, int16_t &cy, int16_t r, uint16_t &color);
  void PickNewRandomColor();  // for screensaver
  void DrawButton(int16_t x, int16_t y, uint16_t w, uint16_t h, const char* label, uint16_t borderColor, uint16_t onFill, uint16_t offFill, bool isOn);
  void DrawTriangleButton(int16_t x, int16_t y, uint16_t w, uint16_t h, bool isUp, uint16_t borderColor, uint16_t fillColor);
  void FastDrawBitmap(int16_t x, int16_t y, uint8_t* bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg);
  // keyboard functions
  void MakeKeyboard(const char type[][13], char* label);
  void DrawKeyboardButton(int x, int y, int w, int h);
  byte IsTouchWithin(int x, int y, int w, int h);
  bool GetKeyboardPress(char * textBuffer, char* label, char * textReturn);


// PRIVATE VARIABLES

  // current screen brightness
  int current_brightness_ = 0;

  // screensaver
  bool screensaver_move_down_ = true, screensaver_move_right_ = true;
  int current_random_color_index_ = 0;
  GFXcanvas1* my_canvas_ = NULL;
  bool show_colored_edge_screensaver_ = true;
  bool fly_screensaver_horizontally_ = false;

  // location of various display text strings
  int16_t gap_right_x_ = 0, gap_up_y_ = 0;
  int16_t tft_HHMM_x0_ = kTimeRowX0, tft_HHMM_y0_ = 2 * kTimeRowY0;
  uint16_t tft_HHMM_w_ = 0, tft_HHMM_h_ = 0;
  int16_t screensaver_x1_ = 0, screensaver_y1_ = 0;
  uint16_t screensaver_w_ = 0, screensaver_h_ = 0;
  int16_t tft_AmPm_x0_ = 0, tft_AmPm_y0_ = 0;
  int16_t tft_SS_x0_ = 0;
  int16_t date_row_x0_ = 0;
  int16_t alarm_row_x0_ = 0;
  int16_t alarm_icon_x0_ = 0, alarm_icon_y0_ = 0;


// PRIVATE CONSTANTS
  
  // display brightness constants
  const int kNightBrightness = 1;
  const int kEveningBrightness = 100;
  const int kDayBrightness = 150;
  const int kMaxBrightness = 255;

  // user defined locations of various text strings on display
  const int16_t kTimeRowX0 = 10, kTimeRowY0 = 80, kAM_PM_row_Y0 = 45;
  const int16_t kDisplayTextGap = 5;
  const int16_t kDateRow_Y0 = 140;
  const int16_t kAlarmRowY0 = 210, kAlarmRowY1 = 160;
  const int16_t kRadiusButtonRoundRect = 5;
  const char cancelStr[2] = "X", wifiSettingsStr[5] = "WiFi", ssidStr[5] = "SSID", passwdStr[7] = "PASSWD", weatherStr[8] = "WEATHER", updateTimeStr[12] = "UPDATE TIME";
  const int16_t kCancelButtonSize = 40, kCancelButtonX1 = kTftWidth - kCancelButtonSize, kCancelButtonY1 = kTftHeight - kCancelButtonSize;
  const int16_t kWiFiSettingsButtonX1 = 200, kWiFiSettingsButtonY1 = 0, kWiFiSettingsButtonW = 100, kWiFiSettingsButtonH = 25;
  const int16_t kSsidButtonX1 = 100, kSsidButtonY1 = 0, kSsidButtonW = 80, kSsidButtonH = 25;
  const int16_t kPasswdButtonX1 = 200, kPasswdButtonY1 = 0, kPasswdButtonW = 120, kPasswdButtonH = 25;
  const int16_t kWeatherButtonX1 = 0, kWeatherButtonY1 = 80, kWeatherButtonW = 130, kWeatherButtonH = 25;
  const int16_t kUpdateTimeButtonX1 = 150, kUpdateTimeButtonY1 = 80, kUpdateTimeButtonW = 170, kUpdateTimeButtonH = 25;

  // color definitions
  const uint16_t  kDisplayColorBlack        = 0x0000;
  const uint16_t  kDisplayColorBlue         = 0x001F;
  const uint16_t  kDisplayColorRed          = 0xF800;
  const uint16_t  kDisplayColorOrange       = 0xfca0;
  const uint16_t  kDisplayColorGreen        = 0x07E0;
  const uint16_t  kDisplayColorCyan         = 0x07FF;
  const uint16_t  kDisplayColorMagenta      = 0xF81F;
  const uint16_t  kDisplayColorYellow       = 0xFFE0;
  const uint16_t  kDisplayColorWhite        = 0xFFFF;
  
  // https://github.com/newdigate/rgb565_colors
  #define RGB565_Argentinian_blue                                            		0x6D9D         // Argentinian Blue                        	#6CB4EE			https://en.wikipedia.org/wiki/Shades_of_azure#Argentinian_blue
  #define RGB565_Light_sky_blue                                              		0x867E         // Light Sky Blue                          	#87CEFA			https://en.wikipedia.org/wiki/Shades_of_azure#Light_sky_blue
  #define RGB565_Blue_violet                                                 		0x897B         // Blue-violet                             	#8A2BE2			https://en.wikipedia.org/wiki/Indigo#Deep_indigo_(web_color_blue-violet)
  #define RGB565_Vivid_sky_blue                                              		0x065F         // Vivid sky blue                          	#00CCFF			https://en.wikipedia.org/wiki/Sky_blue#Vivid_sky_blue
  #define RGB565_Beige                                                       		0xF7BB         // Beige                                   	#F5F5DC			https://en.wikipedia.org/wiki/Shades_of_brown#Beige
  #define RGB565_Buff                                                        		0xDD0D         // Buff                                    	#DAA06D			https://en.wikipedia.org/wiki/Shades_of_brown#Buff
  #define RGB565_Sandy_brown                                                 		0xF52C         // Sandy Brown                             	#F4A460			https://en.wikipedia.org/wiki/Shades_of_brown#Sandy_brown
  #define RGB565_Cyan                                                        		0x07FF         // Cyan                                    	#00FFFF			https://en.wikipedia.org/wiki/Shades_of_cyan#Cyan
  #define RGB565_Turquoise                                                   		0x46F9         // Turquoise                               	#40E0D0			https://en.wikipedia.org/wiki/Shades_of_cyan#Turquoise
  #define RGB565_Puce                                                        		0xCC53         // Puce                                    	#CC8899			https://en.wikipedia.org/wiki/Shades_of_gray#Puce
  #define RGB565_Bright_green                                                		0x67E0         // Bright green                            	#66FF00			https://en.wikipedia.org/wiki/Shades_of_green#Bright_green
  #define RGB565_Caribbean_green                                             		0x0653         // Caribbean green                         	#00CC99			https://en.wikipedia.org/wiki/Spring_green#Caribbean_green
  #define RGB565_Electric_green                                              		0x07E0         // Electric green                          	#00FF00			https://en.wikipedia.org/wiki/Electric_green
  #define RGB565_Green_yellow                                                		0xAFE6         // Green-yellow                            	#ADFF2F			https://en.wikipedia.org/wiki/Shades_of_green#Yellow-green
  #define RGB565_Magenta                                                     		0xF81F         // Magenta                                 	#FF00FF			https://en.wikipedia.org/wiki/Shades_of_magenta#Magenta
  #define RGB565_Shocking_pink                                               		0xF897         // Shocking Pink                           	#FC0FC0			https://en.wikipedia.org/wiki/Shades_of_magenta#Shocking_pink
  #define RGB565_Apricot                                                     		0xFE76         // Apricot                                 	#FBCEB1			https://en.wikipedia.org/wiki/Shades_of_orange#Apricot
  #define RGB565_Atomic_tangerine                                            		0xFCCC         // Atomic tangerine                        	#FF9966			https://en.wikipedia.org/wiki/Shades_of_orange#Atomic_tangerine
  #define RGB565_Dark_orange                                                 		0xFC60         // Dark orange                             	#FF8C00			https://en.wikipedia.org/wiki/Shades_of_orange#Dark_orange
  #define RGB565_Orange                                                      		0xFBE0         // Orange                                  	#FF7F00			https://en.wikipedia.org/wiki/Shades_of_orange#Orange
  #define RGB565_Tart_orange                                                 		0xFA69         // Tart Orange                             	#FB4D46			https://en.wikipedia.org/wiki/List_of_Crayola_crayon_colors#Heads_n_Tails
  #define RGB565_Light_deep_pink                                             		0xFAF9         // Light Deep Pink                         	#FF5CCD			https://en.wikipedia.org/wiki/Shades_of_pink#Light_deep_pink
  #define RGB565_Pink_flamingo                                               		0xFBBF         // Pink flamingo                           	#FC74FD			https://en.wikipedia.org/wiki/Manatee_(color)
  #define RGB565_Electric_purple                                             		0xB81F         // Electric Purple                         	#BF00FF			https://en.wikipedia.org/wiki/Shades_of_purple#Electric_purple
  #define RGB565_Purple_x11                                                  		0x991D         // Purple (X11)                            	#A020F0			https://en.wikipedia.org/wiki/Shades_of_purple#Purple_(X11_color)_(veronica)
  #define RGB565_Candy_apple_red                                             		0xF840         // Candy apple red                         	#FF0800			https://en.wikipedia.org/wiki/Candy_apple_red
  #define RGB565_Red_rgb                                                     		0xF800         // Red (RGB)                               	#FF0000			https://en.wikipedia.org/wiki/Shades_of_red#Red_rgb
  #define RGB565_Tomato                                                      		0xFB09         // Tomato                                  	#FF6347			https://en.wikipedia.org/wiki/Shades_of_red#Tomato
  #define RGB565_Ivory                                                       		0xFFFD         // Ivory                                   	#FFFFF0			https://en.wikipedia.org/wiki/Shades_of_white#Ivory
  #define RGB565_Chartreuse_web                                              		0x7FE0         // Chartreuse web                          	#7FFF00			https://en.wikipedia.org/wiki/Chartreuse_(color)
  #define RGB565_Golden_yellow                                               		0xFEE0         // Golden yellow                           	#FFDF00			https://en.wikipedia.org/wiki/Gold_(color)#Golden_yellow
  #define RGB565_Yellow_rgb_x11_yellow                                       		0xFFE0         // Yellow (RGB) (X11 yellow)               	#FFFF00			https://en.wikipedia.org/wiki/Shades_of_yellow#Yellow_rgb_x11_yellow
  #define RGB565_Lime_color_wheel                                            		0xBFE0         // Lime (color wheel)                      	#BFFF00			https://en.wikipedia.org/wiki/Lime_(color)

  constexpr static uint8_t kColorPickerWheelSize = 33;
  const uint16_t kColorPickerWheel[kColorPickerWheelSize] = {0x6D9D, 0x867E, 0x897B, 0x065F, 0xF7BB, 0xDD0D, 0xF52C, 0x07FF, 0x46F9, 0xCC53, 0x67E0, 0x0653, 0x07E0, 0xAFE6, 0xF81F, 0xF897, 0xFE76, 0xFCCC, 0xFC60, 0xFBE0, 0xFA69, 0xFAF9, 0xFBBF, 0xB81F, 0x991D, 0xF840, 0xF800, 0xFB09, 0xFFFD, 0x7FE0, 0xFEE0, 0xFFE0, 0xBFE0};

  // The colors we actually want to use
  const uint16_t        kDisplayTimeColor         = kDisplayColorYellow;
  const uint16_t        kDisplayDateColor         = kDisplayColorGreen;
  const uint16_t        kDisplayAlarmColor        = kDisplayColorCyan;
  const uint16_t        kDisplayBackroundColor    = kDisplayColorBlack;
  // keyboard colors
  const uint16_t  kTextRegularColor          = kDisplayColorWhite;
  const uint16_t  kTextHighLightColor        = kDisplayBackroundColor;
  const uint16_t  kKeyboardButtonFillColor   = kDisplayColorRed;

  // BIG BELL ICONS

  const uint8_t kBellWidth = 114, kBellHeight = 75;
  // 'bell_114x75, 114x75px
  // convert image into binary monochrome using https://javl.github.io/image2cpp/
  const unsigned char kBellBitmap[1125] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x3f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x01, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x07, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x1f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 
    0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 
    0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 
    0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 
    0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xe0, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x03, 0x80, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x07, 
    0xe0, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x07, 0xf8, 
    0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x07, 0xfe, 0x00, 
    0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x3f, 0xf0, 0x00, 0x03, 0xff, 0xc0, 0x00, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x01, 0xff, 0xe0, 0x00, 0x00, 0xff, 0xf0, 0x00, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x07, 0xff, 0x80, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0x80, 0x1f, 0xfe, 0x00, 0x00, 0x00, 0x0f, 0xff, 0x00, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0x80, 0x7f, 0xf8, 0x00, 0x00, 0x00, 0x03, 0xff, 0x80, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0x80, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0xff, 0x81, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xc0, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x81, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xc0, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
    0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfc, 0x03, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xe0, 0x1f, 0xf0, 0x00, 0x00, 0x1f, 0xff, 0xfe, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xf0, 0x3f, 0xff, 0xfc, 0x00, 0x3f, 0xff, 0xfe, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xf0, 0x3f, 0xff, 0xfe, 0x00, 0x3f, 0xff, 0xfe, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 
    0x3f, 0xff, 0xfe, 0x00, 0x3f, 0xff, 0xfc, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x1f, 
    0xff, 0xfe, 0x00, 0x1f, 0xf8, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x0f, 
    0xfc, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x0e, 
    0x00, 0x00, 0x00, 0x00, 0x01, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xc0, 
    0x00, 0x00, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x1f, 0xf8, 0x00, 
    0x00, 0x00, 0x7f, 0xfc, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x1f, 0xff, 0x00, 0x00, 
    0x03, 0xff, 0xf8, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x0f, 0xff, 0xe0, 0x00, 0x1f, 
    0xff, 0xc0, 0x00, 0x00, 0x07, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x01, 0xff, 0xfc, 0x00, 0x3f, 0xfe, 
    0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x3f, 0xfe, 0x00, 0x3f, 0xf0, 0x00, 
    0x00, 0x00, 0x03, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x3f, 0x80, 0x00, 0x00, 
    0x00, 0x01, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x3f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 
    0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00
  };

  const uint8_t kBellFallenWidth = 75, kBellFallenHeight = 71;
  // 'bell_fallen_75x71, 75x71px
  const unsigned char kBellFallenBitmap[710] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xf0, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 
    0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x1f, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xfc, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
    0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 
    0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x0f, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0x7c, 
    0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x80, 0x00, 0x00, 0x03, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xe0, 0x00, 
    0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xf0, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xf8, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xfc, 0x00, 0x00, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xfc, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xfe, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0xfe, 0x00, 0x03, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xef, 0xfe, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 
    0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xff, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xf7, 0xff, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x0f, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfb, 0xff, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfb, 
    0xfe, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x1f, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xfd, 0xfe, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xfc, 0x00, 
    0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xfe, 0xf8, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xf0, 0x00, 0x3f, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0x40, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 
    0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0x80, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x0f, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xc0, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x07, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 
    0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xf0, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x7f, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xf0, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x0f, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 
    0x00, 0x03, 0xff, 0xff, 0xff, 0xfe, 0x07, 0xff, 0xe0, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x80, 
    0x00, 0x1f, 0xc0, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x03, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

  // SMALL BELL ICONS

  const uint8_t kBellSmallWidth = 70, kBellSmallHeight = 46;
  // 'bell_114x75', 70x46px
  const unsigned char kBellSmallBitmap [414] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff,
    0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xfe, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x07, 0xff, 0xff, 0xff,
    0x00, 0x03, 0xc0, 0x1f, 0x80, 0x0f, 0xff, 0xff, 0xff, 0x80, 0x0f, 0xc0, 0x1f, 0xe0, 0x0f, 0xff,
    0xff, 0xff, 0x80, 0x3f, 0xc0, 0x07, 0xfc, 0x0f, 0xff, 0xff, 0xff, 0x80, 0xff, 0x00, 0x01, 0xfe,
    0x0f, 0xff, 0xff, 0xff, 0x83, 0xfc, 0x00, 0x00, 0x7f, 0x0f, 0xff, 0xff, 0xff, 0xc3, 0xf8, 0x00,
    0x00, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xc3, 0xc0, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xc0,
    0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff,
    0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x1f,
    0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00,
    0x00, 0x1f, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0xfc, 0x1f, 0xff, 0xff, 0xff, 0xe0, 0xf8,
    0x00, 0x3f, 0xfc, 0x1f, 0xff, 0xff, 0xff, 0xe1, 0xff, 0xf0, 0x7f, 0xfc, 0x3f, 0xff, 0xff, 0xff,
    0xe1, 0xff, 0xf0, 0x7f, 0xfc, 0x3f, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xf0, 0x00, 0x00, 0x7f, 0xff,
    0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00,
    0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xfe,
    0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff,
    0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x7c, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x03, 0xfc, 0x00, 0x1f, 0xff, 0xc0, 0x00, 0xff, 0x00, 0x1f,
    0xf8, 0x00, 0x1f, 0xff, 0xc0, 0x00, 0xff, 0xc0, 0x7f, 0xe0, 0x00, 0x0f, 0xff, 0xc0, 0x00, 0x3f,
    0xf0, 0x7f, 0x00, 0x00, 0x0f, 0xff, 0x80, 0x00, 0x03, 0xf0, 0x78, 0x00, 0x00, 0x03, 0xff, 0x00,
    0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x01, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

  const uint8_t kBellFallenSmallWidth = 49, kBellFallenSmallHeight = 46;
  // 'bell_fallen_75x71', 49x46px
  const unsigned char kBellFallenSmallBitmap [322] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f,
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xc0, 0x00,
    0x00, 0x00, 0x00, 0x01, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xe0, 0x00, 0x00, 0x00,
    0x00, 0x07, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0xff,
    0xff, 0xf0, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xf7,
    0x80, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xf7, 0xc0, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00,
    0x01, 0xff, 0xff, 0xff, 0xfb, 0xf8, 0x00, 0x03, 0xff, 0xff, 0xff, 0xfb, 0xf8, 0x00, 0x07, 0xff,
    0xff, 0xff, 0xff, 0xfc, 0x00, 0x07, 0xff, 0xff, 0xff, 0xfd, 0xfc, 0x00, 0x0f, 0xff, 0xff, 0xff,
    0xfd, 0xfc, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xfe, 0xfc,
    0x00, 0x1f, 0xff, 0xff, 0xff, 0xfe, 0xfc, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x3f,
    0xff, 0xff, 0xff, 0xff, 0x78, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0x78, 0x00, 0x3f, 0xff, 0xff,
    0xff, 0xff, 0xf0, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xa0, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff,
    0x80, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00,
    0x1f, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x1f, 0xff,
    0xff, 0xff, 0xff, 0xe0, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x07, 0xff, 0xff, 0xff,
    0xff, 0xe0, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xf0,
    0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00,
    0x3f, 0xff, 0xf8, 0x0f, 0xe0, 0x00, 0x00, 0x1f, 0xff, 0x80, 0x00, 0x80, 0x00, 0x00, 0x01, 0xf0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00
  };

  const uint8_t kSettingsGearWidth = 40, kSettingsGearHeight = 40;
  const int16_t kSettingsGearX1 = kTftWidth - kSettingsGearWidth - 10, kSettingsGearY1 = kDateRow_Y0 - kSettingsGearHeight + 5;
  // 'gear-settings-1-48', 40x40px
  const unsigned char kSettingsGearBitmap[200] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 
    0x01, 0xff, 0x00, 0x00, 0x00, 0x01, 0xc3, 0x00, 0x00, 0x00, 0x01, 0x83, 0x80, 0x00, 0x00, 0x01, 
    0x83, 0x80, 0x00, 0x00, 0x03, 0x81, 0x80, 0x00, 0x07, 0x0f, 0x81, 0xe1, 0xe0, 0x0f, 0xff, 0x00, 
    0xff, 0xf0, 0x1d, 0xf8, 0x00, 0x3f, 0xf0, 0x18, 0x30, 0x00, 0x0c, 0x38, 0x38, 0x00, 0x38, 0x00, 
    0x18, 0x30, 0x00, 0xff, 0x00, 0x1c, 0x78, 0x03, 0xff, 0x80, 0x1c, 0x3c, 0x03, 0x81, 0xc0, 0x38, 
    0x1e, 0x07, 0x00, 0xe0, 0x70, 0x07, 0x0e, 0x00, 0x60, 0xe0, 0x07, 0x0c, 0x00, 0x60, 0xc0, 0x03, 
    0x0c, 0x00, 0x70, 0xc0, 0x07, 0x0c, 0x00, 0x70, 0xc0, 0x03, 0x0c, 0x00, 0x70, 0xc0, 0x07, 0x0e, 
    0x00, 0x60, 0xe0, 0x0e, 0x06, 0x00, 0xe0, 0xf0, 0x1c, 0x07, 0x01, 0xc0, 0x78, 0x38, 0x03, 0xc3, 
    0xc0, 0x3c, 0x70, 0x01, 0xff, 0x00, 0x1c, 0x30, 0x00, 0x7e, 0x00, 0x1c, 0x38, 0x00, 0x00, 0x00, 
    0x18, 0x1c, 0xf8, 0x00, 0x1e, 0x38, 0x1f, 0xfc, 0x00, 0x7f, 0xf0, 0x0f, 0xdf, 0x01, 0xf3, 0xe0, 
    0x04, 0x07, 0x81, 0xc0, 0x40, 0x00, 0x01, 0x81, 0x80, 0x00, 0x00, 0x01, 0x83, 0x80, 0x00, 0x00, 
    0x01, 0x83, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 
    0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

  // KEYBOARD

  const char Mobile_KB_Capitals[3][13] PROGMEM = {
    {0, 13, 10, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P'},
    {1, 12, 9, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L'},
    {3, 10, 7, 'Z', 'X', 'C', 'V', 'B', 'N', 'M'},
  };

  const char Mobile_KB_Smalls[3][13] PROGMEM = {
    {0, 13, 10, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p'},
    {1, 12, 9, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l'},
    {3, 10, 7, 'z', 'x', 'c', 'v', 'b', 'n', 'm'},
  };

  const char Mobile_NumKeys[3][13] PROGMEM = {
    {0, 13, 10, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'},
    {0, 13, 10, '-', '/', ':', ';', '(', ')', '$', '&', '@', '"'},
    {5, 8, 5, '.', ',', '?', '!', '\''}
  };

  const char Mobile_SymKeys[3][13] PROGMEM = {
    {0, 13, 10, '[', ']', '{', '}', '#', '%', '^', '*', '+', '='},
    {4, 9, 6, '_', '\\', '|', '~', '<', '>'}, //4
    {5, 8, 5, '.', ',', '?', '!', '\''}
  };

  const int16_t kTextAreaHeight = 100;
  

};

#endif    // RGB_DISPLAY_H