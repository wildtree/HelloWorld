
#include <M5Unified.h>
#include <M5Cardputer.h>
#include <vscroll.h>
#include <lineeditor.h>
#include <graph.h>

const int KEYBOARD_I2C_ADDR = 0x08;
const int KEYBOARD_INT = 5;
// static xQueueHandle keyboard_queue = NULL;
uint16_t y = 0, x = 0;
static ZVScroll *vsc = NULL;
static LineEditor *le = NULL;
static Canvas *cv = NULL;
/*
static void IRAM_ATTR keyboard_cb()
{
  uint8_t key_val;
  Wire.requestFrom(KEYBOARD_I2C_ADDR, 1);
  key_val = Wire.read();
  if (key_val)
  {
    xQueueSendFromISR(keyboard_queue, &key_val, NULL);
  }
}
*/

void setup() {
    // put your setup code here, to run once:
    auto cfg = M5.config();
    M5Cardputer.begin(cfg);
    M5.Display.setRotation(1);
    // M5.Lcd.loadFont("fonts/RuikaMonoKyohkan-04-16",SD);
    M5.Display.setFont(&fonts::lgfxJapanGothic_8);
    M5.Display.println("Hello World!");
    M5.Display.println("こんにちは世界");
    //M5.lcd.unloadFont();
    //Wire.begin();
    //
    //keyboard_queue = x    QueueCreate(10,sizeof(uint8_t));
    //pinMode(KEYBOARD_INT, INPUT_PULLUP);
    //attachInterrupt(KEYBOARD_INT, keyboard_cb, FALLING);
    //pinMode(KEYBOARD_INT,INPUT);
    //digitalWrite(KEYBOARD_INT, HIGH);

    M5Cardputer.Keyboard.begin();
    M5.Display.setTextColor(GREEN);
    M5.Display.println("キー入力待ち。Waiting key...");
    cv = new Canvas(0, 3*16, 240, 135-3*16);
    y = 10 * 16;
    vsc = new ZVScroll(y, 16);
    y = vsc->scrollLine();

    le = new LineEditor();

    cv->cls();
    cv->line(0,0,240 - 1,135 - 3*16 - 1,RED);
    cv->paint(0,10,BLUE, RED);
    cv->paint(100,0,GREEN,RED);
}

void loop() {
    // put your main code here, to run repeatedly:
    uint8_t c;

    M5Cardputer.update();
    if (M5Cardputer.Keyboard.isPressed())
    {
        cv->cls();
    }
/*
  if (xQueueReceive(keyboard_queue,&c, 0))
  {
    if (c >= 0x20 && c < 0x7f)
    {
      M5.Lcd.printf("%c",c);
    }
    else if (c == 0x0a || c == 0x0d)
    {
      M5.Lcd.println("");
    }
  }
*/
#if 0
  if(digitalRead(KEYBOARD_INT) == LOW)
  {
    Wire.requestFrom(KEYBOARD_I2C_ADDR,1);
    if (Wire.available())
    {
      c = Wire.read();
      M5.Lcd.fillRect(0, 224, 320, 16, BLACK);
      String s = le->putChar(c);
      if (c == '\r')
      {
        x = 0;
        y = vsc->scrollLine();
        // draw it to scroll area
        vsc->print(s);
        return;
      }
      M5.Lcd.setCursor(0, 224);
      M5.Lcd.print(s);
/*
      if (c == 0x0a || c == 0x0d || x >= 320 - 8)
      {
        x = 0;
        y = vsc->scrollLine();
      }
      if (c >= 0x20 && c < 0x7f)
      {
        M5.Lcd.drawChar(c, x, y, 2);
        x += 8;
      }
      else if (c == 0x08)
      {
        x -= 8;
        if (x < 0) x = 0;
        M5.Lcd.fillRect(x, y, 8, 16, BLACK);
      }
      M5.Lcd.setCursor(160,0);
      M5.Lcd.fillRect(160,0,160,16,BLACK);
      M5.Lcd.printf("(x,y) = (%d,%d)",x,y);
*/
//      M5.Lcd.drawLine(319, 0, 0, 239, MAGENTA);
    }
  }
#endif
}