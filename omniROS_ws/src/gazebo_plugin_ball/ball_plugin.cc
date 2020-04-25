//the path of the plugin must be known by gazebo through an environment variable, and this needs to be set at each terminal startup
//So you need to add the following to the .bashrc file at the root of your home directory
//nano ~.bashrc
//and add on the bottom
//export GAZEBO_PLUGIN_PATH=${GAZEBO_PLUGIN_PATH}:~/home/omniROS/omniROS_ws/src/gazebo_plugin_holonomic/build

//ROS compatibility example comes from : http://gazebosim.org/tutorials?tut=guided_i6
//Include ROS-related stuff
#include <thread>
#include "ros/ros.h"
#include "ros/callback_queue.h"
#include "ros/subscribe_options.h"
#include "geometry_msgs/Vector3.h"
#include "ros/advertise_options.h"
#include "nav_msgs/Odometry.h"

//To run the plugin $/build cmake ../ && make
#include <functional>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <ignition/math/Vector3.hh>
#include <iostream>

namespace gazebo
{
  class BallPlugin : public ModelPlugin
  {

    public: BallPlugin() {}
    public: void Load(physics::ModelPtr _model, sdf::ElementPtr /*_sdf*/)
    {
      this->model = _model;
      this->link= this->model->GetLinks()[0];

      this->updateConnection = event::Events::ConnectWorldUpdateBegin(std::bind(&BallPlugin::OnUpdate, this));
          if (!ros::isInitialized())
          {
            int argc = 0;
            char **argv = NULL;
            ros::init(argc, argv, "gazebo_client",ros::init_options::NoSigintHandler);
          }
      this->rosNode.reset(new ros::NodeHandle("gazebo_client"));
          // Create a named topic, and subscribe to it.
      ros::SubscribeOptions so =
          ros::SubscribeOptions::create<geometry_msgs::Vector3>(
                "/" + this->model->GetName() + "/ball_force",
                1,
                boost::bind(&BallPlugin::OnRosMsg, this, _1),
                ros::VoidPtr(), &this->rosQueue);
      this->rosSub = this->rosNode->subscribe(so);

      this->rosPub = this->rosNode->advertise<nav_msgs::Odometry>("/" + this->model->GetName() + "/odometry",5);
      this->rosQueueThread =
        std::thread(std::bind(&BallPlugin::QueueThread, this));
    }

    // Called by the world update start event
    public: void OnUpdate()
    {
      math::Vector3 Vector3DLinear(this->model->GetWorldLinearVel());
      this->odometry.twist.twist.linear.x = Vector3DLinear[0];
      this->odometry.twist.twist.linear.y = Vector3DLinear[1];
      this->odometry.twist.twist.linear.z = Vector3DLinear[2];

      math::Vector3 Vector3DAngular(this->model->GetWorldAngularVel());
      this->odometry.twist.twist.angular.x = Vector3DAngular[0];
      this->odometry.twist.twist.angular.y = Vector3DAngular[1];
      this->odometry.twist.twist.angular.z = Vector3DAngular[2];

      math::Pose PoseVector(this->link->GetWorldPose());
      this->odometry.pose.pose.position.x = PoseVector.pos.x;
      this->odometry.pose.pose.position.y = PoseVector.pos.y;
      this->odometry.pose.pose.position.z = PoseVector.pos.z;
      this->odometry.pose.pose.orientation.x = PoseVector.rot.x;
      this->odometry.pose.pose.orientation.y = PoseVector.rot.y;
      this->odometry.pose.pose.orientation.z = PoseVector.rot.z;

      this->rosPub.publish(this->odometry);

      std::cerr << "Velocity on link  : "<<std::setprecision(5) <<PoseVector.pos.x << " m/s\n";

    }


    public: void OnRosMsg(const geometry_msgs::Vector3ConstPtr &_msg)
    {
      std::cerr << "message received\n";
      ignition::math::Vector3d punchVelocity(_msg->x,_msg->y,_msg->z);
      this->link->SetForce(punchVelocity);
    }

    /// \brief ROS helper function that processes messages
    private: void QueueThread()
    {
      static const double timeout = 0.01;
      while (this->rosNode->ok())
      {
        this->rosQueue.callAvailable(ros::WallDuration(timeout));
      }
    }

    public: physics::ModelPtr model;
    public: physics::LinkPtr link;
    public: physics::EntityPtr entity;
    public: event::ConnectionPtr updateConnection;
    public: std::unique_ptr<ros::NodeHandle> rosNode;

    private: ros::Subscriber rosSub;
    private: ros::Publisher rosPub;
    private: ros::CallbackQueue rosQueue;
    private: std::thread rosQueueThread;

    private: nav_msgs::Odometry odometry;

  };

  // Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(BallPlugin)
}
