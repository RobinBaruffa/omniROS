/* This sketch is used to calibrate properly the wheel radius
 * It updates the R,r1,r2 and r3 value published on the wheelRadius topic
 * The wheels speed is brodcasted via a rostopic to be recorded in a rosbag, then the data is exported to a simulator
 * The purpose of the simulator is to optimize the wheel radius to match the real pah of the robot as closely as possible
 */

#include <PID_v1.h>
#include <ESP8266WiFi.h>
#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/String.h>
#include <control_msgs/PidState.h>
#include <std_msgs/Float32MultiArray.h>
//#include <ros/time.h>
#include <tf/tf.h>
//#include <tf/transform_broadcaster.h>


#define ODOMETRY_PUBLISHER_FREQUENCY 8.25
#define IDLE_THRESHOLD 15  //Maximum PID output that is in the dead-zone (no movement just hissing sounds)
#define PID_P 30.0
#define PID_I 0.0
#define PID_D 0.0
#define PID_REFRESH_RATE 100 //Refresh time between two PID.compute in millisecond

float R = 0.1245;
float r1 = 0.029;
float r2 = 0.029;
float r3 = 0.029;

#define sinpi3 0.86602
#define sqrt3  1.73205

// WiFi configuration. Replace '***' with your data

const char* ssid = "ubiquityrobot05DA";
const char* password = "robotseverywhere";
IPAddress server(10, 42, 0, 1);   // Set the rosserial socket ROSCORE SERVER IP address
const uint16_t serverPort = 11411;    // Set the rosserial socket server port

unsigned long timer;
unsigned long oldNow;

const short pinMotor[3][2] = {
  {4, 5},
  {14, 0},
  {12, 13}
};

double measuredSpeed[3] = {0.0, 0.0, 0.0};
double PIDSpeed[3] = {0.0, 0.0, 0.0};
double motorSpeed[3] = {0.0, 0.0, 0.0};

float oldPosition[3] = {0.0, 0.0, 0.0};
float theta;

PID pid1(&measuredSpeed[0], &PIDSpeed[0], &motorSpeed[0], PID_P, PID_I, PID_D, DIRECT);
PID pid2(&measuredSpeed[1], &PIDSpeed[1], &motorSpeed[1], PID_P, PID_I, PID_D, DIRECT);
PID pid3(&measuredSpeed[2], &PIDSpeed[2], &motorSpeed[2], PID_P, PID_I, PID_D, DIRECT);



