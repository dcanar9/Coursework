#pragma once
#include <iostream>

//Dennis Canar
//This is my work
//Vector class

using namespace std;
template <class T>
class myGenericVec {
private:
	int numElements;
	int capacity;
	T * arr;
	void ensureCapacity(int newCap);
	bool contains(T val);
public:
	//constructor
	myGenericVec();
	//destructor
	~myGenericVec();
	//copy constructor
	myGenericVec(const myGenericVec & rhs);
	myGenericVec<T>& operator= (const myGenericVec<T>& n);
	int size() const noexcept;
	void resize(int n);
	void resize(int n, const int & val);
	void push_back(T& val);
	void push_back(const T & val);
	void pop_back();
	T& operator[] (int n);
	const T& operator[] (int n)const;
	T& front();
	const T& front()const;
	T& back();
	const T& back()const;
	bool empty() const noexcept;
	void shrink_to_fit();
	void reserve(int n);
	void clear();
	int getCapacity() const noexcept;

	friend ostream& operator<<(ostream & out, const myGenericVec<T> & rhs) {
		out << "[";
		for (int i = 0; i < rhs.numElements; i++) {
			out << rhs.arr[i] << " | ";
		}
		out << "]";
		return out;
	}
};

template<class T>
inline void myGenericVec<T>::ensureCapacity(int newCap)
{
	//do i need to expand the exisisting space
	if (capacity < newCap) {
		T * arrPtr = new T[newCap];

	//if i do then make a temp with size and copy elements
		for (int i = 0; i < numElements; i++)
			arrPtr[i] = arr[i];
		delete[] arr;
		arr = arrPtr;
		capacity = newCap;
	}
}

template<class T>
inline bool myGenericVec<T>::contains(T val)
{
	//goes through each element and checks if val is one of them
	for (int i = 0; i < numElements; i++)
	{
		if (val == arr[i])
			return true;
	}
	return false;
}

template<class T>
inline myGenericVec<T>::myGenericVec()
{
	capacity = 10;
	numElements = 0;
	arr = new T[capacity];
}

template<class T>
inline myGenericVec<T>::~myGenericVec()
{
	//destroys vector at the end of the program
	if (arr != nullptr)
		delete[]arr;
}

template <class T>
inline 	myGenericVec<T>::myGenericVec(const myGenericVec<T> & rhs) {
	//this is our copy constructor creates vector with same specs
	numElements = rhs.numElements;
	capacity = rhs.capacity;
	arr = new T[rhs.capacity];

	//copies elements
	for (int i = 0; i < numElements; i++) {
		arr[i] = rhs[i];
	}
}


template<class T>
inline myGenericVec<T>& myGenericVec<T>::operator=(const myGenericVec<T>& n)
{
	//overloaded = operator creates a copy of a vector
	delete[] arr;
	numElements = n.numElements;
	capacity = n.numElements;
	arr = new T[n.numElements];
	for (int i = 0; i < n.numElements; i++)
		arr[i] = n.arr[i];
	return *this;
}

template<class T>
inline int myGenericVec<T>::size() const noexcept
{
	return numElements;
}

template<class T>
inline void myGenericVec<T>::resize(int n)
{
	//is n smaller than current number of elements just shrink to size wanted
	if (n < numElements) {
		numElements = n;
		shrink_to_fit();
	}
	//if n is bigger we make room and fill with default class element
	else {
		ensureCapacity(n);
			while (n > numElements) {
				arr[numElements] = T();
				numElements++;
		}
			shrink_to_fit();
	}
}

template<class T>
inline void myGenericVec<T>::resize(int n, const int & val)
{
	//same as other resize just fo a constant vector
	if (n < numElements) {
		numElements = n;
		shrink_to_fit();
	}
	else {
		ensureCapacity(n)
			while (n > numElements) {
				arr[numElements] = T();
				numElements++
			}
		shrink_to_fit();
	}
}

template<class T>
inline void myGenericVec<T>::push_back(T& val)
{
	//checks if element is in the vector. If its not it will add it.
	if (!contains(val)) {
		ensureCapacity(numElements + 1);
		arr[numElements] = val;
		numElements++;
	}
}

template<class T>
inline void myGenericVec<T>::pop_back()
{
	//removes last element
	numElements--;
}

template<class T>
inline T & myGenericVec<T>::operator[](int n)
{
	//indexing overloaded operator
	return arr[n];
}

template<class T>
inline const T & myGenericVec<T>::operator[](int n) const
{
	//for constant vector
	return arr[n];
}

template<class T>
inline T & myGenericVec<T>::front()
{
	//return first element in the vector
	return arr[0];
}

template<class T>
inline const T & myGenericVec<T>::front() const
{
	//for constant vector
	return arr[0];
}

template<class T>
inline T & myGenericVec<T>::back()
{
	//returns last element in vector
	return arr[numElements - 1];
}

template<class T>
inline const T & myGenericVec<T>::back() const
{
	//for constant vector
	return arr[numElements - 1];
}

template<class T>
inline bool myGenericVec<T>::empty() const noexcept
{
	//checks if vector is empty by checking if it has no elements
	if (numElements == 0)
		return true;
	else
		return false;
}

template<class T>
inline void myGenericVec<T>::push_back(const T& val)
{
	//constant vector pushback
	if (!contains(val)) {
		ensureCapacity(numElements + 1);
		arr[numElements] = val;
		numElements++;
	}
}

template<class T>
inline void myGenericVec<T>::shrink_to_fit()
{
	//check to see if num elements is smaller than capacity
	//allocate an appropriate sized space
	if (numElements < capacity) {
		T * temp = new T[numElements];

		//copy elements
		for (int i = 0; i < numElements; i++)
			temp[i] = arr[i];

		delete[] arr;
		//update capacity
		capacity = numElements;
		arr = temp;
	}
}

template<class T>
inline void myGenericVec<T>::reserve(int n)
{
	//creates a bigger capacity by using ensure capacity
	ensureCapacity(n);
}

template<class T>
inline void myGenericVec<T>::clear()
{
	//gets rid of all elements in the vector
	numElements = 0;
}

template<class T>
inline int myGenericVec<T>::getCapacity() const noexcept
{
	//returns our size of our container
	return capacity;
}
