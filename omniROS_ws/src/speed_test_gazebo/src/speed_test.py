#!/usr/bin/env python

import sys
import rospy
from time import sleep
from std_srvs.srv import Empty
from gazebo_msgs.srv import GetWorldProperties

def pauseSim():
    rospy.wait_for_service('/gazebo/pause_physics')
    try:
        pauseSim = rospy.ServiceProxy('/gazebo/pause_physics', Empty)
        #print("\n paused")
        return pauseSim()
    except rospy.ServiceException, e:
        print "Service call failed: %s"%e

def unpauseSim():
    rospy.wait_for_service('/gazebo/unpause_physics')
    try:
        unpauseSim = rospy.ServiceProxy('/gazebo/unpause_physics', Empty)
        #print("\n upaused")
        return unpauseSim()
    except rospy.ServiceException, e:
        print "Service call failed: %s"%e

def getSimTime():
    rospy.wait_for_service('/gazebo/get_world_properties')
    try:
        serviceSimTime = rospy.ServiceProxy('/gazebo/get_world_properties', GetWorldProperties)
        response = serviceSimTime()
        return response.sim_time
    except rospy.ServiceException, e:
        print "Service call failed: %s"%e

if __name__ == "__main__":
    oldSimTime = 0
    while(True):
        simTime =getSimTime()
        unpauseSim()
        pauseSim()
        sleep(7)
        unpauseSim()
        pauseSim()

        #print("\n Simulation time : %f" %(simTime - oldSimTime))
        #oldSimTime = simTime
        #sleep(2)
