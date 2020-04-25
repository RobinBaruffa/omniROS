# -*- coding: utf-8 -*-

import math
import gym
from gym import spaces
from gym.utils import seeding
import numpy as np
from time import sleep

import sys
import rospy

from trollius import From
import pygazebo
import pygazebo.msg.world_control_pb2

#Import the necessary ROS messages in order to gather observation, fitness, and send actions

from std_srvs.srv import Empty
from geometry_msgs.msg import Twist
from sensor_msgs.msg import LaserScan
from std_msgs.msg import Float32

class OmniROSEnv(gym.Env):
    metadata = {
        'render.modes': ['human', 'rgb_array'],
        'video.frames_per_second': 30
    }

    def __init__(self):

        rospy.init_node('ddpg_node', anonymous=False)
        self.subscription_fitness = rospy.Subscriber("/omniROS/fitness", Float32, self.callbackFitness)
        self.subscription_lidar = rospy.Subscriber("/omniROS/lidarScan", LaserScan, self.callbackLaserScan)
        self.publisher_action = rospy.Publisher("/omniROS/cmd_vel", Twist, queue_size=1)
        self.pause_service = rospy.ServiceProxy('/gazebo/pause_physics', Empty)
        self.unpause_service = rospy.ServiceProxy('/gazebo/unpause_physics', Empty)
        self.reset_service = rospy.ServiceProxy('/omniROS/resetSimulation', Empty)

        self.fitness = np.float64(0.0)
        self.lidar = np.full((360, 1), 0.0)#np.ndarray(shape=(360,0), dtype=float)

        self.Vx_max_action = 1.0
        self.Vy_max_action = 1.0
        self.Vz_max_action = 6.0    #In radians/sec

        self.lidar_distance_min = 0.0
        self.lidar_distance_max = 10.0

        self.goal_position = 0.05 # was 0.5 in gym, 0.45 in Arnaud de Broissia's version
        self.power = 0.0015

        self.viewer = None
        self.action_space = spaces.Box( np.array([-self.Vx_max_action,-self.Vy_max_action,-self.Vz_max_action]), np.array([self.Vx_max_action,self.Vy_max_action,self.Vz_max_action]))
        self.observation_space = spaces.Box(self.lidar_distance_min, self.lidar_distance_max, shape=(360,), dtype=np.float32)

        self.i = 0

        self.seed()
        self.reset()


    def seed(self, seed=None):
        self.np_random, seed = seeding.np_random(seed)
        return [seed]

    def callbackFitness(self,msgFitness):
        self.fitness = np.float64(msgFitness.data)

    def callbackLaserScan(self,msgScan):
        self.lidar = np.float64(msgScan.ranges)
        self.lidar[np.isinf(self.lidar)] = 0    #Replace all np.inf caused by the lidar not seeing anything by zero

    def pauseSim(self):
        rospy.wait_for_service('/gazebo/pause_physics')
        self.pause_service()

    def unpauseSim(self):
        rospy.wait_for_service('/gazebo/unpause_physics')
        self.unpause_service()

    def step(self, action):
        self.i = self.i + 1
        if rospy.is_shutdown():
            exit()
        print("\nStep n°%d\n"%(self.i))
        print("Observation : ")
        print(self.lidar)
        print("\nAction : ")
        print(action)

        msg = Twist()
        if(not np.isnan(action[0][0])):
            msg.linear.x = action[0][0]
        if(not np.isnan(action[0][1])):
            msg.linear.y = action[0][1]
        if(not np.isnan(action[0][2])):
            msg.angular.z = action[0][2]

        self.publisher_action.publish(msg)

        message = pygazebo.msg.world_control_pb2.WorldControl()
        message.pause = True
        message.multi_step = 100
        #yield From(publisherStep.publish(message))

        done = False
        done = bool(self.fitness >= 4.0)
        if done:
            self.fitness =+ 10.0

        return self.lidar, self.fitness, done, {}

    def reset(self):
        rospy.wait_for_service('/omniROS/resetSimulation')
        self.reset_service()
        self.state = self.lidar
        self.i =0
        self.unpauseSim()
        return np.array(self.state)

    def close(self):
        if self.viewer:
            self.viewer.close()
            self.viewer = None
