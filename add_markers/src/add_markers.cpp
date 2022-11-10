#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <std_msgs/Float32MultiArray.h>
//#include <nav_msgs/Odometry.h>
#include <tf/tf.h>
#include <math.h>

float pos_pickup[] = {1.7,6.9,1.0};
float pos_dropoff[] = {-2.0,-3.0,1.0};
float goal_state[] = {0.0,0.0};
bool pickup = false;
bool dropoff = false;

void goalCallback(const std_msgs::Float32MultiArray::ConstPtr& msg)
{
  goal_state=msg->data;
  return;
}


int main( int argc, char** argv )
{
  ros::init(argc, argv, "add_markers");
  ros::NodeHandle n;
  ros::Rate r(1);
  ros::Subscriber goal_sub = n.subscribe("goal_status",1,goalCallback);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  //ros::Subscriber odom_sub=n.subscribe("odom",10,OdomCallback);
  // Set position and variable
  while(ros::ok())
  {
    visualization_msgs::Marker marker;
  
    // Set the frame ID and timestamp.  See the TF tutorials for information on these.
    marker.header.frame_id = "/map";
    marker.header.stamp = ros::Time::now();

    // Set the namespace and id for this marker.  This serves to create a unique ID
    // Any marker sent with the same namespace and id will overwrite the old one
    marker.ns = "add_markers";
    marker.id = 0;

    // Set the color -- be sure to set alpha to something non-zero!
    marker.color.r = 0.0f;
    marker.color.g = 1.0f;
    marker.color.b = 0.0f;
    marker.color.a = 1.0;

    marker.type = visualization_msgs::Marker::SPHERE;
    marker.action = visualization_msgs::Marker::ADD;
    marker.pose.position.x = pos_pickup[0];
    marker.pose.position.y = pos_pickup[1];
    marker.pose.position.z = 0;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = pos_pickup[2];
    marker.pose.orientation.w = 1.0;
    marker.scale.x = 0.3;
    marker.scale.y = 0.3;
    marker.scale.z = 0.3;
    marker.lifetime = ros::Duration();
   
    if (!pickup)
    {
      marker_pub.publish(marker);
      float x =pos_pickup[0] - goal_state[0];
      float y =pos_pickup[1] - goal_state[1];
      float pos = pow((sqrt(x)+sqrt(y)),2);
      ROS_INFO("pos %f ",pos);
      if (pos <0.3)
      {
        marker.action = visualization_msgs::Marker::DELETE;
        marker_pub.publish(marker);
        pickup = true;
      }
    }
   

    ros::spin();
    r.sleep();
  }
  return 0;
}

