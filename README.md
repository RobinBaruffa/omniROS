#   omniROS
omniROS is an omni-wheel robotic project with embedded LIDAR sensor aiming to create an evolutive robotic platform to experiment with Reinforcement Learning, based on ROS (Robot Operating System) and Gazebo simulator.

# Overview of the robot

![](https://raw.githubusercontent.com/RobinBaruffa/omniROS/master/Documentation%20and%20misc/omniROS_picture.JPG)

# Specifications
It's a three omni wheel (aka holonomic or mecanuum wheels) robot composed of an ESP8266, an Arduino Nano, a Raspberry PI 3, Ydlidar X4 Lidar sensor, a MDF frame, three 251RPM DFrobot DC motors with 64 counts per revolution encoders, 58mm omni wheels, 3D printed motor mounts, Cytron 10A motor controllers and a 5.5Ah 11.1v LiPo battery from another project.

# Description of the Robot
This robot is based on ROS (Robot Operating System) which is a robotic framework allowing different programming languages (Python, C++, Java, Arduino) to communicate between each other with standardized messages. It includes a lot of usefull packages to visualize and debug software and hardware.

The robot's Raspberry PI create a hotspot on which a computer can log in to send commands to move the robot around, and gather information like odometry and lidar scan data. I used the SLAM (Simultaneous Localization And Mapping) Gmapping package made by Brian Gerkey to create a map of the environment.

![](https://raw.githubusercontent.com/RobinBaruffa/omniROS/master/Documentation%20and%20misc/map_picture.JPG)

Controlling the robot is done via an external computer connected to the ROS network, a homemade joystick controller is used to send velocity commands. The Raspberry then sends the received data to the embedded ESP8266 via Wifi, which performs a basis change operation in order to send a PWM (Pulse Width Modulated) signal to the motor controllers. The ESP8266 perform a closed loop PID control in the meantime. 

The Arduino Nano counts the number of revolution via external interrupts and sends it to the ESP8266 which performs the integration and the backward kinematics, then forwards it to the network.

# Description of the Simulation
The cyberphysical environment has been created using Gazebo, a physics simulator with native ROS support. A simplified model of the real robot is designed using only cylinders (which dramatically improves performance by making collision detection much easier). The simulated robot features an emulated LIDAR scanner with the same properties of the genuine one.

![](https://raw.githubusercontent.com/RobinBaruffa/omniROS/master/Documentation%20and%20misc/gazebo_screenshot.png)

At all steps of the development, I have checked as much as possible that the robot model behavior was as close as possible to the real one : here is an example where I checked that the PID controller worked as intended

![](https://raw.githubusercontent.com/RobinBaruffa/omniROS/master/Documentation%20and%20misc/Screenshot%20from%202019-05-05%2017-35-53.png)

# Description of the reinforcement learning algorithm
Reinforcement Learning (RL) algorithm are at the heart of recent innovation in autonomous vehicle and AI improvement. They are used in a large variety of fields (Google DeepMind, OpenAI, AlphaGO). I use an algorithm called DDPG (Deep Deterministic Policy Gradients) created by Google DeepMind laboratory, and my goal here is to teach the robot how to throw back a ball without giving it any prior knowledge about how it should be done. This algorithm gets as an input a 360 dimensions vector representing the distance between the robot and an object each degree of rotation, and outputs a 3 dimension vector responsible for the two velocity in translation and the velocity in rotation. 
An algorithm manages the whole simulation by spawning a simulated ball at a random distance from the robot, and then strike it roughly in its direction with a random velocity. A fitness function evalutates the RL algorithm performance and is used to find an optimal policy

# Video pitch of a potential consumer product based on the project
While attending an entrepreneurship course at UTBM (Université de Technologie de Belfort-Montbéliard), I edited a video explaining how a commercial product could be derived from this project.

[![omniROS omniwheel robotic development platform for AI](https://raw.githubusercontent.com/RobinBaruffa/omniROS/master/Documentation%20and%20misc/omniROSYTthumbnail.png | width=300)](http://www.youtube.com/watch?v=R22B-GZS-08)
