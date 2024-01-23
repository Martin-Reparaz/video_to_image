#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/highgui/highgui.hpp>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "video_publisher");
    ros::NodeHandle nh;

    ros::Publisher pub = nh.advertise<sensor_msgs::Image>("/video", 10);
    ros::Rate rate(30);  // Frecuencia de publicación en Hz

    cv::VideoCapture cap("/home/usuario/catkin_ws/src/video_to_image/videos/city.mp4");  // Reemplaza con la ruta de tu archivo de video
    if (!cap.isOpened())
    {
        ROS_ERROR("Error al abrir el archivo de video.");
        return -1;
    }

    // Establecer la resolución deseada
    int target_width = 480;  // Cambia según tus necesidades
    int target_height = 320; // Cambia según tus necesidades

    cv_bridge::CvImage cv_image;
    cv_image.encoding = "bgr8";

    while (ros::ok() && cap.isOpened())
    {
        cv::Mat frame;
        cap >> frame;
        if (frame.empty())
        {
            break;
        }

        // Redimensionar la imagen a la resolución deseada
        cv::resize(frame, frame, cv::Size(target_width, target_height));

        cv_image.image = frame;

        // Convierte la imagen a mensaje de imagen y publica
        sensor_msgs::ImagePtr img_msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();
        pub.publish(img_msg);

        ros::spinOnce();
        rate.sleep();
    }

    cap.release();

    return 0;
}