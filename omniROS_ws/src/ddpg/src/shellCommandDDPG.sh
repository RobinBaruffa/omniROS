#!/bin/sh
#This script is used to bypass the lack of support for python3 in rospack
#It just searches for the ROS ddpg package and start ddpg.py with python3
LOCATION=$(rospack find ddpg)
python3 $LOCATION/src/ddpg.py
