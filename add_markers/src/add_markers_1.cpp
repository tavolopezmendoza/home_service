#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <nav_msgs/Odometry.h>
#include <tf/tf.h>
#include <math.h>

float pos_pickup[] = {1.7,6.9,1.0};
float pos_dropoff[] = {-2.0,-3.0,1.0};
float odom_pose_x = 0.0;
float odom_pose_y = 0.0;
bool pickup = false;
bool dropoff = false;

void odomCallback(const nav_msgs::Odometry::ConstPtr& msg)
{
  ::odom_pose_x = msg->pose.pose.position.x;
  ::odom_pose_y = msg->pose.pose.position.y;     
}

int main( int argc, char** argv )
{
  ros::init(argc, argv, "add_markers");
  ros::NodeHandle n;
  ros::Rate r(1);
  ros::Subscriber odom_sub = n.subscribe("/odom",1000,odomCallback);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  
  // Set position and variable
  
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
  while(ros::ok()){
    if (!pickup)
    {
      marker_pub.publish(marker);
      float x =pos_pickup[0] - odom_pose_x;
      float y =pos_pickup[1] - odom_pose_y;
      float pos = pow((sqrt(x)+sqrt(y)),2);
      ROS_INFO("pos %f ",pos);
      if (pos <0.3)
      {
        marker.action = visualization_msgs::Marker::DELETE;
        marker_pub.publish(marker);
        pickup = true;
      }
    }
   

    
    //r.sleep();
  }
}
