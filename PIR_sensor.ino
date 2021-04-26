int sensor = A0; /
int state = LOW; // by default, no motion detected
int val = 0;

void setup() {
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(9, OUTPUT);
  pinMode(sensor, INPUT);    // initialize sensor as an input
  Serial.begin(9600);        // initialize serial
}

void loop(){
  val = digitalRead(sensor);   // read sensor value
  if (val == HIGH) {           // check if the sensor is HIGH
  RGB_color(255, 255, 255);

    delay(10000);                // delay 100 milliseconds

    if (state == LOW) {
      Serial.println("Motion detected!");
      state = HIGH;       // update variable state to HIGH
    }
  }
  else {
  RGB_color(255, 255, 0);
      delay(100);
      if (state == HIGH){
        Serial.println("Motion stopped!");
        state = LOW;

  }
}

    void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
 {
  analogWrite(10, red_light_value);
  analogWrite(9, green_light_value);
  analogWrite(8, blue_light_value);
}
