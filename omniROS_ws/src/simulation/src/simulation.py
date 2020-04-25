#!/usr/bin/env python
import rospy
import random
from geometry_msgs.msg import Vector3Stamped,Pose,Vector3,Quaternion,Twist
from gazebo_msgs.srv import SetModelState
from gazebo_msgs.msg import ModelStates,ModelState
from std_srvs.srv import Empty,EmptyRequest

class Class(object):
    simTime = 0.0

    def __init__(self):
         self.subscription_torque = rospy.Subscriber("/omniROS/torque", Vector3Stamped, self.callback1)
         self.subscription_model_state = rospy.Subscriber("/gazebo/model_states", ModelStates, self.callback2)
         self.totalTorque = 0.0
         self.initialBallPosition = [0,0,0]
         self.initialBallVelocity = [0,0,0]
         self.initialRobotPosition = [0,0,0]
         self.simTimeMax = 10.0


    def reset(self):
        rospy.loginfo("Reset CALLED")
        #rospy.wait_for_service('/gazebo/pause_physics')
        callServicePause = rospy.ServiceProxy('/gazebo/pause_physics', Empty)
        callServicePause()
        #self.subscription_torque.unregister()
        #self.subscription_model_state.unregister()

        rospy.wait_for_service('/gazebo/set_model_state')
        callServiceSetModelState = rospy.ServiceProxy('/gazebo/set_model_state', SetModelState)

        model_state = ModelState()
        model_state.model_name = "omniROS"
        # Generate the coordinates for the robot
        for i in range(0,2):
            self.initialRobotPosition[i] = 3+random.random()*3
        model_state.pose = Pose(Vector3(self.initialRobotPosition[0],self.initialRobotPosition[1],0),Quaternion(0,0,0,0))
        model_state.twist = Twist(Vector3(self.initialRobotPosition[0],self.initialRobotPosition[1],0),Vector3(0,0,0))
        rospy.wait_for_service('/gazebo/set_model_state')
        callServiceSetModelState(model_state)
        # Generate the coordinates for the ball
        model_state.model_name = "ball"
        for i in range(0,2):
            self.initialBallPosition[i] = 3+random.random()*3
        model_state.pose = Pose(Vector3(self.initialBallPosition[0],self.initialBallPosition[1],0),Quaternion(0,0,0,0))
        model_state.twist = Twist(Vector3(self.initialBallPosition[0],self.initialBallPosition[1],0),Vector3(0,0,0))
        rospy.wait_for_service('/gazebo/set_model_state')
        callServiceSetModelState(model_state)

        #rospy.wait_for_service('/gazebo/unpause_physics')
        callServiceUnpause = rospy.ServiceProxy('/gazebo/unpause_physics', Empty)
        callServiceUnpause()

    def callback1(self,torque):
        self.totalTorque = self.totalTorque + abs(torque.vector.x)+ abs(torque.vector.y)+ abs(torque.vector.z)
        rospy.loginfo("Total torque :  %s \n",self.totalTorque)
        rospy.loginfo("Simulaton time :  %s \n",torque.header.stamp.secs)
        rospy.loginfo("Simulaton time limit :  %s \n",self.simTime + self.simTimeMax)
        if torque.header.stamp.secs > self.simTime + self.simTimeMax:
            self.simTime = torque.header.stamp.secs
            self.reset()

            #self.main()

    def callback2(self,models_states):
        a = 2


    def main(self):
        while not rospy.is_shutdown():
            rospy.spin()
            #subscription_models_states.unregister()
if __name__ == "__main__":
    rospy.init_node('simulation_node')
    my_node = Class()
    my_node.main()
