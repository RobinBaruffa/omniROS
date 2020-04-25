//the path of the plugin must be known by gazebo through an environment variable, and this needs to be set at each terminal startup
//So you need to add the following to the .bashrc file at the root of your home directory
//nano ~.bashrc
//and add on the bottom
//export GAZEBO_PLUGIN_PATH=${GAZEBO_PLUGIN_PATH}:~/home/omniROS/omniROS_ws/src/gazebo_plugin_holonomic/build

//ROS compatibility example comes from : http://gazebosim.org/tutorials?tut=guided_i6#include <map>
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


using std::fixed;

namespace gazebo
{
  class HolonomicPlugin : public ModelPlugin
  {

    public: HolonomicPlugin() {}
    public: void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
    {

      this->model = _model;

      // this->link= this->model->GetLink("LidarHead");
      //
      // std::cerr << "\nThe omniROS plugin is attach to the link : [" <<this->link->GetName() << "]\n";


      this->Rotation1 = _model->GetJoint("Rotation1");
      this->Rotation2 = _model->GetJoint("Rotation2");
      this->Rotation3 = _model->GetJoint("Rotation3");

      std::cerr << "\nThe omniROS plugin is attach to model[" <<_model->GetName() << "]\n";
      std::cerr << "\nRotation1 is linked to : [" <<this->Rotation1->GetName() << "]\n";
      std::cerr << "\nRotation2 is linked to : [" <<this->Rotation2->GetName() << "]\n";
      std::cerr << "\nRotation3 is linked to : [" <<this->Rotation3->GetName() << "]\n";
      std::cerr << "\nRotation1 scope name is : [" <<this->Rotation1->GetScopedName() << "]\n";

      double pid_p = 0.0;
      double pid_i = 0.0;
      double pid_d = 0.0;
      if(_sdf->HasElement("pid_p")){      //Check if specific pid values are specified in the world file
        pid_p = _sdf->Get<double>("pid_p");
      }
      if(_sdf->HasElement("pid_i")){
        pid_i = _sdf->Get<double>("pid_i");
      }
      if(_sdf->HasElement("pid_d")){
        pid_d = _sdf->Get<double>("pid_d");
      }

      std::cerr << "P value : "<<std::setprecision(5) <<pid_p << " \n";
      std::cerr << "I value : "<<std::setprecision(5) <<pid_i << " \n";
      std::cerr << "D value : "<<std::setprecision(5) <<pid_d << " \n";

      this->pid = common::PID(pid_p, pid_i, pid_d, 0.0, 0.0, 1.76, -1.76);
//
// // Set the joint's target velocity. This target velocity is just
// // for demonstration purposes.
//
//       this->model->GetJointController()->SetVelocityPID(this->Rotation1->GetScopedName(), this->pid);
//
//       this->model->GetJointController()->SetVelocityPID(this->Rotation2->GetScopedName(), this->pid);
//
//       this->model->GetJointController()->SetVelocityPID(this->Rotation3->GetScopedName(), this->pid);

      this->jointController1.reset(new physics::JointController(
            this->model));
      this->jointController1->AddJoint(this->Rotation1);
      this->jointController1->SetVelocityPID(this->Rotation1->GetScopedName(), this->pid);

      this->jointController2.reset(new physics::JointController(
            this->model));
      this->jointController2->AddJoint(this->Rotation2);
      this->jointController2->SetVelocityPID(this->Rotation2->GetScopedName(), this->pid);

      this->jointController3.reset(new physics::JointController(
            this->model));
      this->jointController3->AddJoint(this->Rotation3);
      this->jointController3->SetVelocityPID(this->Rotation3->GetScopedName(), this->pid);

      this->jointController1->SetVelocityTarget(this->Rotation1->GetScopedName(),0.0);
      this->jointController2->SetVelocityTarget(this->Rotation2->GetScopedName(),0.0);
      this->jointController3->SetVelocityTarget(this->Rotation3->GetScopedName(),0.0);

      this->updateConnection = event::Events::ConnectWorldUpdateBegin(std::bind(&HolonomicPlugin::OnUpdate, this));
          if (!ros::isInitialized())
          {
            int argc = 0;
            char **argv = NULL;
            ros::init(argc, argv, "gazebo_client",
                ros::init_options::NoSigintHandler);
          }
          // Create our ROS node. This acts in a similar manner to
          // the Gazebo node
          this->rosNode.reset(new ros::NodeHandle("gazebo_client"));
          // Create a named topic, and subscribe to it.
          ros::SubscribeOptions so =
            ros::SubscribeOptions::create<geometry_msgs::Vector3>(
                "/" + this->model->GetName() + "/vel_w",
                1,
                boost::bind(&HolonomicPlugin::OnRosMsg, this, _1),
                ros::VoidPtr(), &this->rosQueue);
          this->rosSub = this->rosNode->subscribe(so);
          this->rosPub = this->rosNode->advertise<nav_msgs::Odometry>("/" + this->model->GetName() + "/odometry",5);

          this->rosQueueThread =
            std::thread(std::bind(&HolonomicPlugin::QueueThread, this));
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

      math::Pose PoseVector(this->model->GetWorldPose());
      this->odometry.pose.pose.position.x = PoseVector.pos.x;
      this->odometry.pose.pose.position.y = PoseVector.pos.y;
      this->odometry.pose.pose.position.z = PoseVector.pos.z;
      this->odometry.pose.pose.orientation.x = PoseVector.rot.x;
      this->odometry.pose.pose.orientation.y = PoseVector.rot.y;
      this->odometry.pose.pose.orientation.z = PoseVector.rot.z;

      this->IterationCounter = this->IterationCounter + 1;
      if(this->IterationCounter > 17){
        this->IterationCounter = 0;
          //max_step_size default is 0.001
        this->jointController1->Update();
        this->jointController2->Update();
        this->jointController3->Update();
        std::cerr << "####### Rotation 1 ######## \n";
        std::cerr << "Force  : "<<std::setprecision(5) <<this->Rotation1->GetForceTorque(0).body1Torque<< " Newtons\n";
        std::cerr << "Velocity  measured : "<<std::setprecision(5) <<this->Rotation1->GetVelocity(0)<< " m/s\n";
        std::cerr << "Velocity  PID target : "<<std::setprecision(5) <<this->jointController1->GetVelocities()["Rotation1"]<< " m/s\n";
        std::cerr << " name of joint : " << this->jointController1->GetJoints()["Rotation1"]<< "\n";


        std::cerr << "####### Rotation 2 ######## \n";
        std::cerr << "Force  : "<<std::setprecision(5) <<this->Rotation2->GetForceTorque(0).body1Torque<< " Newtons\n";
        std::cerr << "Velocity  measure : "<<std::setprecision(5) <<this->Rotation2->GetVelocity(0)<< " m/s\n";
        std::cerr << "Velocity  PID target : "<<std::setprecision(5) <<this->jointController2->GetVelocities()["Rotation2"]<< " m/s\n";

        std::cerr << "####### Rotation 3 ######## \n";
        std::cerr << "Force  : "<<std::setprecision(5) <<this->Rotation3->GetForceTorque(0).body1Torque<< " Newtons\n";
        std::cerr << "Velocity  measure : "<<std::setprecision(5) <<this->Rotation3->GetVelocity(0)<< " m/s\n";
        std::cerr << "Velocity  PID target : "<<std::setprecision(5) <<this->jointController3->GetVelocities()["Rotation3"]<< " m/s\n";


      }


      //std::cerr << "Simulation time : " << std::setprecision(5)<<this->World->GetSimTime()<<"\n";
      this->rosPub.publish(this->odometry);

    }
    public: void OnRosMsg(const geometry_msgs::Vector3ConstPtr &_msg)
    {
      // this->model->GetJointController()->SetVelocityTarget(this->Rotation1->GetScopedName(), static_cast<double>(_msg->x));
      // this->model->GetJointController()->SetVelocityTarget(this->Rotation2->GetScopedName(), static_cast<double>(_msg->y));
      // this->model->GetJointController()->SetVelocityTarget(this->Rotation3->GetScopedName(), static_cast<double>(_msg->z));
      this->jointController1->SetVelocityTarget(this->Rotation1->GetScopedName(),_msg->x);
      this->jointController2->SetVelocityTarget(this->Rotation2->GetScopedName(),_msg->y);
      this->jointController3->SetVelocityTarget(this->Rotation3->GetScopedName(),_msg->z);


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
    public: std::int16_t IterationCounter;
    public: physics::ModelPtr model;
    public: physics::LinkPtr link;
    public: physics::JointPtr Rotation1;
    public: physics::JointPtr Rotation2;
    public: physics::JointPtr Rotation3;
    public: physics::JointControllerPtr controller;
    public: event::ConnectionPtr updateConnection;
    public: std::unique_ptr<ros::NodeHandle> rosNode;

    private: ros::Subscriber rosSub;
    private: ros::Publisher rosPub;
    private: ros::CallbackQueue rosQueue;
    private: std::thread rosQueueThread;


    public: common::PID pid;
    public: physics::JointControllerPtr jointController1;
    public: physics::JointControllerPtr jointController2;
    public: physics::JointControllerPtr jointController3;

    private: nav_msgs::Odometry odometry;

  };

  // Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(HolonomicPlugin)
}
