#include "ros/ros.h"
#include "rosgraph_msgs/Clock.h"




int main(int argc, char **argv)
{
  ros::init(argc, argv, "clock_server_node");
  ros::NodeHandle n;
  ros::Publisher publisher_clock;

  publisher_clock = n.advertise<rosgraph_msgs::Clock>("/clock", 100);

  while (ros::ok())
   {
       rosgraph_msgs::Clock timeNow;
       timeNow.clock = ros::Time::now();
       publisher_clock.publish(timeNow);
       ros::spin();
   }
  return 0;
}
