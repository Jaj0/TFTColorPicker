#include <Arduino.h>

#include <TFT_eSPI.h>

#ifndef TFT_DISPOFF
#define TFT_DISPOFF 0x28
#endif

#ifndef TFT_SLPIN
#define TFT_SLPIN   0x10
#endif

uint8_t command_buffer[4];
uint8_t command_len;
uint16_t bg_color = TFT_BLACK;
uint16_t fg_color = TFT_WHITE;
TFT_eSPI tft = TFT_eSPI(135, 240); // Invoke custom library

void update_screen()
{
    tft.fillScreen(bg_color);

    tft.setTextSize(2);
    tft.setCursor(45, 55, 2);
    tft.setTextColor(fg_color, bg_color);
    tft.println("Color Picker");

    // Show real color components on screen
    uint8_t r, g, b;
    r = (bg_color >> 8) & 0xF8; r |= (r >> 5);
    g = (bg_color >> 3) & 0xFC; g |= (g >> 6);
    b = (bg_color << 3) & 0xF8; b |= (b >> 5);
    tft.setTextSize(1);
    tft.setCursor(2, 120, 2);
    tft.printf("BG ");
    tft.printf("R:0x%02X ", r);
    tft.printf("G:0x%02X ", g);
    tft.printf("B:0x%02X", b);

    r = (fg_color >> 8) & 0xF8; r |= (r >> 5);
    g = (fg_color >> 3) & 0xFC; g |= (g >> 6);
    b = (fg_color << 3) & 0xF8; b |= (b >> 5);
    tft.setCursor(2, 105, 2);
    tft.printf("FG ");
    tft.printf("R:0x%02X ", r);
    tft.printf("G:0x%02X ", g);
    tft.printf("B:0x%02X", b);
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Start");

    tft.init();
    tft.setRotation(1);

    if (TFT_BL > 0) { // TFT_BL has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
         pinMode(TFT_BL, OUTPUT); // Set backlight pin to output mode
         digitalWrite(TFT_BL, TFT_BACKLIGHT_ON); // Turn backlight on. TFT_BACKLIGHT_ON has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
    }
    update_screen();
}

void loop()
{
    if(Serial.available()){
        command_len = Serial.readBytesUntil('\r', command_buffer, 4);

        if(command_len == 4) {
            switch (command_buffer[0]) {
                case 0xAA:
                    bg_color = tft.color565(command_buffer[1], command_buffer[2], command_buffer[3]);
                    update_screen();
                    Serial.println("OK");
                    break;
                case 0xBB:
                    fg_color = tft.color565(command_buffer[1], command_buffer[2], command_buffer[3]);
                    update_screen();
                    Serial.println("OK");
                    break;
                default:
                    Serial.println("Unknown command");
                    break;
            }
        }
    }
}
