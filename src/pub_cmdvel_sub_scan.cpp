#include <geometry_msgs/Twist.h>
#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>

geometry_msgs::Twist ling;

void laserscanCallback(const sensor_msgs::LaserScan::ConstPtr &msg) {
  int rangesize = 719;
  int midrange = rangesize / 2;
  if (msg->ranges[midrange] <= 1) {
    ROS_INFO("collision ahead %d/%d:%f", midrange, rangesize,
             msg->ranges[midrange]);
    // ling.angular.z = 3;
    // ling.linear.y += 1;
    ling.linear.x -= 0.001;
    ling.linear.y += 0.01;
    ling.angular.z = 1;
  } else {
    ROS_INFO("%d/%d:%f", midrange, rangesize, msg->ranges[midrange]);
    ling.linear.x += 0.001;
    ling.angular.z = 0;
  }
}

int main(int argc, char **argv) {

  ros::init(argc, argv, "topic_quiz");
  ros::NodeHandle nh;
  ros::Subscriber sub =
      nh.subscribe("/kobuki/laser/scan", 1000, laserscanCallback);
  // ros::spin();

  ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 1000);
  ros::Rate loop_rate(2);

  /*ling.linear.x = 0;
  ling.linear.y = 0;
  ling.linear.z = 0;
  ling.angular.x = 0;
  ling.angular.y = 0;
  ling.angular.z = 0;*/

  while (ros::ok()) {
    pub.publish(ling);
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}