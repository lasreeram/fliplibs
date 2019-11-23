#ifndef _MY_GRAPHS_DEF_H_
#define _MY_GRAPHS_DEF_H_

#include <assert.h>
#include <math.h>
#include <ArrayList.h>
#include <LinkedList.h>
//based on opendatastructures.org by Pat Morin

namespace AlgoBox{

//A more careful implementation can pack w boolean values in to each word of memory could reduce this space to O(n^2/w) words of memory
class AdjacencyMatrix{
	bool** _adj;
	int _numberOfVertices;
	AdjacencyMatrix(int numberOfVertices){
		_adj = new bool[numberOfVertices][numberOfVertices];
		_numberOfVertices = numberOfVertices;
	}

	void addEdge(int i, int j){
		_adj[i][j] = true;
	}

	void removeEdge(int i, int j){
		_adj[i][j] = false;
	}

	bool hasEdge(int i, int j){
		return _adj[i][j];
	}

	~AdjacencyMatrix(){
		if( _adj )
			delete _adj;
		_adj = NULL;
	}
};

class AdjacencyList{
	int _numberOfVertices;
	SinglyLinkedList<int>* _adj;
	AdjacencyList(int numberOfVertices){
		_numberOfVertices = numberOfVertices;
		_adj = new SinglyLinkedList<int>[_numberOfVertices];
	}
	~AdjacencyList(){
		if( _adj )
			delete _adj;
	}

	void addEdge(int i, int j){
		_adj[i].addToQueue(j);
	}

	void removeEdge(int i, int j){
		for( int k = 0; k < _adj[i].size(); k++ ){
			if( _adj[i].get(k) == j ){
				_adj[i].removeFirst(k);
				return;
			}
		}
	}

	bool hasEdge(int i, int j){
		return _adj[i].contains(j);
	}


};

//Despite the high memory usage and poor performance of theinEdges(i) andoutEdges(i)operations, an AdjacencyMatrixcan still be useful for some applications. 
//In particular,when the graphGisdense, i.e., it has close to n^2 edges, then a memory usage of n^2 may be acceptable.
template <typename StoreDataStructure> class Graph{
	StoreDataStructure* _storage;
	int _numberOfVertices;

	Graph(int numberOfVertices){
		_storage = new StoreDataStructure(numberOfVertices);
		_numberOfVertices = numberOfVertices;
	}
	
	~Graph(){
		if( _storage )
			delete _storage;
		_numberOfVertices = 0;
	}

	void addEdge(int i, int j){
		return _storage->addEdge(i, j);
	}

	void removeEdge(int i, int j){
		return _storage->removeEdge(i, j);
	}

	bool hasEdge(int i, int j){
		return _storage->hasEdge(i, j);
	}

	void inEdges(int i, SinglyLinkedList<int>& list){
		for( int j = 0; j < _numberOfVertices; j++ )
			if( _storage->hasEdge(i, j) ) list.addToQueue(j);
	}

	void outEdges(int j, SinglyLinkedList<int>& list){
		for( int i = 0; i < _numberOfVertices; i++ )
			if( _storage->hasEdge(i, j) ) list.addToQueue(i);
	}
};

}
#endif
