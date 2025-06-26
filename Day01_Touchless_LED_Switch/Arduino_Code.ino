/*THIS CODE LOGIC WILL WORK AS BY ODD OR EVEN GESTURES . AS THE STARTING GESTURE IS ZERO SO 1ST GESTURE IS ONE AND THAT IS ODD SO
   FOR ODD GESTURE THE LED OR THE RELAY WILL TURN ON AND FOR EVEN IT WILL TURN OFF
*/



#define trig_pin 3
#define echo_pin 4
#define led 2
long duration = 0;
int distance = 0;
int gesture = 0;



void setup() {
  Serial.begin(9600);
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
  pinMode(led, OUTPUT);
 
  // put your setup code here, to run once:

}

void loop() {
  digitalWrite(3, LOW);
  delayMicroseconds(2);
  digitalWrite(3, HIGH);
  delayMicroseconds(10);
  digitalWrite(3, LOW);
  

  duration = pulseIn(4, HIGH);
  distance = (duration  * 0.034 ) / 2 ;
  Serial.print("DISTANCE " );
   Serial.println(distance);
  
  
  
  
  Serial.println(gesture);
  gesture_count();
  
  delay(100);

  // put your main code here, to run repeatedly:

}


void gesture_count()
{
  
  if (distance <= 10)//for gesture detection
  {
    gesture++;
  }
  if (gesture % 2 == 1)
  {
    digitalWrite(led, HIGH);
  }

  if (gesture % 2 == 0)
  {
    digitalWrite(led, LOW);
  }

}
