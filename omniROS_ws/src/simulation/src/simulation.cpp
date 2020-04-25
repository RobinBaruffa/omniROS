#include "ros/ros.h"
#include "geometry_msgs/Vector3Stamped.h" //Message type for the torque values
#include "gazebo_msgs/SetModelState.h" //Service type for teleporting the robot via
#include "rosgraph_msgs/Clock.h"
#include "gazebo_msgs/ModelStates.h"  //Message type to read current robot / ball position
#include "std_srvs/Empty.h"
#include <stdlib.h>
#include <ros/console.h>
#include "tf/tf.h"

#include <cmath>


//#define M_PI 3.14159265359
#define POSITION_ROBOT_MAX 0.0
#define POSITION_BALL_MAX 5.0
#define POSITION_BALL_MIN 2.0
#define SPEED_BALL_MAX 10.0
#define SPEED_BALL_MIN 5.0
#define SPEED_BALL_ANGLE_MAX 0.3  //max angle between the initial trajectory of the ball and the initial position of the robot in radians
#define SIMTIMEMAX 7.0

class Simulation
{
private:
    double distanceBR;
    rosgraph_msgs::Clock simTimeStart;
    double simTimeMax;
    double position_robot_max;
    double position_ball_max;
    double position_ball_min;
    double speed_ball_max;
    double speed_ball_min;
    double speed_ball_angle_max;
    gazebo_msgs::SetModelState initial_model_state_robot;  //Keeps track of the initial position and speed of the initialRobotPosition
    gazebo_msgs::SetModelState initial_model_state_ball;
    ros::ServiceClient ServicePauseSimulation;
    ros::ServiceClient ServiceUnpauseSimulation;
    ros::ServiceClient ServiceSetModelState;
    ros::ServiceClient ServiceResetTorque;
    ros::ServiceClient ServiceResetFitness;
    ros::ServiceServer service_server_reset;
    ros::NodeHandle n;
    ros::Subscriber subscription_model_state;
public:
  void initialisation(){
    this->position_robot_max = POSITION_ROBOT_MAX;
    this->position_ball_max = POSITION_BALL_MAX;
    this->position_ball_min = POSITION_BALL_MIN;
    this->speed_ball_max = SPEED_BALL_MAX;
    this->speed_ball_min = SPEED_BALL_MIN;
    this->speed_ball_angle_max = SPEED_BALL_ANGLE_MAX;
    this->simTimeMax = SIMTIMEMAX;

    this->service_server_reset = n.advertiseService("/omniROS/resetSimulation",&Simulation::resetService,this);

    this->ServicePauseSimulation = this->n.serviceClient<std_srvs::Empty>("/gazebo/pause_physics");
    this->ServiceUnpauseSimulation = this->n.serviceClient<std_srvs::Empty>("/gazebo/unpause_physics");
    this->ServiceSetModelState = this->n.serviceClient<gazebo_msgs::SetModelState>("/gazebo/set_model_state");
    this->ServiceResetTorque = this->n.serviceClient<std_srvs::Empty>("/omniROS/resetTotalTorque");
    this->ServiceResetFitness = this->n.serviceClient<std_srvs::Empty>("/omniROS/resetTotalFitness");
    this->initial_model_state_robot.request.model_state.model_name="omniROS"; //Remembers the name of the robot so that we don't have to do it each time
    this->initial_model_state_ball.request.model_state.model_name="ball"; //Remembers the name of the robot so that we don't have to do it each time
    //TODO
    this->initial_model_state_robot.request.model_state.pose.position.z=0.0;

  }

  bool resetService(std_srvs::Empty::Request &req,std_srvs::Empty::Response &res){
    //This is a service server that will reset the whole simulation, it's a function that takes a void as input and returns a void as an output
    this->reset();
    return true;
  }

