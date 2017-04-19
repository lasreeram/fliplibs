#ifndef _MY_LINKEDLIST_DEF_H_
#define _MY_LINKEDLIST_DEF_H_

#include <assert.h>
#include <math.h>
//based on opendatastructures.org by Pat Morin

//Linked list are list interfaces based on pointer-based data structures rather than arrays

//Disadvantage - get(i), set(i) are no longer O(1) but O(min(i, n-i))
//Advantage - add, remove a node in constant time

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
There traversing of the list as it would take O(n) time
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
	private:
		Node<T>* _head;
		Node<T>* _tail;
		int _used;
};

template <typename T> class NodeD {
	public:
		T x;
		NodeD* next;
		NodeD* prev;
		NodeD( T x0 ){
			x = x0;
			next = NULL;
		}
};

/*
all operations (get, set, add, remove) run in O(min(i, used-i). All operations require getNode.
which takes O(min(i,used-1)).

In certain applications it is possible to keep references to the nodes in some external Data structure.
In such cases getNode can be avoided. The below interface exposes direct operations with Node* to allow this.

Why the dummy node?
Using a dummy node avoids a lot of special cases required when using _head and _tail pointers.
With dummy node _head is no longer required. The _used value will tell us if the dummy is the only node in the chain.

*/

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
		NodeD<T>* getNode(i){
			if( _used == 0 )
				throw debug_lib::Exception( "SinglyLinkedList:getNode no more elements" );
			if( i+1 > _used )
				throw debug_lib::Exception( "SinglyLinkedList:getNode no such index i=" + i );
				
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

template <typename T> class NodeB {
	public:
		NodeB<T>* next;
		NodeB<T>* prev;
		BDQueue<T> _blockdq;
		NodeB(){
		}
		NodeB(int b):_blockdq(b){
		}
};

template <typename T> class Location {
	public:
		NodeB* _u;
		int j;
		Location(){}
		Location(NodeB<T>* u, int j){
			this->u = u;
			this->j = j;
		}
};


template <typename T> class SpaceEfficientLinkedList {
	public:

		SpaceEfficientLinkedList( int blocks ){
			_sizeOfBLocks = blocks;
			_used = 0;
		}

		T get(int i){
			Location<T> loc;
			getLocation(i, loc);
			return loc.u->_blockdq.get(loc.j);
		}

		T set(int i, T x){
			Location<T> loc;
			getLocation(i, loc);
			T y = loc.u->_blockdq.get(loc.j);
			loc.u->_blockdq.set(loc.j, x);
			return y;
		}

		//only exposed so that can be used with Node* from an external DS. 
		//needs to be reviewed
		NodeB<T>* addBefore(NodeB<T>* w, T x){
			NodeB<T>* u = new NodeB<T>(_numBlocks);
			u->x = x;
			u->prev = w->prev;
			u->next = w;
			u->next->prev = u;
			u->prev->next = u;
			_used++;
			
			return u;
		}

		//Add to the Last
		void addLast(T x){
			Node<B>* last = _dummy.prev;
			if( last == &_dummy ){
				addBefore(&_dummy, x);
			}else if ( last->getSize() == b+1 ){
				addBefore(&_dummy, x);
			}else{
				last->_blockdq.addLast(x);
				_used++;
			}
		}


		void add(int i, T x){
			if( i == _used ){
				addLast(x);
				return;
			}

			Location loc;
			getLocation(i, loc);
			NodeD<T>* u = loc.u;

			int r = 0;
			//Look for a node that has space for the new element
			//Limit the search by _blockSize steps (refer analysis)
			//If can't find within _blockSize steps make space
			while( r < _blockSize && u != &_dummy && u->_blockdq.size() == b+1 ){
				u = u->next;
				r++;
			}

			if( r == _blockSize ){ //Need to make space
				spread(loc.u);
				u = loc.u;
			}else if( u == &_dummy ){ //the list is empty. Add before dummy
				addBefore(&_dummy, x);
			}

			
			while( u != loc.u ){
				u->_blockdq.add(0, u->prev->_blockdq.remove(u->prev->_blockdq.size()-1));
				u = u->prev;
			}

			u->_blockdq.add(loc.j, x);
			_used++;
		}


		T remove(int i){
			Location loc;
			getLocation(i, loc);

			T y = loc.u->_blockdq.get(loc.j);
			NodeD<T>* u = loc.u;

			int r = 0;
			while( r < _blockSize && u != &_dummy &&  u->_blockdq.size() == b-1 ){
				u = u->next;
				r++;
			}

			if( r == _blockSize ){
				gather(loc.u);
			}

			u = loc.u;
			u->_blockdq.remove(loc.j);
			while( u->_blockdq.getSize() < b-1 && u->next != &_dummy ){
				u->_blockdq.add(u->next->_blockdq.remove(0));
				u = u->next;
			}

			if( u->_blockdq.getSize() == 0 )
				remove(u);

			_used--;
			return y;
		}

		

	private:
		int _used;
		int _blockSize;
		NodeB<T> _dummy;

		void getLocation(int i, Location<T>& ell){
			if( i < _used/2){
				NodeB<T>* u = _dummy.next;
				while( i >= u->_blockdq.getSize() ){
					i -= u->_blockdq.getSize();
					u = u->next;
				}
				ell.u = u;
				ell.j = i;
			} else{
				NodeB<T>* u = &_dummy;
				int idx = _used;
				while ( i < idx ){
					u = u->prev;
					idx -= u->_blockdq.getSize();
				}
				ell.u = u;
				ell.j = i - idx;
			}
		}

		void spread(NodeB<T>* u){
			NodeB<T>* w = u;
			for( int j = 0; j < _blockSize; j++ ){
				w = w-next;
			}

			w = addBefore(w);
			while( w != u ){
				while( w->_blockdq.getSize() < b ){
					w->_blockdq.add( 0, w->prev->_blockdq.remove(w->prev->_blockdq.getSize()-1));
				}
				w = w->prev;
			}
		}

		void gather( Node* u ){
			NodeB<T>* w = u;
			for( int j = 0; j < _blockSize-1; j++ ){
				while( w->_blockdq.getSize() < b )
					w->_blockdq.add( w->next->_blockdq.remove(0));
				w = w-next;
			}
			remove(w);
		}
		
};

}
#endif
