/* Extract all integers from string */
#include <iostream> 
#include <sstream> 
#include <vector>

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

void extractBoxes(string str, int num_boxes, vector<vector<int>> &boxes) 
{ 
	stringstream ss;	 

	/* Storing the whole string into string stream */
	ss << str; 
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
	int numberOfBoxes = 3;
	string str = "[ 88, 30, 100, 400, 33, 40, 8, 34, 11, 23, 30, 40 ]"; 
	vector<vector<int>> fboxes;
	extractBoxes(str,numberOfBoxes,fboxes); 
	printVector(fboxes);
	return 0; 
} 

