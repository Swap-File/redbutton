#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>
#include <avr/pgmspace.h>
#include <TimerOne.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 6
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000011, B10000000,
  B00001001, B11100000,
  B00011100, B11101000,
  B00111110, B00001100,
  B00000000, B00001110,
  B01111000, B00001110,
  B01110000, B00001100,
  B11100000, B00001011,
  B11000000, B00000111,
  B00010000, B00001110,
  B00110000, B00011110,
  B01110100, B00000000,
  B00110110, B01111100,
  B00010111, B00111000,
  B00000111, B10010000,
  B00000001, B11000000
};

#define LOGO24_GLCD_HEIGHT 24
#define LOGO24_GLCD_WIDTH  24

static const unsigned char PROGMEM logo24_glcd_bmp[] =
{ B00000000, B00011100, B00000000,
  B00000001, B11001111, B10000000,
  B00000111, B11100111, B11000000,
  B00001111, B11110011, B11010000,
  B00011111, B11111001, B11011000,
  B00000000, B00000000, B11011100,
  B00111110, B00000000, B01011100,
  B01111100, B00000000, B00011110,
  B01111000, B00000000, B00011110,
  B01110000, B00000000, B00011110,
  B11100000, B00000000, B00011100,
  B11001000, B00000000, B00011001,
  B10011000, B00000000, B00010011,
  B00111000, B00000000, B00000111,
  B01111000, B00000000, B00001110,
  B01111000, B00000000, B00011110,
  B01111000, B00000000, B00111110,
  B00111010, B00000000, B01111100,
  B00111011, B00000000, B00000000,
  B00011011, B10011111, B11111000,
  B00001011, B11001111, B11110000,
  B00000011, B11100111, B11100000,
  B00000001, B11110011, B10000000,
  B00000000, B00111000, B00000000
};

#define LOGO36_GLCD_HEIGHT 32
#define LOGO36_GLCD_WIDTH  32

static const unsigned char PROGMEM logo32_glcd_bmp[] =
{ B00000000, B00000011, B11000000, B00000000,
  B00000000, B00111001, B11111100, B00000000,
  B00000000, B11111100, B11111111, B00000000,
  B00000011, B11111110, B01111111, B01000000,
  B00000111, B11111111, B00111111, B01100000,
  B00001111, B11111111, B10011111, B01110000,
  B00011111, B11111111, B11001111, B01111000,
  B00000000, B00000000, B00000111, B01111000,
  B00111111, B11000000, B00000011, B01111100,
  B00111111, B10000000, B00000001, B01111100,
  B01111111, B00000000, B00000000, B01111110,
  B01111110, B00000000, B00000000, B01111110,
  B01111100, B00000000, B00000000, B01111110,
  B01111000, B00000000, B00000000, B01111100,
  B11110010, B00000000, B00000000, B01111001,
  B11100110, B00000000, B00000000, B01110011,
  B11001110, B00000000, B00000000, B01100111,
  B10011110, B00000000, B00000000, B01001111,
  B00111110, B00000000, B00000000, B00011110,
  B01111110, B00000000, B00000000, B00111110,
  B01111110, B00000000, B00000000, B01111110,
  B01111110, B00000000, B00000000, B11111110,
  B00111110, B10000000, B00000001, B11111100,
  B00111110, B11000000, B00000011, B11111100,
  B00011110, B11100000, B00000000, B00000000,
  B00011110, B11110011, B11111111, B11111000,
  B00001110, B11111001, B11111111, B11110000,
  B00000110, B11111100, B11111111, B11100000,
  B00000010, B11111110, B01111111, B11000000,
  B00000000, B11111111, B00111111, B00000000,
  B00000000, B00111111, B10011100, B00000000,
  B00000000, B00000011, B11000000, B00000000
};


