#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <std_msgs/Float32MultiArray.h>

//Define a client for to send goal requets to the move_base server through a SimpleActionClient
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char** argv){
  //Initialize the pick_objects node
  ros::init(argc, argv, "pick_objects");

  ros::NodeHandle n;
  ros::Publisher goal_pub=n.advertise<std_msgs::Float32MultiArray>("goal_status",1);
  ros::Rate loop_rate(5.0);
  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  float pos_pickup[]={1.7,6.9,1.0};
  float pos_dropoff[]={-2.0,-3.0,1.0};
  //Wait 5 sec for move_base action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  move_base_msgs::MoveBaseGoal goal;
  std_msgs::Float32MultiArray status;
  //set up the frame parameters
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();

  //Define a position and orientation for the robot to reach
  goal.target_pose.pose.position.x = pos_pickup[0];
  goal.target_pose.pose.position.y = pos_pickup[1];  
  goal.target_pose.pose.orientation.w = pos_pickup[2];;

  //Send the goal position and orientation for the robot to reach	
  ROS_INFO("Sending pickup position");
  ac.sendGoal(goal);

  //Wait an infinite time for the results	
  ac.waitForResult();

  //Check if the robot reached its goal	
  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
    ROS_INFO("Hooray, the robot reached the pickup zone");
    status.data={pos_pickup[0],pos_pickup[1]};
    goal_pub.publish(status);
    ros::Duration(5.0).sleep();
  }
  else {
    ROS_INFO("The robot failed to reach the pickup zone for some reason");
    ros::Duration(5.0).sleep();
  }
  //Wait 5 sec for move_base action server to come up
  //while(!ac.waitForServer(ros::Duration(5.0))){
    //ROS_INFO("Waiting 5 seconds for the robot to pick up the objects.");
  //}

  //Define a position and orientation for the robot to reach
  goal.target_pose.pose.position.x = pos_dropoff[0];
  goal.target_pose.pose.position.y = pos_dropoff[1];  
  goal.target_pose.pose.orientation.w = pos_dropoff[2];

  //Send the goal position and orientation for the robot to reach	
  ROS_INFO("Sending drop off position");
  ac.sendGoal(goal);

  //Wait an infinite time for the results	
  ac.waitForResult();

  //Check if the robot reached its goal	
  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
    ROS_INFO("Hooray, the robot reached the dropoff zone");
    status.data={pos_dropoff[0],pos_dropoff[1]};
    goal_pub.publish(status);
  } 
  else
    ROS_INFO("The robot failed to reach the dropoff zone for some reason");
  
  	
  return 0;
}
