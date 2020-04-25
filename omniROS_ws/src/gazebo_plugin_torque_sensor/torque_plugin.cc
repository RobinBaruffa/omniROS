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
#include "geometry_msgs/Vector3Stamped.h"
#include "ros/advertise_options.h"

//To run the plugin $/build cmake ../ && make
#include <functional>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <ignition/math/Vector3.hh>
#include <iostream>

namespace gazebo
{
  class TorquePlugin : public ModelPlugin
  {

    public: TorquePlugin() {}
    public: void Load(physics::ModelPtr _model, sdf::ElementPtr /*_sdf*/)
    {
      this->model = _model;
      this->Rotation1 = _model->GetJoint("Rotation1");
      this->Rotation2 = _model->GetJoint("Rotation2");
      this->Rotation3 = _model->GetJoint("Rotation3");

      this->updateConnection = event::Events::ConnectWorldUpdateBegin(std::bind(&TorquePlugin::OnUpdate, this));
          if (!ros::isInitialized())
          {
            int argc = 0;
            char **argv = NULL;
            ros::init(argc, argv, "gazebo_client",ros::init_options::NoSigintHandler);
          }
      this->rosNode.reset(new ros::NodeHandle("gazebo_client"));
          // Create a named topic, and subscribe to it.

      this->rosPub = this->rosNode->advertise<geometry_msgs::Vector3Stamped>("/" + this->model->GetName() + "/torque",5);
      this->rosQueueThread =
        std::thread(std::bind(&TorquePlugin::QueueThread, this));
    }

    // Called by the world update start event
    public: void OnUpdate()
    {

      this->vector3.vector.x = this->Rotation1->GetForceTorque(0).body1Torque[2];
      this->vector3.vector.y = this->Rotation2->GetForceTorque(0).body1Torque[2];
      this->vector3.vector.z = this->Rotation3->GetForceTorque(0).body1Torque[2];
      gazebo::common::Time timeVar = this->model->GetWorld()->GetSimTime();
      //std::cerr << "World Simulation time :    " << timeVar.sec <<"\n";
      this->vector3.header.stamp.sec = timeVar.sec;
      this->vector3.header.stamp.nsec = timeVar.nsec;
      this->rosPub.publish(this->vector3);


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
    public: geometry_msgs::Vector3Stamped vector3;
    private: ros::Publisher rosPub;
    private: ros::CallbackQueue rosQueue;
    private: std::thread rosQueueThread;

    public: physics::JointPtr Rotation1;
    public: physics::JointPtr Rotation2;
    public: physics::JointPtr Rotation3;


  };

  // Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(TorquePlugin)
}
