#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <math.h>

using namespace std;

#define TEST_INPUT 0
#define TEST_OUTPUT 1

struct Point {
	double x;
	double y;
	int index;
	Point(double _x, double _y, int _index):x(_x), y(_y), index(_index){} ;
	Point & operator = (Point const & b) {
		x = b.x;
		y = b.y;
		index = b.index;
		return *this;
	}
	double compare_angle(Point const & b) {
		return atan2(b.y-y, b.x-x);
	}

};


void minkowskiSum(vector<Point> &polyP, vector<Point> &polyR, vector<Point> &polyM) {
	int i = 0;
	int j = 0;
	int index_M = 0;
	
	do{
		polyM.push_back(Point( polyP[i].x + polyR[j].x, polyP[i].y + polyR[j].y ,index_M));
		index_M++;

		if ( polyP[i].compare_angle(polyP[i+1]) < polyR[j].compare_angle(polyR[j+1])) {
			i++;
		}
		else if ( polyP[i].compare_angle(polyP[i+1]) > polyR[j].compare_angle(polyR[j+1])) {
			j++;
		}
		else {
			i++;
			j++;
		}
	}while((i<polyP.size()) && (j <polyR.size()));
	
};


int main() {
	//Read from input.txt
	vector<Point> polyP;
	vector<Point> polyR;
	vector<Point> polyM;
	
	int index_P = 0;
	int index_R = 0;
	double px,py;

	freopen("input.txt","r", stdin);
	string line;
	getline(cin, line);
	istringstream iss_A(line);
	while(iss_A >> px >> py){
			polyP.push_back(Point(px, py, index_P));
			index_P++;
	}
	//add Point v_(n+1)
	polyP.push_back(Point(polyP[0].x, polyP[0].y, index_P));


	getline(cin, line);
	std::istringstream iss_B(line);
	while(iss_B >> px >> py){
			polyR.push_back(Point(px, py,index_R));
			index_R++;
	}
	polyR.push_back(Point(polyR[0].x, polyR[0].y,index_R));
	fclose(stdin);

	if(TEST_INPUT) {
		cout<<"Convex Polygon P:"<<endl;
		for (vector<Point>::iterator itp = polyP.begin(); itp != polyP.end(); ++itp) {
			cout << "P["<<itp->index<<"]:"<<"("<<itp->x << "," << itp->y << ")"<<endl;
		}

		cout<<endl<<endl<<"Convex Polygon R:"<<endl;
		for (vector<Point>::iterator itp = polyR.begin(); itp != polyR.end(); ++itp) {
			cout << "R["<<itp->index<<"]:"<<"("<<itp->x << "," << itp->y << ")"<<endl;
		}

	}

	minkowskiSum(polyP, polyR, polyM);
	if (TEST_OUTPUT) {
		cout<<endl<<endl<<"Minkovski Sums result:"<<endl;
		for (vector<Point>::iterator itp = polyM.begin(); itp != polyM.end(); ++itp) {
			cout << "M["<<itp->index<<"]:"<<"("<<itp->x << "," << itp->y << ")"<<endl;
		}
	}
		
	ofstream myfile;
	myfile.open ("output.txt");
	for (vector<Point>::iterator itp = polyM.begin(); itp != polyM.end(); ++itp) {
			myfile <<itp->x << "," << itp->y <<endl;
	}
	myfile.close();
	return 0;
}


