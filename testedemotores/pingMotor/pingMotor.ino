

// the setup function runs once when you press reset or power the board
void setup() {
    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(5, OUTPUT);
    
  }
  
  // the loop function runs over and over again forever
  void loop() {
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    digitalWrite(3, HIGH);
    digitalWrite(5, HIGH);
    delay(1000);                      // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    digitalWrite(3, LOW);
    digitalWrite(5, LOW);
    delay(1000);                      // wait for a second
  }
  