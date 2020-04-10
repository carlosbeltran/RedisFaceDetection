#include <iostream>
#include <opencv2/opencv.hpp>
#include <cstdio>
#include <hiredis.h>

using namespace std;

template<class T>
void printVector(vector<vector<T>> const &mat) {
	for (vector<T> row: mat) {
		for (T val: row) {
			cout << val << " ";
		}
		cout << '\n';
	}
}

void drawBoxes(cv::Mat &img,vector<vector<int>> const &mat,cv::Scalar color) {
	for (vector<int> row: mat) {
		cv::rectangle( img,
  		       cv::Point( row[0],row[1]),
  		       cv::Point( row[2],row[3]),
  		       color,
  		       cv::LINE_4,
  		       cv::LINE_8 );
	}
}

int extractInt(string str)
{
	stringstream ss;	 
	ss << str;
	int i;
	ss >> i;
	return i;
}
	
void extractBoxes(string str, int num_boxes, vector<vector<int>> &boxes) 
{ 
	stringstream ss;	 

	/* Storing the whole string into string stream */
	ss << str.substr(1); 
	//vector<vector<int>> fboxes;
	int nboxes = num_boxes;

	/* Running loop till the end of the stream */
	string temp; 
	int found; 
	int j = 0;
	vector<int> v;

	for (int i=0; i<nboxes;){
		/* extracting word by word from stream */

		ss >> temp; 

		/* Checking the given word is integer or not */
		if (stringstream(temp) >> found){ 
			//cout << found << " "; 
			v.push_back(found);
			j++;
			if (j == 4){
				j=0; i++;
				boxes.push_back(v);
				v.clear();
				//cout <<  " | "; 
			}
		}

		/* To save from space at the end of string */
		temp = ""; 
	}
	cout << endl; 
	//printVector(boxes);
} 
int
main(int argc,char*argv[])
{
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
        string faces(reply->element[0]->element[1]->element[0]->element[1]->element[1]->str);
        int numberOfBoxes = extractInt(reply->element[0]->element[1]->element[0]->element[1]->element[3]->str);
        cout<< " numberofboxes: " << numberOfBoxes;
		vector<vector<int>> fboxes;
		extractBoxes(faces,numberOfBoxes,fboxes); 
		printVector(fboxes);
		drawBoxes(img,fboxes,cv::Scalar( 255,0,0 ));

        ////Facesboxes
        string eyes(reply->element[0]->element[1]->element[0]->element[1]->element[5]->str);
        numberOfBoxes = extractInt(reply->element[0]->element[1]->element[0]->element[1]->element[7]->str);
        cout<< " numberofboxes: " << numberOfBoxes;
		vector<vector<int>> eboxes;
		extractBoxes(eyes,numberOfBoxes,eboxes); 
		printVector(eboxes);
		drawBoxes(img,eboxes,cv::Scalar( 0,255,0 ));
    }
   freeReplyObject(reply);
   imwrite("./output_cplusplus.jpg", img);   // Read the file
   
}
