#define pinLED LED_BUILTIN
#define buttonPIN  4
boolean buttonPrev = true;

void setup() {
  Keyboard.begin();
  pinMode(buttonPIN, INPUT);
  digitalWrite(buttonPIN, HIGH);
  
  pinMode(pinLED,  OUTPUT);
}

void loop() {
  boolean button = digitalRead(buttonPIN);
  if (button & !buttonPrev)
  {
    delay (50);
    boolean button = digitalRead(buttonPIN);
    if (button)
    {
      digitalWrite (pinLED, HIGH);
      Keyboard.press ('a');
      delay (10);
      Keyboard.releaseAll ();
      delay (140);
      digitalWrite (pinLED, LOW);
    }
  } 
  buttonPrev = button;
}
