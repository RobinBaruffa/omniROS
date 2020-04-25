#include "ros/ros.h"
#include "total_torque/Float32Stamped.h" //Message type for the torque values
#include "geometry_msgs/Vector3Stamped.h"
#include "std_msgs/Float32.h"
#include "std_srvs/Empty.h"

class Torque
{
private:
    total_torque::Float32Stamped totalTorque;
    ros::NodeHandle n;
    ros::Subscriber subscription_torque;
    ros::Publisher publisher_torque;
    ros::ServiceServer service_server_torque;
public:

  Torque();

  void callbackTorque(const geometry_msgs::Vector3Stamped::ConstPtr& torque)
  {
    this->totalTorque.data += std::abs(torque->vector.x);
    this->totalTorque.header.stamp = ros::Time::now();
    this->publisher_torque.publish(this->totalTorque);

  }

  bool resetService(std_srvs::Empty::Request &req,std_srvs::Empty::Response &res){
    //This is a service server that will reset the totalTorque, it's a function that takes a void as input and returns a void as an output
    this->totalTorque.data=0.0;
  }
};

Torque::Torque(){
  this->service_server_torque = n.advertiseService("/omniROS/resetTotalTorque",&Torque::resetService,this);
  this->subscription_torque = this->n.subscribe("/omniROS/torque", 100,&Torque::callbackTorque,this);
  this->publisher_torque = this->n.advertise<total_torque::Float32Stamped>("/omniROS/totalTorque", 100);
};

int main(int argc, char **argv)
{
  ros::init(argc, argv, "total_torque_node");
  Torque torque;
  // torque.initialisation();
  while (ros::ok())
   {
       ros::spin();
   }
  return 0;
}
