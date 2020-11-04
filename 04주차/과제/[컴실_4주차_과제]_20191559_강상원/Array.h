#ifndef __ARRAY__
#define __ARRAY__
#include <cstdlib>
#include <iostream>

using namespace std;

template <class T> class Array {
	protected:
		T *data;
		int len;

	public: 
		Array(){};
		Array(int size);
		~Array();

		int length() const;

		virtual T &operator[](int i);
		virtual T operator [](int i) const;

		void print();
};

template <class T> Array<T>::Array(int size){
	if(size<0){
		cerr << "Input (Size) cannot be negative." << endl;
		exit(-1);
	}
	data=new T[size];

	if (data==NULL){
		cerr << "Failed to allocate memory." << endl;
		exit(-1);
	}
	len=size;

}

template <class T> Array<T>::~Array(){
	delete[] data;
}

template <class T> int Array<T>::length() const {
	return len;
}

template <class T> T &Array<T>::operator[](int i){
	static T dontknow;
	if(i<len && i>=0) return data[i];
	else {
		cerr << "Array is out of bound." << endl;
		return dontknow;
	}
}

template <class T> T Array<T>::operator[](int i) const{
	if(i<len && i>=0) return data[i];
	else{
		cerr << "Array is out of bound." << endl;
		return 0;
	}
}

template <class T> void Array<T>::print(){
	cout << '[';
	for(int i=0; i<len; i++)
		cout << ' ' << data[i];
	cout << " ]\n";
}

#endif
