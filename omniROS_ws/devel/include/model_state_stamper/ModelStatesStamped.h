// Generated by gencpp from file model_state_stamper/ModelStatesStamped.msg
// DO NOT EDIT!


#ifndef MODEL_STATE_STAMPER_MESSAGE_MODELSTATESSTAMPED_H
#define MODEL_STATE_STAMPER_MESSAGE_MODELSTATESSTAMPED_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>

#include <std_msgs/Header.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Twist.h>

namespace model_state_stamper
{
template <class ContainerAllocator>
struct ModelStatesStamped_
{
  typedef ModelStatesStamped_<ContainerAllocator> Type;

  ModelStatesStamped_()
    : header()
    , name()
    , pose()
    , twist()  {
    }
  ModelStatesStamped_(const ContainerAllocator& _alloc)
    : header(_alloc)
    , name(_alloc)
    , pose(_alloc)
    , twist(_alloc)  {
  (void)_alloc;
    }



   typedef  ::std_msgs::Header_<ContainerAllocator>  _header_type;
  _header_type header;

   typedef std::vector<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > , typename ContainerAllocator::template rebind<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::other >  _name_type;
  _name_type name;

   typedef std::vector< ::geometry_msgs::Pose_<ContainerAllocator> , typename ContainerAllocator::template rebind< ::geometry_msgs::Pose_<ContainerAllocator> >::other >  _pose_type;
  _pose_type pose;

   typedef std::vector< ::geometry_msgs::Twist_<ContainerAllocator> , typename ContainerAllocator::template rebind< ::geometry_msgs::Twist_<ContainerAllocator> >::other >  _twist_type;
  _twist_type twist;





  typedef boost::shared_ptr< ::model_state_stamper::ModelStatesStamped_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::model_state_stamper::ModelStatesStamped_<ContainerAllocator> const> ConstPtr;

}; // struct ModelStatesStamped_

typedef ::model_state_stamper::ModelStatesStamped_<std::allocator<void> > ModelStatesStamped;

typedef boost::shared_ptr< ::model_state_stamper::ModelStatesStamped > ModelStatesStampedPtr;
typedef boost::shared_ptr< ::model_state_stamper::ModelStatesStamped const> ModelStatesStampedConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::model_state_stamper::ModelStatesStamped_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::model_state_stamper::ModelStatesStamped_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace model_state_stamper

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': False, 'IsMessage': True, 'HasHeader': True}
// {'std_msgs': ['/opt/ros/kinetic/share/std_msgs/cmake/../msg'], 'model_state_stamper': ['/home/user/omniROS/omniROS_ws/src/model_state_stamper/msg'], 'geometry_msgs': ['/opt/ros/kinetic/share/geometry_msgs/cmake/../msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::model_state_stamper::ModelStatesStamped_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::model_state_stamper::ModelStatesStamped_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::model_state_stamper::ModelStatesStamped_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::model_state_stamper::ModelStatesStamped_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::model_state_stamper::ModelStatesStamped_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::model_state_stamper::ModelStatesStamped_<ContainerAllocator> const>
  : TrueType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::model_state_stamper::ModelStatesStamped_<ContainerAllocator> >
{
  static const char* value()
  {
    return "9af1f97108e4fbd46176fb2e4dc1375b";
  }

  static const char* value(const ::model_state_stamper::ModelStatesStamped_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x9af1f97108e4fbd4ULL;
  static const uint64_t static_value2 = 0x6176fb2e4dc1375bULL;
};

template<class ContainerAllocator>
struct DataType< ::model_state_stamper::ModelStatesStamped_<ContainerAllocator> >
{
  static const char* value()
  {
    return "model_state_stamper/ModelStatesStamped";
  }

  static const char* value(const ::model_state_stamper::ModelStatesStamped_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::model_state_stamper::ModelStatesStamped_<ContainerAllocator> >
{
  static const char* value()
  {
    return "std_msgs/Header header\n\
string[] name\n\
geometry_msgs/Pose[] pose\n\
geometry_msgs/Twist[] twist\n\
\n\
================================================================================\n\
MSG: std_msgs/Header\n\
# Standard metadata for higher-level stamped data types.\n\
# This is generally used to communicate timestamped data \n\
# in a particular coordinate frame.\n\
# \n\
# sequence ID: consecutively increasing ID \n\
uint32 seq\n\
#Two-integer timestamp that is expressed as:\n\
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')\n\
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')\n\
# time-handling sugar is provided by the client library\n\
time stamp\n\
#Frame this data is associated with\n\
# 0: no frame\n\
# 1: global frame\n\
string frame_id\n\
\n\
================================================================================\n\
MSG: geometry_msgs/Pose\n\
# A representation of pose in free space, composed of position and orientation. \n\
Point position\n\
Quaternion orientation\n\
\n\
================================================================================\n\
MSG: geometry_msgs/Point\n\
# This contains the position of a point in free space\n\
float64 x\n\
float64 y\n\
float64 z\n\
\n\
================================================================================\n\
MSG: geometry_msgs/Quaternion\n\
# This represents an orientation in free space in quaternion form.\n\
\n\
float64 x\n\
float64 y\n\
float64 z\n\
float64 w\n\
\n\
================================================================================\n\
MSG: geometry_msgs/Twist\n\
# This expresses velocity in free space broken into its linear and angular parts.\n\
Vector3  linear\n\
Vector3  angular\n\
\n\
================================================================================\n\
MSG: geometry_msgs/Vector3\n\
# This represents a vector in free space. \n\
# It is only meant to represent a direction. Therefore, it does not\n\
# make sense to apply a translation to it (e.g., when applying a \n\
# generic rigid transformation to a Vector3, tf2 will only apply the\n\
# rotation). If you want your data to be translatable too, use the\n\
# geometry_msgs/Point message instead.\n\
\n\
float64 x\n\
float64 y\n\
float64 z\n\
";
  }

  static const char* value(const ::model_state_stamper::ModelStatesStamped_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::model_state_stamper::ModelStatesStamped_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.header);
      stream.next(m.name);
      stream.next(m.pose);
      stream.next(m.twist);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct ModelStatesStamped_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::model_state_stamper::ModelStatesStamped_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::model_state_stamper::ModelStatesStamped_<ContainerAllocator>& v)
  {
    s << indent << "header: ";
    s << std::endl;
    Printer< ::std_msgs::Header_<ContainerAllocator> >::stream(s, indent + "  ", v.header);
    s << indent << "name[]" << std::endl;
    for (size_t i = 0; i < v.name.size(); ++i)
    {
      s << indent << "  name[" << i << "]: ";
      Printer<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::stream(s, indent + "  ", v.name[i]);
    }
    s << indent << "pose[]" << std::endl;
    for (size_t i = 0; i < v.pose.size(); ++i)
    {
      s << indent << "  pose[" << i << "]: ";
      s << std::endl;
      s << indent;
      Printer< ::geometry_msgs::Pose_<ContainerAllocator> >::stream(s, indent + "    ", v.pose[i]);
    }
    s << indent << "twist[]" << std::endl;
    for (size_t i = 0; i < v.twist.size(); ++i)
    {
      s << indent << "  twist[" << i << "]: ";
      s << std::endl;
      s << indent;
      Printer< ::geometry_msgs::Twist_<ContainerAllocator> >::stream(s, indent + "    ", v.twist[i]);
    }
  }
};

} // namespace message_operations
} // namespace ros

#endif // MODEL_STATE_STAMPER_MESSAGE_MODELSTATESSTAMPED_H