const char string_0[] PROGMEM = "If you become light-headed from thirst, feel free to pass out.";
const char string_1[] PROGMEM = "Any appearance of danger is merely a device to enhance your testing experience.";
const char string_2[] PROGMEM = "When the testing is over, you will be missed.";
const char string_3[] PROGMEM = "Cake and grief counseling will be available at the conclusion of the test.";
const char string_4[] PROGMEM = "Thank you for helping us help you help us all.";
const char string_5[] PROGMEM = "The experiment is nearing its conclusion.";
const char string_6[] PROGMEM = "Button-based testing remains an important tool for science.";
const char string_7[] PROGMEM = "For your testing convenience, all button related safety precautions have been deactivated.";
const char string_8[] PROGMEM = "The Enrichment Center reminds you that the Button will never threaten to stab you.";
const char string_9[] PROGMEM = "Prolonged exposure to the Button is not part of this test.";
const char string_10[] PROGMEM = "Thanks to Emergency Testing Protocols, testing can continue.";
const char string_11[] PROGMEM = "If you feel liquid running down your neck, relax, lie on your back, and apply immediate pressure to your temples.";
const char string_12[] PROGMEM = "Because of the technical difficulties we are currently experiencing, your test environment is unsupervised.";
const char string_13[] PROGMEM = "Some emergency testing may require prolonged interaction with lethal military androids.";
const char string_14[] PROGMEM = "Testing is the future, and the future starts with you.";
const char string_15[] PROGMEM = "To ensure that sufficient power remains for core testing protocols, all safety devices have been disabled.";
const char string_16[] PROGMEM = "For your privacy, test results are not being relayed via satelite to long term storage for future review.";

const char* const string_table[] PROGMEM = {string_0, string_1, string_2, string_3, string_4, string_5,  string_6, string_7, string_8, string_9, string_10, string_11, string_12, string_13, string_14, string_15, string_16};

int8_t icons[NUMFLAKES][3];//snowflake positions

char temp_buffer[150];   //ram buffer for messages
uint8_t active_message = 0;  //keep track of which message is active

#define INTERRUPT_PIN 7  //button pin

volatile uint32_t eeprom_updated_time = 0;   //last eeprom update time

volatile boolean idle_mode = true;         //if in idle mode or not
volatile boolean delay_idle_mode_entry = true;  //delay idle mode entry until at least one message has been shown

volatile uint32_t button_presses = 0;  //actual count of button presses

volatile uint32_t old_press_time = 0;     //time of previous button press
volatile uint32_t new_press_time = 0;     //time of this button press
volatile uint16_t clicks_per_minute = 0;  //calculated clicks per minute
uint16_t clicks_per_minute_max = 0;       //max rating of clicks per minute during the last message cycle

void display_reinit() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  display.clearDisplay();
  display.display();
  display.setTextSize(2);
  display.setTextColor(WHITE);
}

void setup()   {

  button_presses = ReadLongEEPROM(0);
  load_message(active_message);

  Serial.begin(115200);


  // initialize
  for (uint8_t f = 0; f < NUMFLAKES; f++) {
    icons[f][XPOS] = random(display.width());
    icons[f][YPOS] = 0;
    icons[f][DELTAY] = random(5) + 1;

    Serial.print("x: ");
    Serial.print(icons[f][XPOS], DEC);
    Serial.print(" y: ");
    Serial.print(icons[f][YPOS], DEC);
    Serial.print(" dy: ");
    Serial.println(icons[f][DELTAY], DEC);
  }

  Serial.println(button_presses);


  pinMode(INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), press_button, RISING);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display_reinit();

  Timer1.initialize(150000);
  Timer1.attachInterrupt(housekeeping_function); // blinkLED to run every 0.15 seconds
}


