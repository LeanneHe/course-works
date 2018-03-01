#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;

// Point in the xy-plane
struct PointType{
    double x;
    double y;
    PointType(double a, double b){
        x = a;
        y = b;
    }
};

// Sort criterion: points are sorted with respect to their x-coordinate.
//                 If two points have the same x-coordinate then we compare
//                 their y-coordinates
bool sortPoints(const PointType &lhs, const PointType &rhs) 
{ 
    return (lhs.x < rhs.x) || (lhs.x==rhs.x && lhs.y < rhs.y); 
}


// Check if three points make a right turn using cross product
bool right_turn(const PointType &P1, const PointType &P2, const PointType &P3)
{
    return ((P3.x-P1.x)*(P2.y-P1.y) - (P3.y-P1.y)*(P2.x-P1.x)) > 0;
}


bool Empty(std::ifstream & pFile)
{
    return pFile.peek() == std::ifstream::traits_type::eof();
}

int main () {
      
    ifstream myfile;
    int n_points=7;
	int cnt_line=0;
    PointType *points;
	double a,b;

    vector<PointType> v;
    vector<PointType> lowerCH;
    vector<PointType> upperCH;
	//n_points = system("wc -l < input.txt");
    
    // reading data from input file
    myfile.open("input.txt");
    if(Empty(myfile))
		return 0;
	
    if(myfile.is_open() && !Empty(myfile))
    {
     //myfile >> n_points; //now don't have this line
     //points = new PointType[n_points];
     //call constructor, get x,y
	 
		while(!myfile.eof()){
			myfile >> a >> b;
			PointType *p = new PointType(a,b);
			v.push_back(*p);
			//for(int i=0; i< n_points; i++)
				//myfile >> points[i].x >> points[i].y;
			//cnt_line++;
		}
        myfile.close();
    }
	
        //Sort points
        //sort(points, points + n_points, sortPoints);
		sort(v.begin(), v.end(), sortPoints);
		
		//for(int i=0; i < v.size()-1; i++)
            //cout  << v[i].x << " " << v[i].y << endl;
		
		
        //Computing upper convex hull start at the first 2 pts
        upperCH.push_back(v[0]);
        upperCH.push_back(v[1]);
		
        //cout<<upperCH[0].x<<" "<< upperCH[0].y<<" "<< upperCH[1].x<<upperCH[1].y<<" "<< endl;
        for(int i=2; i< (v.size()-1); i++)
        {
            while(upperCH.size() > 1 and (!right_turn(upperCH[upperCH.size()-2],upperCH[upperCH.size()-1], v[i]))){
				upperCH.pop_back();
			}
            upperCH.push_back(v[i]);
        }
        //cout << "Upper Covex Hull " << upperCH.size() << endl;
        for(int i=0; i < upperCH.size(); i++)
            cout  << upperCH[i].x << " " << upperCH[i].y << endl;
        
        //Computing lower convex hull
        lowerCH.push_back(v[v.size()-2]);
        lowerCH.push_back(v[v.size()-3]);
		
		///////////////////////////////////////////////////////
        for(int i=2; i< (v.size()-1); i++)
        {
            while(lowerCH.size() > 1 and (!right_turn(lowerCH[lowerCH.size()-2],lowerCH[lowerCH.size()-1], v[v.size()-i-2]))){
                lowerCH.pop_back();
			}
            lowerCH.push_back(v[v.size()-i-1-1]);
        }
		
		//get rid of begin and end pts that already in upperHull
		lowerCH.pop_back(); 
		
        //cout << "Lower Covex Hull" << endl;
        for(int i=1; i < lowerCH.size()-1; i++)
            cout << lowerCH[i].x << " " << lowerCH[i].y << endl;
    
    
    return 0;
}
