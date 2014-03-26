#pragma once

using namespace std;

template <class T> 
class FixedVector
{
public:
	FixedVector(int _size) : size(_size) {}

	~FixedVector(void) {}

	vector<T> getVector() {
		return vec;
	}
	
	void insertElement(T el) {
		if(vec.size() == size) {
			vec.erase(vec.begin());
		}
		
		vec.push_back(el);
	}

	vector<T> performDFT() {
		vector<T> outVec(size);
		dft(vec, outVec);
		return outVec;
	}


private:
	vector<T> vec;
	int size;
};

