#include <iostream>
#include <set>
using namespace std;
struct Data{
	int value1;
	int value2;
};

struct DataCompare{
	//equivalence of lhs, rhs is !DataCompare(lhs,rhs) && !DataCompare(rhs,lhs)
	bool operator () (const Data& lhs, const Data& rhs)const{
		if ( lhs.value1 == rhs.value1 )
			return false;
		return true;
	}
};

void print_set(set<Data,DataCompare>& s){
	cout << "printing set" << endl;
	set<Data,DataCompare>::iterator iter;
	for( iter = s.begin(); iter != s.end(); iter++ ){
		cout << "data: value1 = " << iter->value1 << ", value2 = " << iter->value2 << endl;
	}
	cout << endl;
}

int main(){
	set<Data, DataCompare> s1;
	Data d1;
	d1.value1 = 10;
	d1.value2 = 20;

	s1.insert(d1); //insert d1

	d1.value1 = 10;
	d1.value2 = 30;

	s1.insert(d1); //d1 not inserted
	print_set(s1);

}
