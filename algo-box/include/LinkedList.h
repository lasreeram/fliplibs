#ifndef _MY_LINKEDLIST_DEF_H_
#define _MY_LINKEDLIST_DEF_H_

#include <assert.h>
#include <math.h>
#include <ArrayList.h>
//based on opendatastructures.org by Pat Morin

//Linked list are list interfaces based on pointer-based data structures rather than arrays

//Disadvantage over arrays - get(i), set(i) are no longer O(1) but O(min(i, n-i))
//Advantage over arrays - add, remove a node in O(1) over an array which requires shifting. Ofcourse this requires the pointer to the node to be saved.

namespace AlgoBox{

template <typename T> class Node {
	public:
		T x;
		Node* next;
		Node( T x0 ){
			x = x0;
			next = NULL;
		}
};

/*
Singly linked list provide a limited set of operations but all in O(1) time

push, pop, addToQueue, getNext

These operations operate on the head or tail of the list only.
Inserting at ith location or get(i) will require traversing of the list and would take O(n) time
*/

template <typename T> class SinglyLinkedList{

	public:
		SinglyLinkedList(){
			_head = NULL;
			_tail = NULL;
			_used = 0;
		}

		void push(T x){
			Node<T>* u = new Node<T>(x);
			u->next = _head;
			_head = u;
			if( _used == 0)
				_tail = u;
			_used++;
			return;	
		}

		T pop(){
			if( _used == 0 )
				throw debug_lib::Exception( "SinglyLinkedList:pop: no more elements" );
			T x = _head->x;
			Node<T>* u = _head;
			_head = _head->next;
			delete u;
			_used--;
			if( _used == 0 )
				_tail = NULL;
			return x;
		}

		void addToQueue(T x){
			Node<T>* u = new Node<T>(x);
			if( _used == 0 ){
				_head = u;
			}else{
				_tail->next = u;
			}
			_tail = u;
			_used++;
			return;
		}

		T get(int i){
			if( (i+1) > _used )
				throw debug_lib::Exception( "SinglyLinkedList:getNext no more elements" );
			Node<T>* node = _head;
			for( int j = 0; j < i; j++ ){
				node = node->next;
			}
			return node->x;
		}

		T getNext(){
			if( _used == 0 )
				throw debug_lib::Exception( "SinglyLinkedList:getNext no more elements" );
			T x = _head->x;
			Node<T>* u = _head;		
			_head = _head->next;
			delete u;
			_used--;
			if( _used == 0 ) 
				_tail = NULL;
			return x;
		}

		bool removeFirst(T& x){
			if( _used == 0 )
				return false;

			Node<T>* prevnode = _head;
			Node<T>* node = _head;
			while ( node ){
				if( node->x == x ){
					if( prevnode == _head ){
						pop();	
						return true;
					}
					prevnode->next = node->next;
					delete node;
					return true;
				} 
				prevnode = node;
				node = node->next;
			}
			return false;
		}

		bool contains(T& x){
			if( _used == 0 )
				return false;

			Node<T>* node = _head;
			while ( node ){
				if( node->x == x ){
					return true;
				}
				node = node->next;
			}
			return false;
		}
	private:
		Node<T>* _head;
		Node<T>* _tail;
		int _used;
};

/*
all operations (get, set, add, remove) run in O(min(i, used-i). All operations require getNode.
which takes O(min(i,used-1)).

In certain applications it is possible to keep references to the nodes in some external Data structure.
In such cases getNode can be avoided. The below interface exposes direct operations with Node* to allow this.

Why the dummy node?
Using a dummy node avoids a lot of special cases required when using _head and _tail pointers.
With dummy node _head is no longer required. The _used value will tell us if the dummy is the only node in the chain.

Contrast with arrays.
Arrays implement getNode in O(1) time and insertion or deletion require O(n)
Doubly linked list implement getNode in O(n) time and insertion, deletion in O(1). So an application that can cache the node pointers effectively can avoid getNode and gets all operations in constant time.

Using a Doubly linked list along with USet
The node pointers can be stored in a USet(Unordered set) by their values. The node at ith location can be quickly (O(log)?) found using an USet and all other operations can be done in constant time

*/

template <typename T> class NodeD {
	public:
		T x;
		NodeD* next;
		NodeD* prev;
		NodeD(){
			next = NULL;
		}
		NodeD( T x0 ){
			x = x0;
			next = NULL;
		}
};

template <typename T> class DoublyLinkedList {
	public:
		DoublyLinkedList(){
			_used = 0;
			_dummy.next = &_dummy;
			_dummy.prev = &_dummy;
		}

		T get(int i){
			NodeD<T>* n = getNode(i);
			return n->x;
		}

		T set(int i, T x){
			NodeD<T>* n = getNode(i);
			T y = n->x;
			n->x = x;
			return y;
		}

		//
		//this function allows storing of the pointer in an external Data structure.
		//
		NodeD<T>* addBefore(NodeD<T>* w, T x){
			NodeD<T>* u = new NodeD<T>(x);
			u->prev = w->prev;
			u->next = w;
			u->next->prev = u;
			u->prev->next = u;
			_used++;
			
			return u;
		}

		void add(int i, T x){
			addBefore( getNode(i), x );
			return;
		}

		//
		//this function allows storing of the pointer in an external Data structure.
		//
		T remove(NodeD<T>* w){
			w->prev->next = w->next;
			w->next->prev = w->prev;
			_used--;
			T y = w->x;
			delete w;
			return y;
		}

		T remove(int i){
			NodeD<T>* w = getNode(i);
			return remove(w);
		}
		
		//
		//this function allows storing of the pointer in an external Data structure.
		//
		NodeD<T>* getNode(int i){
			if( _used == 0 )
				throw debug_lib::Exception( "DoublyLinkedList:getNode no more elements" );
			if( i+1 > _used )
				throw debug_lib::Exception( "DoublyLinkedList:getNode no such index i=" + i );
				
			NodeD<T>* ptr;
			if( i < _used/2 ){
				ptr = _dummy.next;
				for(int idx = 0; idx < i; idx++ ){
					ptr = ptr->next;	
				}
				
			}else{
				ptr = _dummy.prev;
				for( int idx = 0; idx < (_used-i-1); idx++ ){
					ptr = ptr->prev;
				}
			}
			return ptr;
		}

	private:
		int _used;
		NodeD<T> _dummy;
};


/*
// - Space efficient linked list - a doubly linked list of blocks instead of individual elements
// - SE Linked list has a very tight restriction on the number of elements. Each block contains at-least (b-1) and atmost (b+1) elements
// - Wasted Space is of the order of O(b + n/b) - i.e. the last block is partially filled and all other blocks have atleast b-1 elements
// - By choosing b to be square root of n we can keep the space wasted to O(n^0.5)
// - 
template <typename T> class NodeSpaceEfficient {
	public:
		NodeSpaceEfficient<T>* next;
		NodeSpaceEfficient<T>* prev;
		BDeQueue<T> _blockdq;
		NodeSpaceEfficient(){
		}
		NodeSpaceEfficient(int b):_blockdq(b){
		}

		int getUsed(){
			return _blockdq.getUsed();
		}

		void set(int i, T x){
			_blockdq.set(i, x);
		}

		void insert(int i, T x){
			_blockdq.set(i, x);
		}

		void add(T x){
			_blockdq.add(x);
		}
};

template <typename T> class Location {
	public:
		NodeSpaceEfficient<T>* _u;
		int j;
		Location(){}
		Location(NodeSpaceEfficient<T>* u, int j){
			this->u = u;
			this->j = j;
		}
};


template <typename T> class SpaceEfficientLinkedList {
	public:

		SpaceEfficientLinkedList( int blockSize ){
			_numBlocks = 0;
			_blockSize = blockSize;
			_used = 0;
		}

		T getFirst(){
			return get(0);
		}

		T get(int i){
			Location<T> loc;
			getLocation(i, loc);
			return loc.u->_blockdq.get(loc.j);
		}

		T setFirst(T x){
			return set(0, x);
		}

		T set(int i, T x){
			Location<T> loc;
			getLocation(i, loc);
			T y = loc.u->_blockdq.get(loc.j);
			loc.u->_blockdq.set(loc.j, x);
			return y;
		}

		//Add to the Last
		void addLast(T x){
			NodeSpaceEfficient<T>* lastBlock = _dummy.prev;
			if( lastBlock == &_dummy ){
				lastBlock = addNewBlockBefore(&_dummy);
			}else if ( last->getUsed() == _blockSize+1 ){ //atmost b-1 - block is full
				lastBlock = addNewBlockBefore(&_dummy);
			}
			lastBlock->_blockdq.addLast(x);
			_used++;
		}


		void add(int i, T x){
			if( i >= _used || i < 0 ){
				throw debug_lib::Exception( "SpaceEfficientLinkedList:index out of bounds:" + i );
			}

			Location<T> loc;
			getLocation(i, loc);
			NodeSpaceEfficient<T>* candidate = loc.u;

			int r = 0;
			//"Gotcha":code to ensure that at all times - All blocks except the last block, have atleast (blocksize-1) elements

			//Look for a candidate that has less than _blocksize+1 elements. It must be either the last block or 
			//some intermediate block that has (blocksize-1) or blocksize 

			//Restrict search within _blockSize steps
			while( r < _blockSize && u != &_dummy && u->_blockdq.size() == _blockSize+1 ){
				candidate = candidate->next;
				r++;
			}


			if( u == &_dummy ){ //Case 1 - Could not find candidate block, ran out of blocks and all blocks are full.
				candidate = addNewBlockBefore(&_dummy);

				//Shift all elements until loc.u block to make space
				while( candidate != loc.u ){
					candidate->_blockdq.add(0, candidate->prev->_blockdq.remove(candidate->prev->_blockdq.getSize()-1));
					candidate = candidate->prev;
				}
			}else if( r < _blockSize ){ //Case 2 - Candidate block is within _blockSize steps

				//Shift all elements until loc.u block to make space
				while( candidate != loc.u ){
					//adding an element to 0th position of blockdq causes internal shifting within blockdq
					candidate->_blockdq.add(0, candidate->prev->_blockdq.remove(candidate->prev->_blockdq.getSize()-1));
					candidate = candidate->prev;
				}
				candidate->_blockdq.add(loc.j, x);
			}else //if( r == _blockSize ){  Case 3 - All blocks are of size _blockSize+1 within _blockSize steps.

				spread(loc.u); // "Gotcha": "b blocks of each (b+1) size" are spread to "(b+1) blocks of each size b"
				candidate = loc.u;
				candidate->_blockdq.add(loc.j, x); //no shifting - thanks to spread, candidate is now of size=b
			}

			_used++;
		}


		T remove(int i){
			Location<T> loc;
			getLocation(i, loc);

			T y = loc.u->_blockdq.get(loc.j);
			NodeSpaceEfficient<T>* candidate = loc.u;
			//"Gotcha":code to ensure that at all times - All blocks except the last block, have atleast (blocksize-1) elements

			//Look for a candidate that has more than (b-1) elements
			int r = 0;
			while( r < _blockSize && candidate != &_dummy &&  candidate->_blockdq.size() == _blockSize-1 ){
				candidate = candidate->next;
				r++;
			}

			NodeSpaceEfficient<T>* blockPtr = NULL;
			//Case 1 and 2 - found a block within r < _blockSize.
			//Note that in the case of candidate==dummy, the last block can have less than (b-1) elements without 
			//violating the invariant
			if( candidate == &_dummy || r < _blockSize ){ 
				blockPtr = loc.u;
				blockPtr->_blockdq.remove(loc.j);
				while( blockPtr->_blockdq.getSize() < _numBlocks-1 && blockPtr->next != &_dummy ){
					blockPtr->_blockdq.add(blockPtr->next->_blockdq.remove(0));
					blockPtr = blockPtr->next;
				}
			}else{ //r == _blockSize
				gather(loc.u);
				blockPtr = loc.u;
				blockPtr->_blockdq.remove(loc.j);
				while( blockPtr->_blockdq.getSize() < _numBlocks-1 && blockPtr->next != &_dummy ){
					blockPtr->_blockdq.add(blockPtr->next->_blockdq.remove(0));
					blockPtr = blockPtr->next;
				}
			}

			//if in the process of shifting the last block is empty remove it
			if( blockPtr->_blockdq.getSize() == 0 )
				remove(blockPtr);

			_used--;
			return y;
		}

		

	private:
		int _used;
		int _blockSize;
		NodeSpaceEfficient<T> _dummy;
		int _numBlocks;

		void getLocation(int i, Location<T>& loc){
			if( i < _used/2){
				NodeSpaceEfficient<T>* u = _dummy.next;
				while( i >= u->_blockdq.getSize() ){
					i -= u->_blockdq.getSize();
					u = u->next;
				}
				loc.u = u;
				loc.j = i;
			} else{
				NodeSpaceEfficient<T>* u = &_dummy;
				int idx = _used;
				while ( i < idx ){
					u = u->prev;
					idx -= u->_blockdq.getSize();
				}
				loc.u = u;
				loc.j = i - idx;
			}
		}


		NodeSpaceEfficient<T>* addNewBlockBefore(NodeSpaceEfficient<T>* w){
			NodeSpaceEfficient<T>* u = new NodeSpaceEfficient<T>(_blockSize+1); //additional 1 space for spreading and gathering
			u->prev = w->prev;
			u->next = w;
			u->next->prev = u;
			u->prev->next = u;
			_numBlocks++;
			return u;
		}

		void spread(NodeSpaceEfficient<T>* u){
			NodeSpaceEfficient<T>* w = u;
			for( int j = 0; j < _blockSize; j++ ){
				w = w->next;
			}

			w = addBefore(w);
			while( w != u ){
				while( w->_blockdq.getSize() < _numBlocks ){
					w->_blockdq.add( 0, w->prev->_blockdq.remove(w->prev->_blockdq.getSize()-1));
				}
				w = w->prev;
			}
		}

		void gather( NodeSpaceEfficient<T>* u ){
			NodeSpaceEfficient<T>* w = u;
			for( int j = 0; j < _blockSize-1; j++ ){
				while( w->_blockdq.getSize() < _numBlocks )
					w->_blockdq.add( w->next->_blockdq.remove(0));
				w = w->next;
			}
			remove(w);
		}
		
};
*/

}
#endif
