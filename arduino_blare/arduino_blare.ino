#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <map>
#include <vector>

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

struct TextItem {
int x;
int y;
int w;
};
std::map<String, TextItem> Words;
unsigned long temp = 0;
unsigned long texttemp = 0;
unsigned int sec = 0;

bool start = false;
int text_size = 3;
int temptime = 0;
bool show = false;
int offsety = 0;
String timetext = "";
TextItem timevalues= {0, 0, 0};
bool showtime = false;

int alarm = 0;
bool alarmset = false;

// Fix setColRowStart() by exposing it via a subclass
class MyST7789 : public Adafruit_ST7789 {
public:
  MyST7789(int8_t cs, int8_t dc, int8_t mosi, int8_t sclk, int8_t rst)
    : Adafruit_ST7789(cs, dc, mosi, sclk, rst) {}
  void setOffsets(uint8_t col, uint8_t row) {
    _colstart = _colstart2 = col;
    _rowstart = _rowstart2 = row;
  }
};

MyST7789 tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void updatetext(){
  clearboard();
  for (auto& element : Words) {
    String text = element.first;
    TextItem& values = element.second;
  if (values.w + values.x > tft.width()){
    values.x--;
  }else{
    values.x = 0;
  }
    tft.setCursor(values.x, values.y);
    tft.print(text);


  }
}
void showtimetext(){
  clearboard();
    String text = timetext;
    
  if (timevalues.w + timevalues.x > tft.width()){
    timevalues.x--;
  }else{
    timevalues.x = 0;
  }
    tft.setCursor(timevalues.x, timevalues.y);
    tft.print(text);
}
void updatetime(int &Tempref){
  while(Tempref < 0) Tempref += 1440;
  while(Tempref >= 1440) Tempref -= 1440;

  String temp = "";
  if(Tempref / 60 < 10) temp += "0";
  temp += String(Tempref / 60);
  if(show)temp += ":";else temp += " ";
  if ((Tempref % 60) < 10) temp += "0";
  temp += String(Tempref % 60);

  timetext = temp;
  int16_t x1, y1;
  uint16_t w, h; 
  tft.getTextBounds(timetext, 0, 0, &x1, &y1, &w, &h);
  if (timevalues.w != (int)w) timevalues = {(tft.width() - w) / 2, (tft.height() - h) / 2, (int)w};
  
}

void settext(String text, int margin = 0, bool centre = false){
  int16_t x1, y1;
  uint16_t w, h; 
  tft.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
  Words[text] = {centre ? ((tft.width() - w) / 2) : 0, offsety + margin, w};
  offsety += h + margin;
}

void clearboard(){
  tft.setTextSize(text_size);
  tft.fillScreen(ST77XX_BLACK); 
  tft.setCursor(0,0);

}
void reset(){
  clearboard();
  offsety = 0;
  Words.clear();

}

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
   tft.setOffsets(82, 18); 
  tft.invertDisplay(false); // Invert the colors (This display is flipped from normal)
  tft.setRotation(1); // Landscape, if it's upside down use 3!
  clearboard();
  Serial.println("TFT Initialized!");

  tft.setTextColor(ST77XX_WHITE);
  tft.print("Welcome to blare: lava"); // Show whatever you want! Draws from the top left of the text/number/shape 
}
void textsize(){
  
    reset();
    tft.fillScreen(ST77XX_BLACK); 
    tft.setCursor(0,0);
    settext("please choose text size");
    settext("1 = increase, 2 = decrease, 3 = enter, 4 = reset");
    updatetext();
    while(true){
      if(digitalRead(first_) == LOW){
        if (text_size < 10) text_size++;
        updatetext();
    }else if(digitalRead(second_) == LOW){
      if (text_size > 1) text_size--;
      updatetext();
    }else if(digitalRead(third_) == LOW){
      break;
    }else if(digitalRead(fourth_) == LOW){
      text_size = 3;
      updatetext();
    }
    while(digitalRead(first_) == LOW || digitalRead(second_) == LOW || digitalRead(third_) == LOW || digitalRead(fourth_) == LOW){
      
  tone(buzzer, 1000, 100); // Play 1000 Hz tone
    }
    }
}
void timechange(){
  start = false;
    reset();
    settext("please select current time");
    settext("press any button to continue");
    updatetext();
    while(digitalRead(first_) && digitalRead(second_) && digitalRead(third_) && digitalRead(fourth_)){delay(1); }
    updatetime(temptime);
  while(true){
      if(digitalRead(first_) == LOW){
        temptime += 1;
        updatetime(temptime);
    }else if(digitalRead(second_) == LOW){
      temptime -= 1;
    updatetime(temptime);
    }else if(digitalRead(third_) == LOW){
      break;
    }else if(digitalRead(fourth_) == LOW){
      temptime += 60;
    updatetime(temptime);
    }
    while(digitalRead(first_) == LOW || digitalRead(second_) == LOW || digitalRead(third_) == LOW || digitalRead(fourth_) == LOW){
      
  tone(buzzer, 1000, 100); // Play 1000 Hz tone
    }
    }
    temp = millis();
    start = true;
    showtime = true;
}
void setalarm(){
    reset();
    settext("please select alarm time");
    settext("press any button to continue");
    updatetext();
    while(digitalRead(first_) && digitalRead(second_) && digitalRead(third_) && digitalRead(fourth_)){delay(1); }
    updatetime(alarm);
  while(true){
      if(digitalRead(first_) == LOW){
        alarm += 1;
        updatetime(alarm);
    }else if(digitalRead(second_) == LOW){
      alarm -= 1;
    updatetime(alarm);
    }else if(digitalRead(third_) == LOW){
      break;
    }else if(digitalRead(fourth_) == LOW){
      alarm += 60;
    updatetime(alarm);
    }
    while(digitalRead(first_) == LOW || digitalRead(second_) == LOW || digitalRead(third_) == LOW || digitalRead(fourth_) == LOW){
  tone(buzzer, 1000, 100); // Play 1000 Hz tone
    }
    }
    alarmset = true;
}
void loop() {
  if ((digitalRead(first_) == LOW || digitalRead(second_) == LOW || digitalRead(third_) == LOW || digitalRead(fourth_) == LOW) && !start){
    textsize();
    timechange();
  }
  if (start){
while (millis() - temp >= 1000) {
temp += 1000;
show = !show;
sec++;
if (sec % 60 == 0){
temptime++;}
updatetime(temptime);
if(temptime == alarm && alarmset){
  tone(buzzer, 1000, 100);
}
}
//set timer
if (digitalRead(first_) == LOW){
  setalarm();
}
//change time
if (digitalRead(second_) == LOW){
  timechange();
}
//set text size
if (digitalRead(third_) == LOW){
  textsize();
}
//snooze
if (digitalRead(fourth_) == LOW){
if(temptime == alarm && alarmset){ 
  noTone(buzzer);
  alarmset = false;
  tone(buzzer, 700, 100);
}
}


  }
if (millis() - texttemp >= 100) {
  texttemp = millis();
  if(showtime) showtimetext();else updatetext();
}
}