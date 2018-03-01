#include<iostream>
#include<stack>
#include<fstream>
#include<vector>
#include<string>
#include<sstream>
#include<algorithm>
#include<list>
#include<map>
#include<set>
using namespace std;
#define LOWER 0
#define UPPER 1
#define INTE 2

struct segments{
	float xs,ys,xe,ye;
	int ind;
	segments(float x1, float y1, float x0, float y0, int i){
		xs = x1;
		ys = y1;
		xe = x0; 
		ye = y0;
		ind = i;
	}
	
	//calculate the x-coor of the intersection between sweep line and segment
	float xcoor(float ycoor){
		
			float a = (ys - ye)/(xs - xe);
		    float b = ys - a*xs;
		    return (ycoor-b)/a;
		
	}
	 	//For Erase 
    bool operator==(const segments &other) const{  
       	 return ((ind == other.ind) && (ind == other.ind));  
   	}
   	
};
bool comp(const segments &a, const segments &b){
		if(a.ind < b.ind){
			return true;
		}
		else
			return false;
}
 
struct points{
	float x,y;
	vector<segments> seg;
	int type;
	points(float a, float b, segments i, int t){
		x = a;
		y = b;
		seg.push_back(i);
		type = t;
	}

    
    bool operator==(const points &other) const{  
       	 return ((x == other.x) && (y == other.y));  
   	}  

};
//save intersections
vector<points> intePt;

struct symbol{
	float x, y;
	bool operator () (const points &a, const points &b){
		if(a.x < b.x){
			return true;
		}
		else
			return false;
}

   	 bool operator==(const points &other) const{  
       	 return ((x == other.x) && (y == other.y));  
   	}   
};


 //For Sort
bool compare(const points &a, const points &b){
		
		if(a.y > b.y){
			return true;
		}
		else if(a.y == b.y){
			if(a.x < b.x)
				return true;
			else if(a.x == b.x){
				if(a.type < b.type)
					return true;
			
				else
					return false;
			}
			else
				return false;
		}
		else
			return false;
	}


//check if it is empty, if not read the data into vector, else stop running
bool input_isEmpty(vector<points> &vec,string filename){
	ifstream in(filename.c_str());
	string ps;
	int ind = 0;
	if(in.is_open()){
		cout<<"File exits"<<endl;
	}
	if(in.peek()==EOF)
	{
		cout<<"File is Empty"<<endl;
		return true;
	}
		
	while(!in.eof()){
		ind++;
		getline(in, ps);	
		float x;
		vector<float> coor;
		stringstream str(ps);
		
		while(str>>x){
			coor.push_back(x);
		}
		segments *seg = new segments(coor[2],coor[3],coor[0],coor[1],ind);
		points *e = new points(coor[0],coor[1],*seg,LOWER);
		points *s = new points(coor[2],coor[3], *seg,UPPER);
		//s

		vec.push_back(*e);
		vec.push_back(*s);
	      //without this condition, the lines got will be one more than actual
		if(in.peek() == '\n')  
			break;	
	}
		in.close();
	return false;
}


