#include <debug.h>
#include <Graph.h>

int main(int argc, char** argv){
	debug_lib::init( argv[0]);
	
	AlgoBox::Graph<AscendencyList> g1(3);
	g1.addEdge(0,1);
	g1.addEdge(1,2);


	return 0;
}