void message_callback( const geometry_msgs::Twist& twist_msg) {

  motorSpeed[0] = (-twist_msg.linear.x + R * twist_msg.angular.z) / r1;
  motorSpeed[1] = (-sinpi3 * twist_msg.linear.y - 0.5 *  (-twist_msg.linear.x) + R * twist_msg.angular.z) / r2;
  motorSpeed[2] = (sinpi3 * twist_msg.linear.y - 0.5 * (-twist_msg.linear.x) + R * twist_msg.angular.z) / r3;

  pid1.Compute();
  if (PIDSpeed[0] < IDLE_THRESHOLD && motorSpeed[0] == 0) {
    digitalWrite(pinMotor[0][0], LOW);
  }
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
  if (PIDSpeed[1] < IDLE_THRESHOLD && motorSpeed[1] == 0) {
    digitalWrite(pinMotor[1][0], LOW);
  }
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
  pid3.Compute();
  if (PIDSpeed[2] < IDLE_THRESHOLD && motorSpeed[2] == 0) {
    digitalWrite(pinMotor[2][0], LOW);
  }
  if (abs(measuredSpeed[2] - motorSpeed[2]) > 0.1) {
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

void PID_callback(const control_msgs::PidState& PID_msg) {
  pid1.SetTunings(PID_msg.p_term, PID_msg.i_term, PID_msg.d_term);
  pid2.SetTunings(PID_msg.p_term, PID_msg.i_term, PID_msg.d_term);
  pid3.SetTunings(PID_msg.p_term, PID_msg.i_term, PID_msg.d_term);
}

void WheelRadius_callback(const std_msgs::Float32MultiArray& WheelRadius_msg) {
  R = WheelRadius_msg.data[0];
  r1 = WheelRadius_msg.data[1];
  r2 = WheelRadius_msg.data[2];
  r3 = WheelRadius_msg.data[3];
}

nav_msgs::Odometry odometry_msg;
ros::Publisher chatter("/omniROS/odometryRAW", &odometry_msg);


ros::Subscriber<geometry_msgs::Twist> sub("/omniROS/cmd_vel", &message_callback);
ros::Subscriber<control_msgs::PidState> subPID("/omniROS/PID", &PID_callback);
ros::Subscriber<std_msgs::Float32MultiArray> subWheelRadius("/omniROS/wheel_radius", &WheelRadius_callback);



void setupWiFi() {                    // connect to ROS server as as a client
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

ros::NodeHandle nh;

////////////////////////////////////Setup////////////////////////////////////////////
void setup()
{
  Serial.begin(9600);
  setupWiFi();

  nh.getHardware()->setConnection(server, serverPort);
  nh.initNode();
  nh.advertise(chatter);

  nh.subscribe(sub);
  nh.subscribe(subPID);


  pinMode(pinMotor[0][0], OUTPUT);
  pinMode(pinMotor[0][1], OUTPUT);
  pinMode(pinMotor[1][0], OUTPUT);

  pinMode(pinMotor[1][1], OUTPUT);

  pinMode(pinMotor[2][0], OUTPUT);

  pinMode(pinMotor[2][1], OUTPUT);


  pid1.SetMode(AUTOMATIC);
  pid1.SetOutputLimits(-1023, 1023);
  pid1.SetSampleTime(PID_REFRESH_RATE);

  pid2.SetMode(AUTOMATIC);
  pid2.SetOutputLimits(-1023, 1023);
  pid2.SetSampleTime(PID_REFRESH_RATE);

  pid3.SetMode(AUTOMATIC);
  pid3.SetOutputLimits(-1023, 1023);
  pid3.SetSampleTime(PID_REFRESH_RATE);

  odometry_msg.child_frame_id = "base_link";
  odometry_msg.header.frame_id = "odom";

  //odom_trans.child_frame_id = "base_link";
  //odom_trans.header.frame_id = "odom";



}

///////////////////////////////////loop///////////////////////////////////////////////

void loop() {


  if (Serial.available()) {
    String packet = Serial.readStringUntil('\n');

    int firstComma = packet.indexOf(',', 0);
    int secondComma = packet.indexOf(',', firstComma + 1);
    int thirdComma = packet.indexOf('\n', secondComma + 1);

    String firstString =  packet.substring(0, firstComma);
    String secondString = packet.substring(firstComma + 1, secondComma);
    String thirdString =  packet.substring(secondComma + 1, thirdComma);

    measuredSpeed[0] = firstString.toFloat();
    measuredSpeed[1] = secondString.toFloat();
    measuredSpeed[2] = thirdString.toFloat();
  }

  unsigned long now = micros();
  float dT = (now - oldNow) * 0.000001;
  oldNow = now;


  float worldSpeed[3];
  /*
  worldSpeed[0] = (r / sqrt3) * (measuredSpeed[2] - measuredSpeed[1]);
  worldSpeed[1] = -(r / 3) * (2 * measuredSpeed[0] - measuredSpeed[1] - measuredSpeed[2]);
  worldSpeed[2] = (r / (3 * R)) * (measuredSpeed[0] + measuredSpeed[1] + measuredSpeed[2]);
*/
  //Source : Introduction to Autonomous Mobile Robots, Roland Siegwart and Illah R. Nourbakhsh
  //Assumptions : rolling without slipping, distance R from the center to each wheel is the same
  worldSpeed[0] = (r1/sqr3)*(measuredSpeed[0] - measuredSpeed[2]);
  worldSpeed[0] = (r2/3)*(-measuredSpeed[0] + 2*measuredSpeed[1] - measuredSpeed[2]);
  worldSpeed[0] = (r3/(3*R))*(-measuredSpeed[0] - measuredSpeed[1] - measuredSpeed[2]);
  
  ros::Time newTime = nh.now();
  odometry_msg.header.stamp = newTime;
  odometry_msg.twist.twist.linear.x =  worldSpeed[0];
  odometry_msg.twist.twist.linear.y =  worldSpeed[1];
  odometry_msg.twist.twist.angular.z = worldSpeed[2];

  theta = theta + worldSpeed[2] * dT;

  float position[3];
  //Implemented from https://pdfs.semanticscholar.org/821b/f18bf282d3be895bca764c1121ebbda6f631.pdf (7)
  //Here Vfront is worldSpeed[0] Vside is worldSpeed[1] and w is worldSpeed[2]

  position[0] = oldPosition[0] + dT * (worldSpeed[1] * cos(theta) - worldSpeed[0] * sin(theta));
  position[1] = oldPosition[1] + dT * (worldSpeed[1] * sin(theta) + worldSpeed[0] * cos(theta));
  position[2] = oldPosition[2] + worldSpeed[2] * dT;

  oldPosition[0] = position[0];
  oldPosition[1] = position[1];
  oldPosition[2] = position[2];

  odometry_msg.pose.pose.position.x = position[0];
  odometry_msg.pose.pose.position.y = position[1];
  odometry_msg.pose.pose.orientation.w = position[2];

  //This is an wrong use of odometry messages but is convenient now
  odometry_msg.pose.pose.orientation.x = measuredSpeed[0];
  odometry_msg.pose.pose.orientation.y = measuredSpeed[1];
  odometry_msg.pose.pose.orientation.z = measuredSpeed[2];
  
  chatter.publish( &odometry_msg );


  nh.spinOnce();
}
