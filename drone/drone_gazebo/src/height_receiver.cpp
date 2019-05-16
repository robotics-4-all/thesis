#include <drone_gazebo/height_receiver.h>

namespace height_receiver
{
Estimator::Estimator()
{
  height_.data = 0.0;

  pub_ = nh_.advertise<drone_gazebo::Float64Stamped>("/height", 1);

  sub_ = nh_.subscribe("/lidar/height/scan", 1, &Estimator::laserCallback, this);
}

Estimator::~Estimator()
{
  ROS_INFO("[Estimator] has been destroyed");
}

void Estimator::laserCallback(const sensor_msgs::LaserScan::ConstPtr& msg)
{
  float height_avg = 0.0;
  int number_of_ranges = msg->ranges.size();
  float sum = 0.0;  // temp variable
  for (int i = 0; i < number_of_ranges; i++)
    sum += msg->ranges[i];

  height_avg = sum / number_of_ranges;

  height_.data = height_avg;
  height_.header.stamp = msg->header.stamp;
  height_.header.frame_id = "height_sensor_frame";
  pub_.publish(height_);  // Publish the height in the /height topic

  ROS_INFO("Current height: [%f]", height_avg);
}
}