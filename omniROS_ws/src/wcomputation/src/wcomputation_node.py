#!/usr/bin/env python
import rospy

from math import *

from geometry_msgs.msg import Vector3
from geometry_msgs.msg import Point
from geometry_msgs.msg import Twist


def compute(msg):
    result = Vector3()
    r= 0.029
    R=0.125
    sinpi3 = 0.86602

    result.x = (-msg.linear.x + R * msg.angular.z) / r;
    result.y = (-sinpi3 * msg.linear.y + 0.5 *  (msg.linear.x) + R *msg.angular.z) / r;
    result.z = (sinpi3 * msg.linear.y + 0.5 * (msg.linear.x) + R * msg.angular.z) / r;

    '''
    result.x = (msg.linear.y +R *msg.angular.z) / r;
    result.y = (-sinpi3 * msg.linear.x - 0.5 * msg.linear.y + R * msg.angular.z) / r;
    result.z = (sinpi3 * msg.linear.x - 0.5 * msg.linear.y + R * msg.angular.z) / r;
    '''
    pub.publish(result)
    rospy.loginfo(result)
    return 0

def main():
    rospy.init_node('wcomputation_node')
    global pub
    pub= rospy.Publisher('/omniROS/vel_w', Vector3, queue_size=10)
    rospy.Subscriber("/omniROS/cmd_vel",Twist ,compute)

    rospy.spin()

if __name__ == "__main__":
    try:
        main()
    except rospy.ROSInterruptException:
        pass
