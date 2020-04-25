//Due to an inconsistency in the ESP8266 rosserial time, the timestamp is created in this NodeHandle
//When an odometryRAW message is received, we add the timestamp of the computer to it
//Also the tf transform is sent from there, which is not crucial but is easier to change on the go

#include "ros/ros.h"
#include "nav_msgs/Odometry.h"
#include <tf/transform_broadcaster.h>


class OdometryClass
{
private:

    ros::NodeHandle n;
    ros::Subscriber subscription_odometryRAW;
    ros::Publisher publisher_odometry;
    tf::TransformBroadcaster broadcaster_odometry_tf;
    nav_msgs::Odometry odometry;

public:

  OdometryClass();

  void callback(const nav_msgs::Odometry::ConstPtr& odometryRAW)
  {
    nav_msgs::Odometry odometry;
    odometry = *odometryRAW;
    ros::Time TimeNow = ros::Time::now();
    //first, we'll publish the transform over tf
    geometry_msgs::TransformStamped odom_trans;
    odom_trans.header.stamp = TimeNow;
    odom_trans.header.frame_id = "odom";
    odom_trans.child_frame_id = "base_link";
    odom_trans.transform.translation.x = odometryRAW->pose.pose.position.x;
    odom_trans.transform.translation.y = odometryRAW->pose.pose.position.y;
    odom_trans.transform.translation.z = 0.0;
    geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(odometryRAW->pose.pose.orientation.w);
    odometry.pose.pose.orientation = odom_quat;
    odom_trans.transform.rotation = odom_quat;

    this->publisher_odometry.publish(odometry);
    //send the transform
    this->broadcaster_odometry_tf.sendTransform(odom_trans);


  }

};

OdometryClass::OdometryClass(){

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
