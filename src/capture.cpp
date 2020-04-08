#include <iostream>
#include <opencv2/opencv.hpp>
#include <cstdio>
#include <hiredis.h>

using namespace std;

int
main(int argc,char*argv[])
{
   //const int width = 640;
   //const int height= 480;
   //cv::Mat img(width,height, CV_8UC3, cvScalar(0,255,255));

   // Redis setup
   redisContext *c;
   redisReply *reply;
   const char *hostname = "localhost";
   int port = 6379;

   struct timeval timeout = { 2, 0 }; // 2 seconds
   c = redisConnectWithTimeout(hostname, port, timeout);
   if (c == NULL || c->err) {
       std::cerr << "Something bad happened" << std::endl;
       exit(1);
   }

   cv::Mat img;
   img = cv::imread("../girl_small.jpg", CV_LOAD_IMAGE_COLOR);   // Read the file

   if(! img.data )                              // Check for invalid input
   {
       cout << "Could not open or find the image" << std::endl ;
       return -1;
   }

   // Store Mat in Redis
   reply = (redisReply*)redisCommand(c,"SET image %b",(char*)img.data,img.size().height*img.size().width*3);
   freeReplyObject(reply);
}