void sort_seg(vector<points> &vec) {
	sort(vec.begin(), vec.end(),compare);
	
}
void findIntersection(segments la, segments lb, vector<points> &event, float y_status, float curr_x){
	float xa1 = la.xs;
	float ya1 = la.ys;
	float xa0 = la.xe;
	float ya0 = la.ye;
	float xb1 = lb.xs;
	float yb1 = lb.ys;
	float xb0 = lb.xe;
	float yb0 = lb.ye;
/*	cout<<"xa1: "<<xa1<<" "<<"ya1: "<<ya1<<endl;
	cout<<"xa0: "<<xa0<<" "<<"ya0: "<<ya0<<endl;
	cout<<"xb1: "<<xb1<<" "<<"yb1: "<<yb1<<endl;
	cout<<"xb0: "<<xb0<<" "<<"yb0: "<<yb0<<endl;*/
	float s1,s2,s3,s4;
	float a,b;
	float inte_x, inte_y;
	//See if points a1 and a0 are at two sides of line [b1,b0].
	s1=(xb0-xa0)*(yb1-ya0)-(xb1-xa0)*(yb0-ya0);
	s2=(xb0-xa1)*(yb1-ya1)-(xb1-xa1)*(yb0-ya1);
	if(s1*s2<=0)
	{
		//One of a1 and a0 on line [b1,b0] is acceptable.
		//See if points b1 and b0 are at two sides of line [a1,a0].
		s3=(xa1-xb0)*(ya0-yb0)-(xa0-xb0)*(ya1-yb0);
		s4=s2-s1+s3;
		if(s3*s4<=0)
		{
			//One of b1 and b2 on line [a1,a0] is acceptable.
			//Calculate the coordinate of the intersection point.

			if(xa0==xa1 || xb0==xb1)
			{
				if(xa0==xa1)
				{
					b=(yb0-yb1)/(xb0-xb1);
					inte_x = xa0;
					inte_y = b*(inte_x-xb1)+yb1;
				}else{
					a=(ya0-ya1)/(xa0-xa1);
					inte_x=xb0;
					inte_y=a*(inte_x-xa1)+ya1;
				}
			}else{
				a=(ya0-ya1)/(xa0-xa1);
				b=(yb0-yb1)/(xb0-xb1);
				inte_x=(a*xa1-b*xb1-ya1+yb1)/(a-b);
				inte_y=a*(inte_x-xa1)+ya1;
					
				}
				//cout<<"int_pt"<< inte_x  <<" "<< inte_y <<endl;
				
			if(inte_y < y_status ||(inte_y == y_status && inte_x >= curr_x)){
				points *inte  = new points(inte_x, inte_y,la,INTE);
				inte->seg.push_back(lb);
				vector<points>::iterator duplicate = find(intePt.begin(), intePt.end(), *inte);
				//check if duplicate(not same type), if no, add into intePt
				if(inte_x != curr_x){
					vector<points>::iterator duplicate_event = find(event.begin(), event.end(), *inte);
					if(duplicate_event!=event.end() && duplicate_event->type == inte->type){
						duplicate_event->seg.push_back(la);
						duplicate_event->seg.push_back(lb);
					}else
					event.push_back(*inte);
				
				}
				
				if(duplicate!=intePt.end()){
					 duplicate->seg.push_back(la);
					 duplicate->seg.push_back(lb);
				}else
					intePt.push_back(*inte);
			
			}
			
		}
	}
	
}



