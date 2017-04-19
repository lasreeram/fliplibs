#ifndef _MY_ARRAY_DEF_H_
#define _MY_ARRAY_DEF_H_

#include <assert.h>
#include <math.h>
//based on opendatastructures.org by Pat Morin

namespace AlgoBox{

template <typename T> class Array{

	public:
		Array(){
			_length = 0;
			_arr = NULL;
		}

		Array(int len){
			_length = len;
			_arr = new T[_length];
		}

		int getLength(){
			return _length;
		}

		T& operator[] (int i){
			//debug_lib::log( "value of i is %d, length is %d", i, _length);
			if( !(i>=0 && i<_length) ){ 
				debug_lib::log( "Array: value of i is %d, length is %d", i, _length);
				std::string errstr = "Array index out of bounds - " + i;
				throw debug_lib::Exception(errstr.c_str());
			}
			return _arr[i];
		}

		Array<T>& operator= (Array<T>& b){
			if( _arr != NULL )
				delete[] _arr;
			_length = b.getLength();
			if( _length >= 0 )
				_arr = new T[_length];
			for( int i=0; i<_length; i++){
				_arr[i] = b[i];
			}
			return *this;
		}

		T* operator+ (int i){
			return _arr+i;
		}

	private:
		int _length;
		T* _arr;


};

/*
The ArrayStack implements the list interface
	set(i) - O(1)
	get(i) - O(1)
	add(i,value) - O(1+n-i)
	remove(i) - O(1+n-i)

	Beginning with an empty ArrayStack and performing any sequence of m add(i,x) and remove(i) operations
	results in a total of O(m) time spent during all calls to resize()
*/

template <typename T> class ArrayStack{
	public:
		ArrayStack(){
			_used = 0;
		}

		void init(int size){
			Array<T> b(std::max(size, 1));
			_arr = b;
		}

	
		T get(int i){
			return _arr[i];
		}

		T set(int i, T x){
			T y = _arr[i];
			_arr[i] = x;
			return y;
		}

		int getSize(){
			return _used;
		}
	
		virtual void add(int i, T x){
			debug_lib::log( "ArrayStack: add called for index is %d", i );
			if( (_used+1) > _arr.getLength() ) 
				resize();
			for( int j = _used; j > i; j-- ){
				_arr[j] = _arr[j-1];
			}
			_arr[i] = x;
			_used++;
			debug_lib::log( "ArrayStack: number of used entries = %d", _used );
		}

		virtual T remove( int i ){
			debug_lib::log( "ArrayStack: remove called for index is %d", i );
			T x = _arr[i];
			for( int j = i; j < _used-1; j++ )
				_arr[j] = _arr[j+1];
			_used--;
			if( _arr.getLength() >= 3*_used ) 
				resize();
			debug_lib::log( "ArrayStack: number of used entries = %d", _used );
			return x;
		}

	protected:
		int _used;
		Array<T> _arr;
		virtual void resize(){
			debug_lib::log( "ArrayStack: resize called - backing array size is %d", _arr.getLength() );
			Array<T> b(std::max(2*_used, 1));
			for( int i = 0; i < _used; i++ )
				b[i] = _arr[i];
			_arr = b;
			debug_lib::log( "ArrayStack: resize called - new backing array size is %d", _arr.getLength() );
		}

		template<typename S> friend class DualArrayDeQueue;

};

template<typename T> class FastArrayStack : public ArrayStack<T> {
	using ArrayStack<T>::_used;
	using ArrayStack<T>::_arr;

	public:
		void add(int i, T x){
			debug_lib::log( "FastArrayStack: add called for index is %d", i );
			if( (_used+1) > _arr.getLength() ) 
				resize();
			std::copy_backward( _arr+i, _arr+_used, _arr+_used+1 );
			_arr[i] = x;
			_used++;
			debug_lib::log( "FastArrayStack: number of used entries = %d", _used );
		}

	
	protected:
		void resize(){
			debug_lib::log( "FastArrayStack: resize called - backing array size is %d", _arr.getLength() );
			Array<T> b(std::max(2*_used, 1));
			std::copy( _arr+0, _arr+_used, b+0);
			_arr = b;
			debug_lib::log( "FastArrayStack: resize called - new backing array size is %d", _arr.getLength() );
		}
	
};

/*
	Data structures built using ArrayStack
*/

template <typename T> class Stack{
	public:
		Stack(){
		}

		void push(T x){
			_arrayStack.add(_arrayStack.getSize(), x );	
		}

		T pop(){
			return _arrayStack.remove(_arrayStack.getSize()-1);
		}

	private:
		ArrayStack<T> _arrayStack;
};

template <typename T> class ArrayQueue{
	public:
		ArrayQueue(){
			_start = 0;
			_used = 0;
		}

		void addToQueue(T x){
			add(x);
		}

		T getNext(){
			return remove();
		}

	private:
		void add(T x){
			if( _used + 1 > _arr.getLength() )
				resize();
			debug_lib::log( "index into the array = %d", _start+_used % _arr.getLength() );
			_arr[_start+_used % _arr.getLength() ] = x;
			_used++;
			return;
		}

		T remove(){
			debug_lib::log( "remove index %d", _start );
			T x = _arr[_start];
			_start = (_start + 1) % _arr.getLength();
			_used--;
			if( _arr.getLength() >= 3*_used ) 
				resize();
			return x;
		}
		
		Array<T> _arr;
		int _start;
		int _used;
		virtual void resize(){
			debug_lib::log( "ArrayQueue: resize called - backing array size is %d", _arr.getLength() );
			Array<T> b(std::max(2*_used, 1));
			for( int i = 0; i < _used; i++ )
				b[i] = _arr[(i+_start) % _arr.getLength()];
			_arr = b;
			_start = 0;
			debug_lib::log( "ArrayQueue: resize called - new backing array size is %d", _arr.getLength() );
		}

};

template <typename T> class ArrayDeQueue{
	public:
		//Operations can be done to both ends of the queue
		ArrayDeQueue(){
			_start = 0;
			_used = 0;
		}

		T getFirst(){
			return get(_start);
		}

		T getLast(){
			return get((_start+_used-1)%_arr.getLength());
		}

		void addFirst(T x){
			add(_start, x);
		}

		void addLast(T x){
			add((_start+_used)%_arr.getLength(), x);
		}

		T removeFirst(){
			return remove(_start);
		}

		T removeLast(){
			return remove((_start+_used-1)%_arr.getLength());
		}

		int getSize(){
			return _used;
		}

	private:
		Array<T> _arr;
		int _start;
		int _used;

		T get(int i){
			return _arr[(_start+i)%_arr.getLength()];
		}

		T set(int i, T x){
			T y = get(i);
			_arr[(_start+i)%_arr.getLength()] = x;
			return y;
		}

		void add(int i, T x){
			if( _used+1 > _arr.getLength() )
				resize();

			if( i < _used/2 ){
				_start = (_start == 0)? _arr.getLength()-1: _start-1;
				for( int k = 0; k <= i-1; k++ )
					_arr[(_start+k)%_arr.getLength()] = _arr[(_start+k+1)%_arr.getLength()];
			}else{
				for( int k = _used; k > i; k-- )
					_arr[(_start+k)%_arr.getLength()] = _arr[(_start+k-1)%_arr.getLength()];
			}

			_arr[(_start+i)%_arr.getLength()] = x;
			_used++;
		}

		T remove(int i){
			T x = _arr[(_start+i)%_arr.getLength()];
			if( i < _used/2 ){
				_start = (_start+1) % _arr.getLength();
				for( int k = i; k > 0; k-- )
					_arr[(_start+k)%_arr.getLength()] = _arr[(_start+k-1)%_arr.getLength()];
			}else{
				for( int k = i; k < _used-1; k++ )
					_arr[(_start+k)%_arr.getLength()] = _arr[(_start+k+1)%_arr.getLength()];
			}
			
			_used--;
			if( 3*_used < _arr.getLength() ) 
				resize();
			return x;
		}


		
		void resize(){
			debug_lib::log( "ArrayDeQueue: resize called - backing array size is %d", _arr.getLength() );
			Array<T> b(std::max(2*_used, 1));
			for( int i = 0; i < _used; i++ )
				b[i] = _arr[(i+_start) % _arr.getLength()];
			_arr = b;
			_start = 0;
			debug_lib::log( "ArrayDeQueue: resize called - new backing array size is %d", _arr.getLength() );
		}

};

/*
	DualArrayDeQueue - performance characteristics
	
	getFirst() - O(1)
	addFirst() - O(1+min(i,n-1)
	removeFirst() - O(1+min(i,n-1)

	getLast() - O(1)
	addLast() - O(1+min(i,n-1)
	removeLast() - O(1+min(i,n-1)

	Any sequence of m add, remove calls results in a total of O(m) time spent during all calls to balance()
	and resize() 
*/

template <typename T> class DualArrayDeQueue{

	public:
		DualArrayDeQueue(){
			_front.init(8);
			_back.init(8);
		}

		int getSize(){
			return _front.getSize() + _back.getSize();
		}

		//Operations can be done to both ends of the queue
		T getFirst(){
			return get(0);
		}

		T getLast(){
			return get(getSize()-1);
		}

		void addFirst(T x){
			add(0, x);
		}

		void addLast(T x){
			add(getSize(), x);
		}

		T removeFirst(){
			return remove(0);
		}

		T removeLast(){
			return remove(getSize()-1);
		}
	
	private:
		ArrayStack<T> _front;
		ArrayStack<T> _back;

		T get(int i){
			if( i < _front.getSize() ){
				return _front.get(_front.getSize() - i - 1);
			}else{
				return _back.get(i-_front.getSize());
			}
		}
	
		T set(int i, T x){
			if( i < _front.getSize() ){
				return _front.set(_front.getSize() - i - 1, x);
			}else{
				return _back.set(i-_front.getSize(), x);
			}
		}

		void add( int i, T x ){
			if( i < _front.getSize() ){
				return _front.add(_front.getSize() - i, x);
			}else{
				return _back.add(i-_front.getSize(), x);
			}
			balance();
		}

		T remove(int i){
			T x;
			if( i < _front.getSize() ){
				debug_lib::log( "remove from front called" );
				x = _front.remove(_front.getSize() - i -1);
			}else{
				debug_lib::log( "remove from front called" );
				x = _back.remove(i-_front.getSize());
			}
			balance();
			return x;
		}
	

		void balance(){
			if( 3*_front.getSize()  < _back.getSize() ||
			 	3*_back.getSize() < _front.getSize() ){
				int n = _front.getSize() + _back.getSize();
				int nf = n/2;
				Array<T> af(std::max(2*nf, 1));
				for(int i = 0; i < nf; i++ ){
					af[nf-i-1] = get(i);
				}
				int nb = n - nf;
				Array<T> ab(std::max(2*nb, 1));
				for(int i=0; i< nb; i++ ){
					ab[i] = get(nf+i);
				}
				_front._arr = af;
				_front._used = nf;
				_back._arr = ab;
				_back._used = nb;
			}
		}
};

template <typename T> class RootishArrayStack{

	public:
		RootishArrayStack(){
			_used = 0;
		}

		int getSize(){
			return _used;
		}
	
		T get(int i){
			int block = indexToBlock(i);
			int j = i - (block*(block+1)/2);
			debug_lib::log( "RootishArrayStack: get called with index = %d, i is = %d, (block*(block+1)/2) is %d", j, i, (block*(block+1)/2) );
			return _blocks.get(block)[j];
		}

		T set(int i, T x){
			int block = indexToBlock(i);
			int j = i - (block*(block+1)/2);
			debug_lib::log( "RootishArrayStack: set called with index = %d, i is = %d, (block*(block+1)/2) is %d", j, i, (block*(block+1)/2) );
			
			T y = _blocks.get(block)[j];
			_blocks.get(block)[j] = x;
		}

		void add( int i, T x ){
			int r = _blocks.getSize();
			debug_lib::log( "RootishArrayStack: inside rootish add %d value of indx 0 is %ld", r );
			if( r*(r+1)/2 < _used+1 )
				grow();
			_used++;
			for( int j = _used-1; j > i; j-- )
				set(j, get(j-1));
			set(i, x);
		}


		T remove(int i){
			T x = get(i);
			for(int j = i; j < _used-1; j++)
				set(j, get(j+1));
			_used--;
			int r = _blocks.getSize();
			if( (r-2)*(r-1)/2 >= _used )
				shrink();
			return x;
		}

	private:
		ArrayStack<T*> _blocks;
		int _used;
		int indexToBlock(int i){
			double db = (-3.0 + sqrt(9 + 8*i)) / 2.0;
			int block = (int)ceil(db);
			return block;
		}

		void grow(){
			debug_lib::log( "RootishArrayStack: grow called %d", _blocks.getSize() );
			_blocks.add(_blocks.getSize(), new T[_blocks.getSize()+1]);
		}
	
		void shrink(){
			int r = _blocks.getSize();
			while( r > 0 && (r-2)*(r-1)/2 >= _used ){
				delete[] _blocks.remove(_blocks.getSize()-1);
				r--;
			}
		}
};

/*
	Bounded Dequeue used by SpaceEfficientLinkedList
*/
template <typename T> class BDeQueue : public ArrayDeQueue<T> {
	public:
		BDeQueue(int b){
			_used = 0;
			j = 0;
			Array<int> z(b+1);
			a = z;
		}

		~BDeQueue(){
		}

		void add(int i, T x){
			ArrayDeQueue<T>::add(i, x);
		}

		void add(T x){
			ArrayDeQueue<T>::add(getsize(), x);
			return;
		}

		void resize(){}
};

}
#endif
