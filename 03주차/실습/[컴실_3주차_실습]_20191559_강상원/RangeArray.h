#ifndef MTT
#define MTT
#include "Array.h"

class RangeArray : public Array{
	protected : 
		int low;
		int high;
	public :
		RangeArray(int,int);
		~RangeArray();

		int baseValue();
		int endValue();

		int& operator[](int);
		int operator[](int) const;
	
};
#endif
