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
       std::cerr << "Couldn't connect with the Redis server" << std::endl;
       exit(1);
   }

   cv::Mat img;
   img = cv::imread("../girl_small.jpg", CV_LOAD_IMAGE_COLOR);   // Read the file

   if(! img.data )                              // Check for invalid input
   {
       cout << "Couldn't open the image" << std::endl ;
       return -1;
   }

   vector<uchar> imgjpg;
   cv::imencode(".jpg",img,imgjpg);

   // Store jpg in Redis
   reply = (redisReply*) redisCommand(c,
           "XADD camera:0 MAXLEN ~ 10000 * image %b", 
           reinterpret_cast<char*>(imgjpg.data()),
           imgjpg.size()
           //img.size().height*img.size().width*3
           );
   freeReplyObject(reply);

   // Wait for the boxes from the facedetection algorithm
   reply = (redisReply*) redisCommand(c,
           "XREAD COUNT 1 BLOCK 0 STREAMS camera:0:facedect $");
   if (reply->type == REDIS_REPLY_ARRAY) {
        printf("%s\n", reply->element[0]->element[0]->str);
        printf("%s\n", reply->element[0]->element[1]->element[0]->element[0]->str);
        int elements =  reply->element[0]->element[1]->element[0]->element[1]->elements;
        for (int j = 0; j < elements; j++) {
            printf("%u) %s\n", j, reply->element[0]->element[1]->element[0]->element[1]->element[j]->str);
            //printf("%u) %s\n", j, reply->element[j]->str);
        }

        ////Facesboxes
        //string facesboxes(reply->element[0]->element[1]->element[0]->element[1]->element[1]->str);
        ////Facesboxes counter
        //reply->element[0]->element[1]->element[0]->element[1]->element[3]->str
        ////eyesboxes
        //reply->element[0]->element[1]->element[0]->element[1]->element[5]->str
        ////Facesboxes counter
        //reply->element[0]->element[1]->element[0]->element[1]->element[7]->str
        ////ref_id
        //reply->element[0]->element[1]->element[0]->element[1]->element[9]->str
    }
   freeReplyObject(reply);
   
}
