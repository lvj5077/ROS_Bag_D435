#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <typeinfo>


#include <rosbag/bag.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>
#include <sensor_msgs/Imu.h>


#include <chrono>
#include <algorithm>
#include <sys/stat.h>
#include <ros/ros.h>

#include <sensor_msgs/image_encodings.h>  

#include <opencv2/objdetect/objdetect.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/core/core.hpp>

#include <std_msgs/String.h>  

#include <cv_bridge/cv_bridge.h>

#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>
using namespace sensor_msgs;
using namespace message_filters;



using namespace std;
using namespace cv ; 

rosbag::Bag bag;
void ImageGrabber(const sensor_msgs::ImageConstPtr& msgRGB,const sensor_msgs::ImageConstPtr& msgD)
{

    // Copy the ros image message to cv::Mat.
    cv_bridge::CvImageConstPtr cv_ptrRGB;
    cv_bridge::CvImageConstPtr cv_ptrAD;
    try
    {
        cv_ptrRGB = cv_bridge::toCvShare(msgRGB, sensor_msgs::image_encodings::BGR8);
        cv_ptrAD = cv_bridge::toCvShare(msgD, sensor_msgs::image_encodings::TYPE_16UC1);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }

    imshow("Live Feed RGB: ", cv_ptrRGB->image);  
	waitKey(3);
	// cv::Mat colorImage = cv_ptrRGB->image;
	// bag.write("color", ros::Time::now()msgRGB.header.stamp, colorImage);
	bag.write("/camera/rgb/image_raw", msgRGB->header.stamp, msgRGB);
    bag.write("/camera/depth_registered/image_raw", msgRGB->header.stamp, cv_ptrAD);
}

void IMUGrabber(const sensor_msgs::Imu::ConstPtr& msgIMU)
{
	// bag.write("IMU", ros::Time::now(), msgIMU->data.c_str());
	bag.write("/imu0", msgIMU->header.stamp, msgIMU);
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "RGB_IMU_bag");
    ros::start();

    mkdir("/home/jin/Data_Capture/myRosBag", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
 	
 	bag.open("/home/jin/Data_Capture/myRosBag/myImage.bag", rosbag::bagmode::Write);

    ros::NodeHandle nh;

    message_filters::Subscriber<sensor_msgs::Image> rgb_sub(nh, "/camera/color/image_raw", 1);
    message_filters::Subscriber<sensor_msgs::Image> depth_sub(nh, "/camera/aligned_depth_to_color/image_raw", 1);
    
    // typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::Image, sensor_msgs::Image> sync_pol;
    // message_filters::Synchronizer<sync_pol> sync(sync_pol(10), rgb_sub,depth_sub);
    
    // TimeSynchronizer<sensor_msgs::Image, sensor_msgs::Image> sync(rgb_sub, depth_sub, 10);
    
    // sync.registerCallback(boost::bind(&ImageGrabber,_1,_2));


    typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::Image, sensor_msgs::Image> sync_pol;
    message_filters::Synchronizer<sync_pol> sync(sync_pol(10), rgb_sub,depth_sub);
    
    //TimeSynchronizer<sensor_msgs::Image, sensor_msgs::Image> sync(rgb_sub, depth_sub, 10);
    
    ros::Subscriber imu_sub = nh.subscribe<sensor_msgs::Imu>("/imu_msg", 10, IMUGrabber);

    ros::spin();

    ros::shutdown();

    bag.close();
    return 0;
}







/*
ros::Publisher imu_pub = n.advertise<std_msgs::String>("IMU_pub", 1000);
ros::Rate loop_rate(10);

while (ros::ok())
{
	std_msgs::String msg;
	msg = ???
	imu_pub.publish(msg);

	ros::spinOnce();
	loop_rate.sleep();
}

*/