//
//  A small plugin used to broadcast the number of iteration, real time update rate,  as a ROS topic
//  To run the plugin $/build cmake ../ && make

#include "ros/ros.h"
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include "std_msgs/UInt64.h"

namespace gazebo
{
  class SimulationPlugin : public WorldPlugin
  {
    public: SimulationPlugin() {}
    public: void Load(physics::WorldPtr _parent, sdf::ElementPtr /*_sdf*/)
    {
      this->updateConnection = event::Events::ConnectWorldUpdateBegin(std::bind(&SimulationPlugin::OnUpdate, this));
          if (!ros::isInitialized())
          {
            int argc = 0;
            char **argv = NULL;
            ros::init(argc, argv, "gazebo_client",ros::init_options::NoSigintHandler);
          }
      this->rosNode.reset(new ros::NodeHandle("gazebo_client"));

      this->world = _parent;
      this->rosPub = this->rosNode->advertise<std_msgs::UInt64>("/omniROS/nbIterations",5);

    }
    public: void OnUpdate()
    {
      std_msgs::UInt64 nbIterations;
      nbIterations.data = this->world->GetIterations();
      this->rosPub.publish(nbIterations);
    }
    public: event::ConnectionPtr updateConnection;
    public: std::unique_ptr<ros::NodeHandle> rosNode;
    private: physics::WorldPtr world;
    private: ros::Publisher rosPub;
  };
  GZ_REGISTER_WORLD_PLUGIN(SimulationPlugin)
}
