#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>

namespace gazebo
{
  /// \brief A plugin to control a Velodyne sensor.
  class LidarPlugin : public ModelPlugin
  {
    /// \brief Constructor
    public: LidarPlugin() {}

    /// \brief The load function is called by Gazebo when the plugin is
    /// inserted into simulation
    /// \param[in] _model A pointer to the model that this plugin is
    /// attached to.
    /// \param[in] _sdf A pointer to the plugin's SDF element.
    public: virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
    {
      // Just output a message for now
      if (_model->GetJointCount() == 0)
  {
    std::cerr << "Invalid joint count, Velodyne plugin not loaded\n";
    return;
  }
  this->model = _model;
  std::cerr << "lidar_plugin is controlling : " << this->model->GetName() << "\n";

  // Store the model pointer for convenience.

  this->link= this->model->GetLink("LidarHead");
  // Get the first joint. We are making an assumption about the model
  // having one joint that is the rotational joint.
  this->joint = _model->GetJoint("LidarHeadJoint");
  math::Vector3 rotationFrequency(0,0,7);
  this->link->SetAngularVel(rotationFrequency);
    }
    private: physics::ModelPtr model;
    /// \brief Pointer to the joint.
    private: physics::JointPtr joint;

    private: physics::LinkPtr link;
  };

  /// \brief Pointer to the model.


  // Tell Gazebo about this plugin, so that Gazebo can call Load on this plugin.
  GZ_REGISTER_MODEL_PLUGIN(LidarPlugin)
}