void loop() {

  if (idle_mode) {
    testdrawbitmap(logo24_glcd_bmp, LOGO24_GLCD_HEIGHT, LOGO24_GLCD_WIDTH);
  }
  else {

    uint32_t test_user_time = millis();
    while (millis() - test_user_time < 5000) {

      display.clearDisplay();

      //print counter center padded
      display.setCursor(0, 0);
      //10 digits
      if (button_presses < 10) display.print(' ');
      if (button_presses < 1000) display.print(' ');
      if (button_presses < 100000) display.print(' ');
      if (button_presses < 10000000) display.print(' ');
      if (button_presses < 1000000000) display.print(' ');
      display.print(button_presses);

      display.setCursor(0, 16);
      if (clicks_per_minute_max < clicks_per_minute) clicks_per_minute_max = clicks_per_minute;

      if (clicks_per_minute_max < 50)          display.print(" ");
      else if (clicks_per_minute_max < 100)    display.print("   Slow");
      else  if (clicks_per_minute_max < 220)   display.print("   Fast");
      else                                     display.print("   Turbo");
      display.display();
    }


    if (millis() - new_press_time > 3000 && delay_idle_mode_entry == false)  {
      idle_mode = true;
        display_reinit();
      display.clearDisplay();
      display.display();
    } else {

      clicks_per_minute_max = 0;
      delay_idle_mode_entry = true;
      for (int i = 0; i < (strlen(temp_buffer) * 6) + 128; i++) {

        display.clearDisplay();

        //print counter center padded
        display.setCursor(0, 0);
        //10 digits
        if (button_presses < 10) display.print(' ');
        if (button_presses < 1000) display.print(' ');
        if (button_presses < 100000) display.print(' ');
        if (button_presses < 10000000) display.print(' ');
        if (button_presses < 1000000000) display.print(' ');
        display.print(button_presses);

        //print scrolling text
        display.setCursor(128 - (i * 2), 16);
        display.print(temp_buffer);

        display.display();
      }

      active_message++;
      if (active_message > 16) active_message = 0;
      load_message(active_message);

      display.clearDisplay();
      delay_idle_mode_entry = false;

    }
  }

}

void housekeeping_function(void) {

  clicks_per_minute = clicks_per_minute * .8;  //decay the cpm

  if (millis() - eeprom_updated_time > 60000 ) {
    Serial.println(F("Saving"));
    WriteLongEEPROM(0, button_presses);
    eeprom_updated_time = millis();
  }

}

void press_button() {
  button_presses++;
  idle_mode = false;
  delay_idle_mode_entry = true;
  old_press_time = new_press_time;
  new_press_time = millis();
  clicks_per_minute = (clicks_per_minute * .9) +  ((((uint32_t)60000 ) / (new_press_time - old_press_time)) * .1);
}

void testdrawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h) {

  // draw each icon
  for (uint8_t f = 0; f < NUMFLAKES; f++) {
    display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, WHITE);
  }
  display.display();
  delay(100);

  // then erase it + move it
  for (uint8_t f = 0; f < NUMFLAKES; f++) {
    display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, BLACK);
    // move it
    icons[f][YPOS] += icons[f][DELTAY];
    // if its gone, reinit
    if (icons[f][YPOS] > display.height()) {
      icons[f][XPOS] = random(display.width() / 12) * 12;
      icons[f][YPOS] = -h;
      icons[f][DELTAY] = random(5) + 1;
    }
  }

}

void WriteLongEEPROM(byte addr, unsigned long data) {
  EEPROM.update(addr + 0, (byte)((data >> 24) & 0xff));
  EEPROM.update(addr + 1, (byte)((data >> 16) & 0xff));
  EEPROM.update(addr + 2, (byte)((data >> 8) & 0xff));
  EEPROM.update(addr + 3, (byte)(data & 0xff));
}

uint32_t ReadLongEEPROM(byte addr) {
  uint32_t result;
  result = ((uint32_t)EEPROM.read(addr + 0)) << 24;
  result |= ((uint32_t)EEPROM.read(addr + 1)) << 16;
  result |= ((uint32_t)EEPROM.read(addr + 2)) << 8;
  result |= ((uint32_t)EEPROM.read(addr + 3));
  return result;
}

void load_message(int i) {
  strcpy_P(temp_buffer, (char*)pgm_read_word(&(string_table[i]))); // Necessary casts and dereferencing, just copy.
  Serial.println(temp_buffer);
}
