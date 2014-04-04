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

	T firFilter(vector<double> weights) {

		T sum = 0;

		if(weights.size() > vec.size()) {
			return vec.at(vec.size()-1);
		}

		for(int i=0; i < weights.size(); i++) {
			T val = vec.at(vec.size() - weights.size() + i);
			sum = sum + val * weights.at(i);
		}


		return sum;

	}


private:
	vector<T> vec;
	int size;
};

