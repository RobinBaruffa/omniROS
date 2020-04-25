#include <string>

#include <gazebo/gazebo.hh>
#include <gazebo/sensors/sensors.hh>


namespace gazebo
{
  class ContactPlugin : public SensorPlugin
  {
    public: ContactPlugin() {}

    public: void Load(sensors::SensorPtr _sensor, sdf::ElementPtr /*_sdf*/)
    {
      this->parentSensor = std::dynamic_pointer_cast<sensors::ContactSensor>(_sensor);
      // Make sure the parent sensor is valid.
      if (!this->parentSensor)
      {
        gzerr << "ContactPlugin requires a ContactSensor.\n";
        return;
      }
      std::cout << "ContactPlugin successfully initialized \n";
      // Connect to the sensor update event.
      this->updateConnection = this->parentSensor->ConnectUpdated(
          std::bind(&ContactPlugin::OnUpdate, this));
      // Make sure the parent sensor is active.
      this->parentSensor->SetActive(true);
    }

    public: void OnUpdate()
    {
      // Get all the contacts.
      msgs::Contacts contacts;
      contacts = this->parentSensor->Contacts();
      for (unsigned int i = 0; i < contacts.contact_size(); ++i)
      {
        std::cout << "Collision between[" << contacts.contact(i).collision1()
                  << "] and [" << contacts.contact(i).collision2() << "]\n";


      }
    }
    private: event::ConnectionPtr updateConnection;
    private: sensors::ContactSensorPtr parentSensor;
  };



// Register this plugin with the simulator
GZ_REGISTER_SENSOR_PLUGIN(ContactPlugin)
}
