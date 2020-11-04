#ifndef __GROWABLE_ARRAY_H__
#define __GROWABLE_ARRAY_H__
#include "Array.h"
using namespace std;

template <class T> class GrowableArray : public Array<T> {
  public:
    GrowableArray(int size);
    virtual T &operator[](int i);
};

template <class T> GrowableArray<T>::GrowableArray(int size) {
    if (size < 0) {
        cerr << "Input (Size) cannot be negative." << endl;
        exit(-1);
    }
    Array<T>::data = new T[size];

    if (Array<T>::data == NULL) {
        cerr << "Failed to allocate memory." << endl;
        exit(-1);
    }
    Array<T>::len = size;
}

template <class T> T &GrowableArray<T>::operator[](int i) {
	if(i<Array<T>::len && i>=0)
        return Array<T>::data[i];

	int new_length=this->len;
	while(new_length <= i)
		new_length *=2;
    T *new_data = new T[new_length];
    if (new_data == NULL) {
        cerr << "Failed to allocate memory." << endl;
        exit(-1);
    }

    for (int i = 0; i < Array<T>::len; i++)
        new_data[i] = Array<T>::data[i];
    for (int i = Array<T>::len; i < new_length; i++)
        new_data[i] = 0;

    T *dontknow = Array<T>::data;
    Array<T>::data = new_data;
    Array<T>::len = new_length;
    delete[] dontknow;

    return Array<T>::data[i];
}

#endif
