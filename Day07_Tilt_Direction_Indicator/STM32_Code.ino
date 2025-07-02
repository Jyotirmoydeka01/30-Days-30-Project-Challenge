#include<Wire.h>
#include<MPU6050.h>


MPU6050 mpu;

#define led_l PA0
#define led_r PA1
#define led_f PA2
#define led_b PA3
#define led_c PA4


int AngleX;
int AngleY;

int16_t ax, ay, az;



void setup() {
  Serial.begin(9600);


  pinMode(led_l, OUTPUT); pinMode(led_r, OUTPUT); pinMode(led_f, OUTPUT); pinMode(led_b, OUTPUT); pinMode(led_c, OUTPUT);
  Wire.begin();

  mpu.initialize();

  if (mpu.testConnection()) {
    Serial.println("MPU Connected");


  }

  else {
    Serial.println("MPU  not Connected");
    while (1);
  }


}

void loop() {
  digitalWrite(led_l, LOW);
  digitalWrite(led_r, LOW);
  digitalWrite(led_f, LOW);
  digitalWrite(led_b, LOW);
  digitalWrite(led_c, LOW);
  

  

  mpu.getAcceleration(&ax, &ay, &az);

  AngleX = atan2(ay, az) * 180 / PI;

  AngleY = atan2(-az, ax) * 180 / PI;
   
  if (AngleX > 5 ) {
    digitalWrite(led_r, HIGH);Serial.println("right");
  }
  if (AngleX < -5 ) {
    digitalWrite(led_l, HIGH);Serial.println("left");
  }
  if (AngleY > -85 ) {
    digitalWrite(led_b, HIGH);Serial.println("back");
  }

if (AngleY < -95 ) {
    digitalWrite(led_f, HIGH);Serial.println("front");
  }
else { digitalWrite(led_c, HIGH);
 
  }

   
  //led_pattern();

  Serial.print(AngleX);
  Serial.print("\t");
  Serial.println(AngleY);

  delay(200);


}

//void led_pattern() {}

 