void Tree(set<points, symbol> &s, vector<points> &event){
	 while (!event.empty()) {
		 sort_seg(event);
		 points top = event[0];
		  //no duplicate points
		  //cout<<"top:"<<top.x<<" "<< top.y<<endl;
		  event.erase(event.begin());
		  vector<points>::iterator duplicate = find(event.begin(), event.end(), top);
		 // cout<<"hi2"<<endl;
		  if(duplicate!=event.end()){
			  //cout<<"duplication"<<endl;
			  points *p_inte = new points(duplicate->x, duplicate->y, top.seg[0], INTE);
			  //cout<<"hi3"<<endl;
			  while(duplicate!=event.end() && duplicate->type == top.type){
				
			   
				  p_inte->seg.push_back(duplicate->seg[0]);
				  top.seg.push_back(duplicate->seg[0]);
				  event.erase(duplicate);
				  duplicate = find(event.begin(), event.end(), top);
			    
		      }
		      intePt.push_back(*p_inte);
		  }
		
		  float y_status = top.y;
		  float x_mid = top.x;
		  vector<points> tmp_vec;
	
		  //find intersection
		  if(!s.empty()){
			  //SL goes down, renew status
			   while(!s.empty()){
				  set<points, symbol>::iterator tmp = s.begin();
				 
				  if(top.x != tmp->x || top.y != tmp->y){
					  tmp_vec.push_back(*tmp);
				  }
				  s.erase(tmp);
		       }
		 
			  //get x-coor of the new intersection between SL and segment
			  for(vector<points>::iterator tmp_p= tmp_vec.begin(); tmp_p != tmp_vec.end(); tmp_p++){
				  vector<segments> tmp_seg = tmp_p->seg;
				  //get x-coor of the new intersection between SL and segment
				  for(vector<segments>::iterator i= tmp_seg.begin(); i != tmp_seg.end(); i++){
					  float x_tmp;
					  
					  if(i->xs == i->xe )   //vertical,
						 x_tmp = i->xs;
					  else
					     x_tmp = i->xcoor(y_status);
				
					  points *tmp_p = new points(x_tmp, y_status, *i, UPPER);
					  if(*tmp_p == top && top.type==LOWER)
					    continue;
					  s.insert(*tmp_p);
				  }
				  
			   }
	
			  int sz = top.seg.size();
			  set<points, symbol>::iterator r = s.lower_bound(top);
			 
			    if(r!=s.end()){
					if(r == s.begin()){
						 // cout<<"right"<<endl;
						  if(top.type==INTE){
							findIntersection(top.seg[0], r->seg[0], event, y_status, x_mid);
						  }
						  else if(top.type==UPPER){
							  findIntersection(top.seg[0],r->seg[0], event, y_status, x_mid);
						  }
						  else{
							
							continue;
						}
					}
					else{
						set<points, symbol>::iterator l = r--;
						//cout<<"right, left"<<endl;
					   if(top.type==LOWER){
					     findIntersection(r->seg[0], l->seg[0], event,y_status, x_mid);
					     continue;				
					  }
					  //inte
					  else if(top.type==INTE){
						  findIntersection(l->seg[0], top.seg[sz-1], event,y_status, x_mid);
						  findIntersection(top.seg[0], r->seg[0], event,y_status, x_mid);
						  
					  }
					  else{
						  findIntersection(l->seg[0], top.seg[0], event, y_status, x_mid); 
						  findIntersection(top.seg[0],r->seg[0], event, y_status, x_mid);
						 
					  }
					}
				  
				}
			  else{
				   set<points, symbol>::iterator l;
				  for(set<points, symbol>::iterator it = s.begin(); it != s.end(); it++){
						l = it;
					}
				  if(l!=s.end()){
				//	  cout<<"left"<<endl;
					  if(top.type==INTE){
						findIntersection(l->seg[0], top.seg[sz-1], event, y_status, x_mid);
			          }
			          else if(top.type==UPPER){
					    findIntersection(l->seg[0], top.seg[0], event, y_status, x_mid); 
				      } 
				      else
				       continue;
				  }
			  }
			}
		
	
		 set<points, symbol>::iterator dup = s.find(top);
		 //check if the point already exits in status
		
		 if(dup!=s.end()){
			 points *comb = new points(dup->x, dup->y, dup->seg[0],dup->type);
			
			 for(vector<segments>::iterator it = top.seg.begin(); it != top.seg.end(); it++){
				 comb->seg.push_back(*it);
			 }
			 s.erase(*dup);
			 s.insert(*comb);
			 
		 }else{
				 s.insert(top); 
		 }
	    	 
	 }
}

void out_file(){
	string name = "output.txt";
	ofstream out;
	out.open(name.c_str());
	if(out.is_open()){
		for(vector<points>::iterator it = intePt.begin(); it != intePt.end(); it++){
		out<<it->x<<" "<<it->y<<" ";
		for(vector<segments>::iterator i= it->seg.begin(); i != it->seg.end(); i++)
			out<<i->ind<<" ";
			out<<"\n";
	    }
	}
	
	out.close();
}

void combine(){
	for(vector<points>::iterator it = intePt.begin(); it != intePt.end(); it++){
		//intePt.erase(intePt.begin());
		vector<points>::iterator duplicate = find(it+1, intePt.end(), *it);
		if(duplicate==intePt.end())
			continue;
			
		while(duplicate!=intePt.end()){
			for(vector<segments>::iterator i= duplicate->seg.begin(); i != duplicate->seg.end(); i++)
				it->seg.push_back(*i);
	
			intePt.erase(duplicate);
			duplicate = find(it+1, intePt.end(), *it);
		 }
		//intePt.insert(it, *p_inte);
		sort(it->seg.begin(), it->seg.end(), comp);
	    vector<segments>::iterator segm = unique(it->seg.begin(), it->seg.end()); 
    	it->seg.erase(segm, it->seg.end()); 
	}			
}

int main(){	
	vector<points> pvec;
	set<points, symbol> sl;
	if(input_isEmpty(pvec,"input.txt"))	
		return 0;
	Tree(sl, pvec);
	combine();
	out_file();
	for(vector<points>::iterator it = intePt.begin(); it != intePt.end(); it++){
		//cout<<it->idx<<" "<<it->y<<endl;
		cout<<it->x<<" "<<it->y<<" ";
		for(vector<segments>::iterator i= it->seg.begin(); i != it->seg.end(); i++)
			cout<<i->ind<<" ";
			cout<<endl;
	}
	return 0;
}
