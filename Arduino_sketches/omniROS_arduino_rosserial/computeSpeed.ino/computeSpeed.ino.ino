////////////////////////////////
//
// Wifi ROS Car with ESP8266
//
// Find last versions at:
// https://github.com/agnunez/espros.git
//
// MIT License 2017 Agustin Nunez
/////////////////////////////////

#include <ESP8266WiFi.h>
#include <ros.h>
#include <geometry_msgs/Vector3.h>
#include <geometry_msgs/Vector3Stamped.h>

#define DEBUG 1

#include <PID_v1.h>
#define PID_P 5.0
#define PID_I 0.0
#define PID_D 0.0

#define pi 3.141592653
#define R 0.125
#define r 0.029
#define sinpi3 0.8660254037
#define nbsteprevolution 1398

//////////////////////
// WiFi Definitions //
//////////////////////
// WiFi configuration. Replace '***' with your data
const char* ssid = "ubiquityrobot05DA";
const char* password = "robotseverywhere";

IPAddress server(10, 42, 0, 1);   // Set the rosserial socket server IP address
const uint16_t serverPort = 11411;    // Set the rosserial socket server port




////////////////////////////Variables/////////////////  //////////////
const short encoderA0 = 14;
const short encoderA1 = 2;
const short encoderB0 = 12;
const short encoderB1 = 13;
const short encoderC0 = 15;
const short encoderC1 = 0;

const short pinMotor[3][3] = {
  {4, 1},
  {0, 3},
  {5, 16}
};


volatile long pos[] = {0, 0, 0};

float oldpos[] = {0, 0, 0};
float oldtime[] = {0, 0, 0};


double measuredSpeed[3] = {0.0, 0.0, 0.0};
double PIDSpeed[3] = {0.0, 0.0, 0.0};
double motorSpeed[3] = {0.0, 0.0, 0.0};

PID pid1(&measuredSpeed[0], &PIDSpeed[0], &motorSpeed[0], PID_P, PID_I, PID_D, DIRECT);
PID pid2(&measuredSpeed[1], &PIDSpeed[1], &motorSpeed[1], PID_P, PID_I, PID_D, DIRECT);
PID pid3(&measuredSpeed[2], &PIDSpeed[2], &motorSpeed[2], PID_P, PID_I, PID_D, DIRECT);

/////////////////////////////Interrupts Service Routine ///////////////
void encoderMotorA(void) {
  if (digitalRead(encoderA0) == digitalRead(encoderA1)) {
    pos[0]++;
  }
  else {
    pos[0]--;
  }
}

void encoderMotorB(void) {
  if (digitalRead(encoderB0) == digitalRead(encoderB1)) {
    pos[1]++;
  }
  else {
    pos[1]--;
  }
}

void encoderMotorC(void) {
  bool pinStatus;
  if (analogRead(encoderC1) > 125) {
    pinStatus = HIGH;
  }
  if (digitalRead(encoderC0) == pinStatus) { //Little hack , I don't have enough digital pins so i use the ADC , slower but effective
    pos[2]++;
  }
  else {
    pos[2]--;
  }
}




void cmd_vel_callback( const geometry_msgs::Vector3& cmd_vel) {

  motorSpeed[0] = (cmd_vel.y + R * cmd_vel.z) / r;
  motorSpeed[1] = (-sinpi3 * cmd_vel.x + 0.5 * cmd_vel.y + R * cmd_vel.z) / r;
  motorSpeed[2] = (sinpi3 * cmd_vel.x + 0.5 * cmd_vel.y + R * cmd_vel.z) / r;


  pid1.Compute();
  if (abs(measuredSpeed[0] - motorSpeed[0]) > 0.1) {
    if (PIDSpeed[0] > 0) {
      digitalWrite(pinMotor[0][1], LOW);
      analogWrite(pinMotor[0][0], abs(PIDSpeed[0]));
    }
    else {
      digitalWrite(pinMotor[0][1], HIGH);
      analogWrite(pinMotor[0][0], abs(PIDSpeed[0]));
    }
  }
  pid2.Compute();
  if (abs(measuredSpeed[1] - motorSpeed[1]) > 0.1) {
    if (PIDSpeed[1] > 0) {
      digitalWrite(pinMotor[1][1], LOW);
      analogWrite(pinMotor[1][0], abs(PIDSpeed[1]));
    }
    else {
      digitalWrite(pinMotor[1][1], HIGH);
      analogWrite(pinMotor[1][0], abs(PIDSpeed[1]));
    }
  }

  if (abs(measuredSpeed[2] - motorSpeed[2]) > 0.1) {
    pid3.Compute();
    if (PIDSpeed[2] > 0) {

      digitalWrite(pinMotor[2][1], LOW);
      analogWrite(pinMotor[2][0], abs(PIDSpeed[2]));
    }
    else {
      digitalWrite(pinMotor[2][1], HIGH);
      analogWrite(pinMotor[2][0], abs(PIDSpeed[2]));
    }
  }


}






geometry_msgs::Vector3Stamped odometry_msg;

ros::Publisher odom_publisher("/omniROS/odometryRAW", &odometry_msg);

ros::Subscriber<geometry_msgs::Vector3> speed_subscriber("/omniROS/cmd_vel", &cmd_vel_callback);




void setupWiFi() {                    // connect to ROS server as as a client
  /*
    Serial.begin(115200);               // Use ESP8266 serial only for to monitor the process
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
  */
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
  /*
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  */
}

ros::NodeHandle nh;

void setup() {
  //Serial.begin(115200);
  setupWiFi();
  delay(2000);
  nh.getHardware()->setConnection(server, serverPort);
  nh.initNode();

  nh.advertise(odom_publisher);

  nh.subscribe(speed_subscriber);

  // configure GPIO's


  pinMode(pinMotor[0][0], OUTPUT);
  pinMode(pinMotor[0][1], OUTPUT);
  pinMode(pinMotor[1][0], OUTPUT);
  pinMode(pinMotor[1][1], OUTPUT);
  pinMode(pinMotor[2][0], OUTPUT);
  pinMode(pinMotor[2][1], OUTPUT);


  pid1.SetMode(AUTOMATIC);
  pid1.SetOutputLimits(-255, 255);

  pid2.SetMode(AUTOMATIC);
  pid2.SetOutputLimits(-255, 255);

  pid3.SetMode(AUTOMATIC);
  pid3.SetOutputLimits(-255, 255);


  pinMode(encoderA0, INPUT);
  pinMode(encoderB0, INPUT);
  pinMode(encoderC0, INPUT);
  pinMode(encoderA1, INPUT);
  pinMode(encoderB1, INPUT);
  pinMode(encoderC1, INPUT);

  digitalWrite(pinMotor[0][0], LOW);
  digitalWrite(pinMotor[0][1], LOW);
  digitalWrite(pinMotor[1][0], LOW);
  digitalWrite(pinMotor[1][1], LOW);
  digitalWrite(pinMotor[2][0], LOW);
  digitalWrite(pinMotor[2][1], LOW);


  sei();                                // Enable interrupts



}

void loop() {
  measuredSpeed[0] = computeSpeed(0);
  measuredSpeed[1] = computeSpeed(1);
  measuredSpeed[2] = computeSpeed(2);

  odometry_msg.vector.x = measuredSpeed[0];
  odometry_msg.vector.y = measuredSpeed[1];
  odometry_msg.vector.z = measuredSpeed[2];
  odom_publisher.publish( &odometry_msg );
  nh.spinOnce();
  delay(50);
}
