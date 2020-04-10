/* Extract all integers from string */
#include <iostream> 
#include <opencv2/opencv.hpp>
#include <sstream> 
#include <vector>

using namespace std; 
using namespace cv;

template<class T>
void printVector(vector<vector<T>> const &mat) {
	for (vector<T> row: mat) {
		for (T val: row) {
			cout << val << " ";
		}
		cout << '\n';
	}
}

void drawBoxes(Mat &img,vector<vector<int>> const &mat,Scalar color) {
	for (vector<int> row: mat) {
  		rectangle( img,
  		       Point( row[0],row[1]),
  		       Point( row[2],row[3]),
  		       color,
  		       LINE_4,
  		       LINE_8 );
	}
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
			cout << found << " "; 
			v.push_back(found);
			j++;
			if (j == 4){
				j=0; i++;
				boxes.push_back(v);
				v.clear();
				cout <<  " | "; 
			}
		}

		/* To save from space at the end of string */
		temp = ""; 
	}
	cout << endl; 
	//printVector(boxes);
} 

// Driver code 
int main() 
{ 
    Mat img;
    img = imread("../girl_small.jpg", CV_LOAD_IMAGE_COLOR);   // Read the file

    if(! img.data )                              // Check for invalid input
    {
        cout << "Couldn't open the image" << std::endl ;
        return -1;
    }
	string faces = "[8, 16, 212, 220]";
	string eyes  = "[39, 60, 90, 111, 118, 64, 174, 120]";
	string str   = "[ 88, 30, 100, 400, 33, 40, 500, 300, 11, 23, 300, 400]"; 

	// Draw example
	//vector<vector<int>> xboxes;
	//int numberOfBoxes = 3;
	//extractBoxes(str,numberOfBoxes,xboxes); 
	//printVector(xboxes);
	//drawBoxes(img,xboxes);
	
	///// Draw faces
	vector<vector<int>> fboxes;
	int numberOfBoxes = 1;
	extractBoxes(faces,numberOfBoxes,fboxes); 
	printVector(fboxes);
	drawBoxes(img,fboxes,Scalar( 255,0,0 ));

	// Draw faces
	vector<vector<int>> eboxes;
	numberOfBoxes = 2;
	extractBoxes(eyes,numberOfBoxes,eboxes); 
	printVector(eboxes);
	drawBoxes(img,eboxes,Scalar( 0,255,0 ));

    imwrite("./output_cplusplus.jpg", img);   // Read the file
	return 0; 
} 

