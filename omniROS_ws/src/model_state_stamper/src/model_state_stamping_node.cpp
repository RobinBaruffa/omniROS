#include "ros/ros.h"
#include "model_state_stamper/ModelStatesStamped.h"
#include "gazebo_msgs/ModelStates.h"

class ModelStates
{
private:
    model_state_stamper::ModelStatesStamped model_states_stamped;
    ros::NodeHandle n;
    ros::Subscriber subscription_model_states;
    ros::Publisher publisher_model_states_stamped;
public:

  ModelStates();

  void callbackModel_states(const gazebo_msgs::ModelStates::ConstPtr& model_states)
  {
    this->model_states_stamped.header.stamp = ros::Time::now();
    this->model_states_stamped.name =model_states->name;
    this->model_states_stamped.pose =model_states->pose;
    this->publisher_model_states_stamped.publish(this->model_states_stamped);

  }

};

ModelStates::ModelStates(){

  this->subscription_model_states = this->n.subscribe("/gazebo/model_states", 100,&ModelStates::callbackModel_states,this);
  this->publisher_model_states_stamped = this->n.advertise<model_state_stamper::ModelStatesStamped>("/omniROS/modelStatesStamped", 100);
};

int main(int argc, char **argv)
{
  ros::init(argc, argv, "model_states_stamping_node");
  ModelStates modelStates;
  // torque.initialisation();
  while (ros::ok())
   {
       ros::spin();
   }
  return 0;
}
