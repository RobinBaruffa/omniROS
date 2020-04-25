
#include <ros.h>

#include <geometry_msgs/Twist.h>

#define FREQUENCY 20
#define MAX_SPEED_TRANSLATION 1
#define MAX_SPEED_ROTATION 1 //2pi rad /sec

ros::NodeHandle  nh;

float timer;
geometry_msgs::Twist twist_msg;
ros::Publisher pub_range( "/joystick", &twist_msg);

void setup()
{
  nh.initNode();
  nh.advertise(pub_range);
  
}

void loop()
{

  if ( (millis()-timer) > FREQUENCY / 1000){
    float input [3] = {analogRead(0)-512,analogRead(1)-512,analogRead(3)-512};
    for(int i=0;i<=3;i++){
      if(abs(input[i])< 10){
        input[i]=0;         //Remove dead-zone (10 /1023 is a sweet spot)
      }
    }
    twist_msg.linear.x= -(input[0] / 512 )*MAX_SPEED_TRANSLATION;
    twist_msg.linear.y= -(input[1] / 512 )*MAX_SPEED_TRANSLATION;
    twist_msg.angular.z =(input[2] / 512 )*MAX_SPEED_ROTATION;
    pub_range.publish(&twist_msg);
    timer =  millis() + FREQUENCY / 1000;
  }
  nh.spinOnce();
}
