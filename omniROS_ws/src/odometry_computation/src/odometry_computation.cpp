#include "ros/ros.h"
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/TwistStamped.h"
#include <tf/transform_broadcaster.h>

class OdometryClass
{
private:

    ros::NodeHandle n;
    ros::Subscriber subscription_odometryRAW;
    ros::Publisher publisher_odometry;
    tf::TransformBroadcaster broadcaster_odometry_tf;
    nav_msgs::Odometry odometry;
    double radius;
    ros::Time last_time;
    float oldX;
    float oldY;
    float oldZ;
public:

  OdometryClass();

  void callback(const geometry_msgs::TwistStamped::ConstPtr& twist)
  {

    float R=0.125;  //Distance between the center of rotation and the center of the wheels
    float r= 0.029; //Radius of the wheels
    this->odometry.child_frame_id = "odom";
    this->odometry.header.frame_id = "base_link";
    this->odometry.header.stamp = ros::Time::now();
    double dT = (twist->header.stamp.sec + twist->header.stamp.nsec * 0.000000001) - (this->last_time.sec + this->last_time.nsec * 0.000000001);
    this->odometry.twist.twist.linear.x = -r*(twist->twist.linear.y-twist->twist.linear.z)/sqrt(3);
    this->odometry.twist.twist.linear.y = (r/3)*(2*twist->twist.linear.x-twist->twist.linear.y-twist->twist.linear.z);
    this->odometry.twist.twist.linear.z = (r/(3*R))*(twist->twist.linear.x+twist->twist.linear.y+twist->twist.linear.z);

    this->odometry.pose.pose.position.x = this->oldX + this->odometry.twist.twist.linear.x*dT;
    this->odometry.pose.pose.position.y = this->oldY + this->odometry.twist.twist.linear.y*dT;
    this->odometry.pose.pose.position.z = this->oldZ + this->odometry.twist.twist.linear.z*dT;

    this->publisher_odometry.publish(this->odometry);

    geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(this->odometry.pose.pose.position.z);
    //first, we'll publish the transform over tf
    geometry_msgs::TransformStamped odom_trans;
    odom_trans.header.stamp = this->odometry.header.stamp;
    odom_trans.header.frame_id = "map";
    odom_trans.child_frame_id = "odom";

    odom_trans.transform.translation.x = this->odometry.pose.pose.position.x;
    odom_trans.transform.translation.y = this->odometry.pose.pose.position.y;
    odom_trans.transform.translation.z = 0.0;
    odom_trans.transform.rotation = odom_quat;

    //send the transform
    this->broadcaster_odometry_tf.sendTransform(odom_trans);

    this->last_time = twist->header.stamp;
    this->oldX = this->odometry.pose.pose.position.x;
    this->oldY = this->odometry.pose.pose.position.y;
    this->oldZ = this->odometry.pose.pose.position.z;

  }

};

OdometryClass::OdometryClass(){

  this->radius = 0.029;
  this->subscription_odometryRAW = this->n.subscribe("/omniROS/odometryRAW", 100,&OdometryClass::callback,this);
  this->publisher_odometry = this->n.advertise<nav_msgs::Odometry>("/odom", 100);

};

int main(int argc, char **argv)
{
  ros::init(argc, argv, "odometry_computation_node");
  OdometryClass odometryclass;
  // torque.initialisation();
  while (ros::ok())
   {
       ros::spin();
   }
  return 0;
}
