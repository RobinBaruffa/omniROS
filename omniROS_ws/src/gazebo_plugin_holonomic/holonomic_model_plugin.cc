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
#include "control_msgs/PidState.h"

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

      physics::PhysicsEnginePtr physics = this->model->GetWorld()->GetPhysicsEngine();
      const std::string frictionModel = "cone_model";
      physics->SetParam("friction_model", frictionModel);

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
      double pid_output_max = 0.0;
      if(_sdf->HasElement("pid_p")){      //Check if specific pid values are specified in the world file
        pid_p = _sdf->Get<double>("pid_p");
      }
      if(_sdf->HasElement("pid_i")){
        pid_i = _sdf->Get<double>("pid_i");
      }
      if(_sdf->HasElement("pid_d")){
        pid_d = _sdf->Get<double>("pid_d");
      }
      if(_sdf->HasElement("pid_output_max")){
        pid_output_max = _sdf->Get<double>("pid_output_max");
      }
      if(_sdf->HasElement("PIDfrequency")){
        this->PIDfrequency = _sdf->Get<double>("PIDfrequency");
      }

      std::cerr << "P value : "<<std::setprecision(5) <<pid_p << " \n";
      std::cerr << "I value : "<<std::setprecision(5) <<pid_i << " \n";
      std::cerr << "D value : "<<std::setprecision(5) <<pid_d << " \n";
      std::cerr << "max value : "<<std::setprecision(5) <<pid_output_max<< " \n";
      std::cerr << "PIDfrequency : "<<std::setprecision(5) <<this->PIDfrequency<< " \n";

      this->pid = common::PID(pid_p, pid_i, pid_d, 0.0, 0.0, pid_output_max, -pid_output_max);


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
            ros::SubscribeOptions::create<geometry_msgs::Vector3>("/omniROS/vel_w",1,
                boost::bind(&HolonomicPlugin::OnRosMsg, this, _1),
                ros::VoidPtr(), &this->rosQueue);
          this->rosSub = this->rosNode->subscribe(so);
          this->rosPubOdometry = this->rosNode->advertise<nav_msgs::Odometry>("/omniROS/odometry",5);
          this->rosPubWheelSpeed = this->rosNode->advertise<geometry_msgs::Vector3>("/omniROS/vel_w_measured",5);
          this->rosPubPID = this->rosNode->advertise<control_msgs::PidState>("/omniROS/pid",5);


          this->rosQueueThread =
            std::thread(std::bind(&HolonomicPlugin::QueueThread, this));
    }

    // Called by the world update start event
    public: void OnUpdate()
    {

      //Sanity check
      //std::cerr << "Loop is running \n";
      common::Time currTime = this->model->GetWorld()->GetSimTime();
      common::Time stepTime = currTime - this->prevUpdateTime;

      // this->Rotation1->SetVelocity(0, this->vel_target_1);
      // this->Rotation2->SetVelocity(0, this->vel_target_2);
      // this->Rotation3->SetVelocity(0, this->vel_target_3);


      if(stepTime > 1/this->PIDfrequency){

        this->prevUpdateTime = currTime;
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

        this->rosPubOdometry.publish(this->odometry);

        geometry_msgs::Vector3 wheelSpeedMessage;
        double vel_target = this->vel_target_1;
        double vel_measured = this->Rotation1->GetVelocity(0);
        double vel_err = vel_measured - vel_target;
        double effort_cmd = this->pid.Update(vel_err, stepTime);
        effort_cmd = effort_cmd > 1.76 ? 1.76 :
          (effort_cmd < -1.76 ? -1.76 : effort_cmd);
        this->Rotation1->SetForce(0, effort_cmd);
        wheelSpeedMessage.x = vel_measured;

        vel_target = this->vel_target_2;
        vel_measured = this->Rotation2->GetVelocity(0);
        vel_err = vel_measured - vel_target;
        effort_cmd = this->pid.Update(vel_err, stepTime);
        effort_cmd = effort_cmd > 1.76 ? 1.76 :
          (effort_cmd < -1.76 ? -1.76 : effort_cmd);
        this->Rotation2->SetForce(0, effort_cmd);
        wheelSpeedMessage.y = vel_measured;

        vel_target = this->vel_target_3;
        vel_measured = this->Rotation3->GetVelocity(0);
        vel_err = vel_measured - vel_target;
        effort_cmd = this->pid.Update(vel_err, stepTime);
        effort_cmd = effort_cmd > 1.76 ? 1.76 :
          (effort_cmd < -1.76 ? -1.76 : effort_cmd);
        this->Rotation3->SetForce(0, effort_cmd);
        wheelSpeedMessage.z = vel_measured;


        this->PIDmessage.error = vel_err;
        this->PIDmessage.error_dot = vel_target;  //Used as target velocity for debug purposes
        this->PIDmessage.p_error = vel_measured;  //used as measured velocity for debug purposes
        this->PIDmessage.p_term = this->pid.GetPGain();
        this->PIDmessage.i_term = this->pid.GetIGain();
        this->PIDmessage.d_term = this->pid.GetDGain();
        this->PIDmessage.output = effort_cmd;
        this->rosPubPID.publish(this->PIDmessage);
        this->rosPubWheelSpeed.publish(wheelSpeedMessage);

      }


    }
    public: void OnRosMsg(const geometry_msgs::Vector3ConstPtr &_msg)
    {
      // this->model->GetJointController()->SetVelocityTarget(this->Rotation1->GetScopedName(), static_cast<double>(_msg->x));
      // this->model->GetJointController()->SetVelocityTarget(this->Rotation2->GetScopedName(), static_cast<double>(_msg->y));
      // this->model->GetJointController()->SetVelocityTarget(this->Rotation3->GetScopedName(), static_cast<double>(_msg->z));
      this->vel_target_1=_msg->x;
      this->vel_target_2=_msg->y;
      this->vel_target_3=_msg->z;


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
    private: ros::Publisher rosPubOdometry;
    private: ros::Publisher rosPubPID;
    private: ros::Publisher rosPubWheelSpeed;
    private: ros::CallbackQueue rosQueue;
    private: std::thread rosQueueThread;
    public: common::Time prevUpdateTime;
    public: double vel_target_1;
    public: double vel_target_2;
    public: double vel_target_3;
    public: double PIDfrequency;
    public: common::PID pid;
    public: control_msgs::PidState PIDmessage;
    public: physics::JointControllerPtr jointController1;
    public: physics::JointControllerPtr jointController2;
    public: physics::JointControllerPtr jointController3;

    private: nav_msgs::Odometry odometry;

  };

  // Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(HolonomicPlugin)
}
