const int sensorPin = 2;                 // Define the digital input pin for the sensor
volatile int pulseCount = 0;              // Define a variable to hold the pulse count, which can be modified by the ISR
unsigned long lastTime = 0;               // Define a variable to hold the last time the RPM was measured

//Motor SetUp
#define pwm1 5                           // Define
#define pwm2 6                           // Define
int maks=255;

const int FILTER_LENGTH = 5;              // Length of the moving average filter
int pulseBuffer[FILTER_LENGTH];           // Buffer to hold the last several pulse counts
int bufferIndex = 0;                      // Index into the buffer for the next value
int bufferSum = 0;                        // Sum of the values in the buffer

void setup() {
  pinMode(sensorPin, INPUT_PULLUP);       // Set the sensor pin as an input with a pull-up resistor
  attachInterrupt(digitalPinToInterrupt(sensorPin), countPulse, RISING);  // Attach an interrupt to the sensor pin that will trigger on a rising edge
  Serial.begin(9600);                     // Start the serial communication with a baud rate of 9600
}

  int i;
  bool inc = 1;
  long unsigned motorTime = 0;
  int counter = 0;

void loop() {
  //Motor SetUp
  if (millis() - motorTime >= 10)
    {
      if(inc && i < 255)
      {
        counter = 0;
        analogWrite(pwm1,i);
        analogWrite(pwm2,0);
        i++;
      }
      else if(i == 255 && counter < 400)
      {
        counter++;
        analogWrite(pwm1,i);
        analogWrite(pwm2,0);
      }
      else if(!inc && i > 0)
      {
        analogWrite(pwm1,i);
        analogWrite(pwm2,0);
        i--;
      }
      motorTime=millis();
    }
    
    if (int i=255)
    {
      if (millis() - motorTime >= 5000)
      {      
      analogWrite(pwm1,i);
      analogWrite(pwm2,0);
      motorTime=millis();
      }
    }
  // Measure RPM every second
  if (millis() - lastTime >= 1000) {      // Check if one second has passed since the last RPM measurement
    // Add the latest pulse count to the buffer and update the buffer sum
    bufferSum -= pulseBuffer[bufferIndex];
    pulseBuffer[bufferIndex] = pulseCount;
    bufferSum += pulseCount;
    bufferIndex = (bufferIndex + 1) % FILTER_LENGTH;
    
    // Calculate the filtered RPM from the average of the pulse counts in the buffer
    int rpm = bufferSum * 60 / (2 * FILTER_LENGTH);      // Calculate the RPM from the pulse count, assuming two pulses per revolution
    Serial.println(rpm);                  // Print the RPM to the serial monitor
    pulseCount = 0;                       // Reset the pulse count for the next measurement
    lastTime = millis();                  // Update the last measurement time
  }
}

void countPulse() {
  pulseCount++;                           // Increment the pulse count each time the ISR is triggered
}
