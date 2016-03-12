#include <ros/ros.h>
#include <geometry_msgs/PointStamped.h>
#include <tf/transform_listener.h>
#include <boost/thread.hpp>
#include <geometry_msgs/Point32.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <math.h>
#include <termios.h>
#define BOUND(x, a, b) (((x)<(a))?(a):((x)>(b))?(b):(x))
#define PI 3.1415926

#define SCREEN_V_RES 1080
#define SCREEN_H_RES 1920

char *JSON_heart_beat="{\"category\":\"heartbeat\"}";

char *JSON_set_push="{\"category\":\"tracker\",\"request\":\"set\",\"values\":{\"push\":true,\"version\":1}}";
// Scanf template for the frame (containing "fix:false")

char *JSON_frame_false="{\"category\":\"tracker\",\"request\":\"get\",\"statuscode\":200,\"values\":{\"frame\":{"\
             "\"avg\":{\"x\":%f,\"y\":%f},\"fix\":false,\"lefteye\":{\"avg\":{\"x\":%f,\"y\""\
             ":%f},\"pcenter\":{\"x\":%f,\"y\":%f},\"psize\":%f,\"raw\":{\"x\":%f"\
             ",\"y\":%f}},\"raw\":{\"x\":%f,\"y\":%f},\"righteye\":{\"avg\":{\"x\":%f"\
             ",\"y\":%f},\"pcenter\":{\"x\":%f,\"y\":%f},\"psize\":%f,\"raw\":{\"x\":%f"\
             ",\"y\":%f}},\"state\":%d,\"time\":%d}}}";
// Scanf template for the frame (containing "fix:true")

char *JSON_frame_true="{\"category\":\"tracker\",\"request\":\"get\",\"statuscode\":200,\"values\":{\"frame\":{"\
             "\"avg\":{\"x\":%f,\"y\":%f},\"fix\":true,\"lefteye\":{\"avg\":{\"x\":%f,\"y\""\
             ":%f},\"pcenter\":{\"x\":%f,\"y\":%f},\"psize\":%f,\"raw\":{\"x\":%f"\
             ",\"y\":%f}},\"raw\":{\"x\":%f,\"y\":%f},\"righteye\":{\"avg\":{\"x\":%f"\
             ",\"y\":%f},\"pcenter\":{\"x\":%f,\"y\":%f},\"psize\":%f,\"raw\":{\"x\":%f"\
             ",\"y\":%f}},\"state\":%d,\"time\":%d}}}";
/***************************************************************************************/
void error(const char *msg)
{
    perror(msg);
    exit(0);
}
/***************************************************************************************/
 geometry_msgs::Point sholder_data;
 ros::Publisher pubFoot;
 ros::Publisher pubHip;
 ros::Publisher pubKnee;
 ros::Publisher pubSholder;
 int sockfd, portno, n;
 struct sockaddr_in serv_addr;
 struct hostent *server;
 int bytesReceived;
 int num_scanned=0;
 char str[256];
 float x_avg, y_avg, left_x_avg, left_y_avg, left_pcenter_x, left_pcenter_y, left_psize, left_x_raw, left_y_raw,x_avg_old,y_avg_old,
    x_raw, y_raw, right_x_avg, right_y_avg, right_pcenter_x, right_pcenter_y, right_psize, right_x_raw, right_y_raw;
 int state, tet_time, fix;
 bool start_program=true;
#define alpha 0.1
 /***************************************************************************************/
 void timerCallback(const ros::TimerEvent&)
 { // Heartbeat
	if(write(sockfd,JSON_heart_beat,strlen(JSON_heart_beat))< 0) error("ERROR writing to socket"); // Heart beating every 1 second
 }
 /***************************************************************************************/

int main(int argc, char** argv)
{
 ros::init(argc, argv, "gaze_client_node");
    ros::NodeHandle nh;
  char buffer[4096];

    portno = 6555;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname("192.168.152.128");
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    bzero(buffer,4096); // clear buffer
       n = write(sockfd,JSON_set_push,strlen(JSON_set_push));
    if (n < 0) error("ERROR writing to socket");
    bzero(buffer,4096);n = read(sockfd,buffer,4095);if (n < 0) error("ERROR reading from socket"); // Ignore this message
    ros::Timer timer = nh.createTimer(ros::Duration(1), timerCallback);
    ros::Publisher eye_gaze = nh.advertise<geometry_msgs::Point>("/eye_gaze/avg", 1);
    geometry_msgs::Point eye_avg;
    while(ros::ok())
    {
    	  bzero(buffer,4096);n = read(sockfd,buffer,4095);if (n < 0) error("ERROR reading from socket"); // Ignore this message
    	  buffer[n]=0; // Make a string zero-terminated
    	  // First try. Template string contains "fix:false"
    	        fix=0;
    	        num_scanned=sscanf(buffer,JSON_frame_false,
    	           &x_avg,&y_avg,
    	           &left_x_avg, &left_y_avg, &left_pcenter_x, &left_pcenter_y, &left_psize, &left_x_raw, &left_y_raw,
    	           &x_raw, &y_raw,
    	           &right_x_avg, &right_y_avg, &right_pcenter_x, &right_pcenter_y, &right_psize, &right_x_raw, &right_y_raw,
    	           &state, &tet_time
    	           );
    	        // Second try. Template string contains "fix:true"
    	        if(20!=num_scanned)
    	        {
    	           fix=1;
    	           num_scanned=sscanf(buffer,JSON_frame_true,
    	           &x_avg,&y_avg,
    	           &left_x_avg, &left_y_avg, &left_pcenter_x, &left_pcenter_y, &left_psize, &left_x_raw, &left_y_raw,
    	           &x_raw, &y_raw,
    	           &right_x_avg, &right_y_avg, &right_pcenter_x, &right_pcenter_y, &right_psize, &right_x_raw, &right_y_raw,
    	           &state, &tet_time
    	           );
    	        }

    	        if(20==num_scanned)
    	        {
    	           // Do something real here instead of just printing
    	   if(7==state){
    	      //     printf("X:%f Y:%f\n",x_avg,y_avg);

    		   if(start_program==true){x_avg_old=x_avg;y_avg_old=y_avg;start_program=false;}
    		   x_avg=(1-alpha)*x_avg_old+alpha*x_avg;
    		   y_avg=(1-alpha)*y_avg_old+alpha*y_avg;
    		  eye_avg.x=x_avg;eye_avg.y=y_avg;eye_avg.z=0;
    	    //    eye_gaze.publish(eye_avg);
    	          sprintf(str, "xdotool mousemove %.0f %.0f", BOUND(x_avg,0,1920), BOUND(y_avg,0,1080));system(str);bzero(str,256);
    	          x_avg_old=x_avg;y_avg_old=y_avg;
    	   }
    	        }
    	   ros::spinOnce();
    }

  return 0;
}

