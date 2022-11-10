#!/bin/sh

export TURTLEBOT_GAZEBO_WORLD_FILE=/home/gustavo/workspace/catkin_ws/src/worlds/gustavo.world
export TURTLEBOT_GAZEBO_MAP_FILE=/home/gustavo/workspace/catkin_ws/src/map/map.yaml

xterm -e "roslaunch turtlebot_gazebo turtlebot_world.launch" &
sleep 7

xterm -e "roslaunch turtlebot_gazebo amcl_demo.launch" &
sleep 5

xterm -e "roslaunch turtlebot_rviz_launchers view_navigation.launch" &
sleep 2

xterm -e "rosrun add_markers add_markers" &

sleep 2
xterm -e "rosrun pick_objects pick_objects"