  void reset(){
    //The reset function generates the initial position of the robot and the ball_plugin
    //Teleport them, apply the initial velocity, and remembers it
    ROS_INFO("Reset called \n");
    std_srvs::Empty nothing;
    ros::service::waitForService("/gazebo/pause_physics",-1);
    this->ServicePauseSimulation.call(nothing);
    this->ServiceResetTorque.call(nothing);
    this->ServiceResetFitness.call(nothing);
    //////////////////ROBOT///////////////////

    this->initial_model_state_robot.request.model_state.pose.position.x= -this->position_robot_max + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(2*this->position_robot_max)));   // v3 in the range 1985-2014
    this->initial_model_state_robot.request.model_state.pose.position.y= -this->position_robot_max + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(2*this->position_robot_max)));
    //Create a quaternion from an eulerian angle (Roll Pitch Yaw) so that the only roation is random between -pi and pi around the Z axis
    tf::Quaternion quaternion;
    quaternion.setRPY(0.0,0.0,-M_PI + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(2*M_PI))));
    this->initial_model_state_robot.request.model_state.pose.orientation.x = (double)quaternion[0];
    this->initial_model_state_robot.request.model_state.pose.orientation.y = (double)quaternion[1];
    this->initial_model_state_robot.request.model_state.pose.orientation.z = (double)quaternion[2];
    this->initial_model_state_robot.request.model_state.pose.orientation.w = (double)quaternion[3];
    //Apply the new model state
    this->ServiceSetModelState.call(this->initial_model_state_robot);

    ////////////////////BALL /////////////////
    double x,y;
    do{
      x= -this->position_ball_max +  (((double) rand()) / (double) RAND_MAX)*2*this->position_ball_max;
    }while(abs(x)<this->position_ball_min);
    this->initial_model_state_ball.request.model_state.pose.position.x = this->initial_model_state_robot.request.model_state.pose.position.x + x;

    do{
      y= static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/((2*this->position_ball_max))))-this->position_ball_max;
    }while(abs(y)<this->position_ball_min);

    this->initial_model_state_ball.request.model_state.pose.position.y = this->initial_model_state_robot.request.model_state.pose.position.y + y;

    //Generate an initial velocity that is applied to the ball only once, this velocity must be within the grasp of the robot

    double BRx= this->initial_model_state_robot.request.model_state.pose.position.x - this->initial_model_state_ball.request.model_state.pose.position.x;
    double BRy= this->initial_model_state_robot.request.model_state.pose.position.y - this->initial_model_state_ball.request.model_state.pose.position.y;
    double Vx,Vy;
    double alpha = -this->speed_ball_angle_max+ static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/((2*this->speed_ball_angle_max))));
    Vx=BRx*(cos(alpha)-sin(alpha));
    Vy=BRy*(sin(alpha)+cos(alpha));

    if(Vx < 0 && Vy < 0){
      this->initial_model_state_ball.request.model_state.twist.angular.x=abs(Vy);
      this->initial_model_state_ball.request.model_state.twist.angular.y=-abs(Vx);
    }
    if(Vx < 0 && Vy > 0){
      this->initial_model_state_ball.request.model_state.twist.angular.x=-abs(Vy);
      this->initial_model_state_ball.request.model_state.twist.angular.y=-abs(Vx);
    }
    if(Vx > 0 && Vy > 0){
      this->initial_model_state_ball.request.model_state.twist.angular.x=-abs(Vy);
      this->initial_model_state_ball.request.model_state.twist.angular.y=abs(Vx);
    }
    if(Vx > 0 && Vy < 0){
      this->initial_model_state_ball.request.model_state.twist.angular.x=abs(Vy);
      this->initial_model_state_ball.request.model_state.twist.angular.y=abs(Vx);
    }




    //Apply the new model state
    this->ServiceSetModelState.call(this->initial_model_state_ball);
    //this->ServiceUnpauseSimulation.call(nothing);
    //ros::service::waitForService("/gazebo/pause_physics",-1);

  }

};


int main(int argc, char **argv)
{
  ros::init(argc, argv, "simulation_node");
  Simulation Simulation;
  Simulation.initialisation();
  while (ros::ok())
   {
       ros::spin();
   }
  return 0;
}
