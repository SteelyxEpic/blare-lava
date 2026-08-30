#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

#define TFT_SCLK 10 // labeled SCL on the screen
#define TFT_MOSI 11 // labeled SDA on the screen
#define TFT_RST 9
#define TFT_DC 5
#define TFT_CS 6
#define TFT_BL 7

#define first_ 3
#define second_ 2
#define third_ 4
#define fourth_ 1

#define buzzer 8

unsigned long temp = 0;
unsigned int sec = 0;
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
bool start = false;
int text_size = 3;
int time = 0;
bool show = false;

void setup() {
  Serial.begin(115200);

  pinMode(first_, INPUT_PULLUP );
  pinMode(second_, INPUT_PULLUP );
  pinMode(third_, INPUT_PULLUP );
  pinMode(fourth_, INPUT_PULLUP );

  pinMode(buzzer, OUTPUT); // Set pin as an output

  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, LOW); 
  
  tft.init(76, 284); // Our panel size (portrait)
  tft.setOffsets(82, 18); // Offsets for the weird resolution
  tft.invertDisplay(false); // Invert the colors (This display is flipped from normal)
  tft.setRotation(1); // Landscape, if it's upside down use 3!
  clearboard();
  Serial.println("TFT Initialized!");

  tft.setTextColor(ST77XX_WHITE);
  tft.print("Welcome to blare: lava"); // Show whatever you want! Draws from the top left of the text/number/shape 
}

void loop() {
  if ((digitalRead(first_) == LOW || digitalRead(second_) == LOW || digitalRead(third_) == LOW || digitalRead(fourth_) == LOW) && !start){
    tft.fillScreen(ST77XX_BLACK); 
  tft.setCursor(0,0);
    tft.println("please choose text size");
    tft.print("1 = increase, 2 = decrease, 3 = enter, 4 = reset");
    while(true){
      if(digitalRead(first_) == LOW){
        if (text_size < 10) text_size++;
        clearboard();
    tft.println("please choose text size");
    tft.print("1 = increase, 2 = decrease, 3 = enter, 4 = reset");
    }else if(digitalRead(second_) == LOW){
      if (text_size > 1) text_size--;
    clearboard();
    tft.println("please choose text size");
    tft.print("1 = increase, 2 = decrease, 3 = enter, 4 = reset");
    }else if(digitalRead(third_) == LOW){
      break;
    }else if(digitalRead(fourth_) == LOW){
      text_size = 3;
    clearboard();
    tft.println("please choose text size");
    tft.print("1 = increase, 2 = decrease, 3 = enter, 4 = reset");
    }
    while(digitalRead(first_) == LOW || digitalRead(second_) == LOW || digitalRead(third_) == LOW || digitalRead(fourth_) == LOW){
      
  tone(buzzer, 1000); // Play 1000 Hz tone
  delay(100);           // Wait for 1 second
  noTone(buzzer);     // Stop the sound
    }
    }
    clearboard();
    tft.println("please select current time");
    tft.print("press any button to continue");
    while(digitalRead(first_) && digitalRead(second_) && digitalRead(third_) && digitalRead(fourth_)){delay(1); }
    updatetime();
  while(true){
      if(digitalRead(first_) == LOW){
        time += 1;
        updatetime();
    }else if(digitalRead(second_) == LOW){
      time -= 1;
    updatetime();
    }else if(digitalRead(third_) == LOW){
      break;
    }else if(digitalRead(fourth_) == LOW){
      time += 60;
    updatetime();
    }
    while(digitalRead(first_) == LOW || digitalRead(second_) == LOW || digitalRead(third_) == LOW || digitalRead(fourth_) == LOW){
      
  tone(buzzer, 1000); // Play 1000 Hz tone
  delay(100);           // Wait for 1 second
  noTone(buzzer);     // Stop the sound
    }
    }
    temp = millis();
    start = true;
  }
  if (start){
if (millis() - temp >= 1000) {
temp += 1000;
show = !show;
sec++;
if (sec % 60 == 0){
time++;}
updatetime();
}
  }
}
void updatetime(){
  clearboard();
  while(time < 0) time += 1440;
  while(time >= 1440) time -= 1440;
  if(time / 60 < 10) tft.print("0");
  tft.print(time / 60);
  if(show)tft.print(":");else tft.print(" ");
  if ((time % 60) < 10)
  tft.print("0");
  tft.print(time % 60);
  
}
void clearboard(){
  tft.setTextSize(text_size);
  tft.fillScreen(ST77XX_BLACK); 
  tft.setCursor(0,0);

}
