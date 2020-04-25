#include <ros/ros.h>
#include <total_torque/Float32Stamped.h>
#include "std_srvs/Empty.h"
#include "std_msgs/Float32.h"
#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include "model_state_stamper/ModelStatesStamped.h"
#define wD 1
#define wT 0
// This code is modified from https://gist.github.com/tdenewiler/e2172f628e49ab633ef2786207793336

class Fitness
{

private:
    ros::NodeHandle n;
    message_filters::Subscriber<total_torque::Float32Stamped> sub_1_;
    message_filters::Subscriber<model_state_stamper::ModelStatesStamped> sub_2_;
    typedef message_filters::sync_policies::ApproximateTime<total_torque::Float32Stamped, model_state_stamper::ModelStatesStamped> MySyncPolicy;
    typedef message_filters::Synchronizer<MySyncPolicy> Sync;
    boost::shared_ptr<Sync> sync_;
    ros::Publisher publisher_fitness;
    ros::ServiceServer service_server_fitness;
    std_msgs::Float32 fitnessMax;
    double weightTorque;
    double weightDistance;
    double oldFitness;

 public:

  Fitness()
  {
    ROS_INFO("initialization of fitness node : \n");
    this->weightTorque= wT;
    this->weightDistance = wD;
    sub_1_.subscribe(n, "/omniROS/totalTorque", 1);
    sub_2_.subscribe(n, "/omniROS/modelStatesStamped", 1);
    this->service_server_fitness = this->n.advertiseService("/omniROS/resetTotalFitness",&Fitness::resetService,this);
    this->publisher_fitness = this->n.advertise<std_msgs::Float32>("/omniROS/fitness", 100);
    sync_.reset(new Sync(MySyncPolicy(10), sub_1_, sub_2_));
    sync_->registerCallback(boost::bind(&Fitness::callback, this, _1, _2));

  }

  void callback(const total_torque::Float32Stamped::ConstPtr& total_torque, const model_state_stamper::ModelStatesStamped::ConstPtr& model_states_stamped)
  {
    double distanceBR = sqrt(pow(model_states_stamped->pose[1].position.x - model_states_stamped->pose[2].position.x ,2.0) +  pow(model_states_stamped->pose[1].position.y- model_states_stamped->pose[2].position.y ,2.0) + pow(model_states_stamped->pose[1].position.z- model_states_stamped->pose[2].position.z ,2.0));
    //For testing purposes, fitnesse function = distance max - actual distance
    double fitness = 5 - distanceBR * this->weightDistance;// + this->weightTorque * total_torque->data;

    //ROS_INFO("Distance factor :  %f \nTorque factor : %f \n",distanceBR * this->weightDistance,this->weightTorque * total_torque->data);
    if(fitness > this->fitnessMax.data){
      this->fitnessMax.data = fitness;
    }
    this->oldFitness = fitness;
    this->publisher_fitness.publish(this->fitnessMax);

  }

  bool resetService(std_srvs::Empty::Request &req,std_srvs::Empty::Response &res){
    this->fitnessMax.data = 0.0;
  }

};

int main(int argc, char **argv)
{
  ros::init(argc, argv, "fitness_node");

  Fitness fitness;

  ros::spin();
}
