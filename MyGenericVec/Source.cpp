//Dennis Canar
//This is my work
//10 - 22 - 17

#include <iostream>
#include <string>
#include <fstream>
#include "myGenericVec.h"

using namespace std;
int main() {
	myGenericVec<int>i;
	myGenericVec<string>s;
	
	cout << "Pushing back to string vector 'Hello I am Dennis'" << endl;
	cout << "Pushing back 1 2 3 4 5 to int vector" << endl;
	s.push_back("Hello");
	s.push_back("I");
	s.push_back("am");
	s.push_back("Dennis");
	i.push_back(1);
	i.push_back(2);
	i.push_back(3);
	i.push_back(4);
	i.push_back(5);
	cout << "Displaying string vector using <<: " << s << endl;
	cout << "Displaying Interger vector using <<: " << i << endl;

	cout << endl;
	
	cout << "Pushing back Hello on string vector, it should not push back" << endl;
	cout << "Pushing back 5 on the integer vector, it should not pushback" << endl;
	s.push_back("Hello");
	i.push_back(5);
	cout << s << endl;
	cout << i << endl;

	cout << endl;

	cout << "Checking NumElements and capacity of both vectors" << endl;
	cout << "String Vector NumElements: " << s.size() << " Capacity: " << s.getCapacity() << endl;
	cout << "Integer Vector NumElements: " << i.size() << " Capacity: " << i.getCapacity() << endl;
	cout << "String vector output should be 4 & 10" << endl;
	cout << "Integer vector output should be 5 & 10" << endl;

	cout << endl;

	cout << "Checking if both Vectors are empty" << endl;
	if (!s.empty()) { cout << "String vector is not empty" << endl; }
	if (!i.empty()) { cout << "Integer vector is not empty" << endl; }

	cout << endl;

	cout << "Returning front and back of both vectors" << endl;
	cout << "String Vector Front: " << s.front() << " Back: "<< s.back() <<endl;
	cout << "Integer Vector Front: " << i.front() << " Back: " << i.back() << endl;

	cout << endl;

	cout << "Returning [2] of both vectors" << endl;
	cout << "String Vector [2]: " << s[2] << endl;
	cout << "Integer Vector [2]: " << i[2] << endl;

	cout << endl;

	cout << "Calling Reserve(15) on both vecotrs" << endl;
	s.reserve(15);
	i.reserve(15);
	cout << "String Vector Capacity: " << s.getCapacity() << " numElements: " << s.size() << endl;
	cout << "Integer Vector Capacity: " << i.getCapacity() << " numElements: " << i.size() << endl;

	cout << endl;

	cout << "Calling Shrink to fit on both Vectors" << endl;
	s.shrink_to_fit();
	i.shrink_to_fit();
	cout << "String Vector Capacity: " << s.getCapacity() << " numElements: " << s.size() << endl;
	cout << "Integer Vector Capacity: " << i.getCapacity() << " numElements: " << i.size() << endl;

	cout << endl;

	cout << "Calling pop_back on both vectors" << endl;
	s.pop_back();
	i.pop_back();
	cout << "Sting Vector After pop_back: " << s << endl;
	cout << "Integer Vector After pop_back: " << i << endl;

	cout << endl;

	cout << "Resizing both vectors to 13" << endl;
	s.resize(13);
	i.resize(13);
	cout << "String Vector After Resize: " << s << endl;
	cout << "Integer Vector After Resize: " << i << endl;
	cout << "String Vector Capacity: " << s.getCapacity() << " numElements: " << s.size() << endl;
	cout << "Integer Vector Capacity: " << i.getCapacity() << " numElements: " << i.size() << endl;
	cout << "String Vecotr is filled with spaces and Integer vector is filled with 0's" << endl;

	cout << endl;

	cout << "Creating a copy if the integer vector using =" << endl;
	myGenericVec<int>i2;
	i2 = i;
	cout << "Integer Vector: " << i << endl;
	cout << "Integer Vector Copy: " << i2 << endl;

	cout << endl;

	cout << "Clearing original Integer vector" << endl;
	i.clear();
	cout << "Original Integer Vector Capacity: " << i.getCapacity() << " numElements: " << i.size() << endl;
	cout << "Chencking if the orignal Integer vector is empty" << endl;
	if (i.empty()) { cout << "Original Integer vector is empty" << endl; }

	cout << endl;



	return 0;
}