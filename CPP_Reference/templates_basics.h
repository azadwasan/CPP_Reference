#pragma once
#include <iostream>
#include <string>
using namespace std;

//https://www.learncpp.com/cpp-tutorial/template-classes/
//There are various ways to include template class in the code base.
//1. Include everything in the header file (easiest and the standard way)
//2. Split the template class in header and code. Move the contents of *.cpp to a new file named *.inl (.inl stands for inline), 
//		and then include *.inl at the bottom of the *.h header (inside the header guard). That yields the same result as putting all the code in the header, but helps keep things a little more organized.
//3. Other solutions involve #including .cpp files, but we don’t recommend these because of the non-standard usage of #include.
//4. Another alternative is to use a three-file approach. Refer the above link for further details.

//*******************************************************************************************************************************************************
//****************************************************** Array size determination and unnamed parameters ************************************************

template <typename T, size_t N>		//this gives us the size of the array
constexpr auto Size(const T(&)[N]) {//This however, is an UNNAMED reference parameter. If we don't need to use the variable, we don't need to name the variable. 
	return N;
}

void runnerSize() {
	char buffer[16];
	for (int i = 0; i < Size(buffer); i++) {
		//do something
	}
}

//*******************************************************************************************************************************************************
//****************************************************** Template non-type parameters ************************************************
//A template non-type parameter is a template parameter where the type of the parameter is predefined and is substituted for a constexpr value passed in as an argument.

template <typename T, int size> // size is an integral non-type parameter
class StaticArray
{
private:
	// The non-type parameter controls the size of the array
	T m_array[size]{};

public:
	T* getArray();

	T& operator[](int index)
	{
		return m_array[index];
	}
};
// Showing how a function for a class with a non-type parameter is defined outside of the class
template <typename T, int size>
T* StaticArray<T, size>::getArray()
{
	return m_array;
}

int runnerStaticArray()
{
	// declare an integer array with room for 12 integers
	StaticArray<int, 12> intArray;

	// Fill it up in order, then print it backwards
	for (int count{ 0 }; count < 12; ++count)
		intArray[count] = count;

	for (int count{ 11 }; count >= 0; --count)
		std::cout << intArray[count] << ' ';
	std::cout << '\n';

	// declare a double buffer with room for 4 doubles
	StaticArray<double, 4> doubleArray;

	for (int count{ 0 }; count < 4; ++count)
		doubleArray[count] = 4.4 + 0.1 * count;

	for (int count{ 0 }; count < 4; ++count)
		std::cout << doubleArray[count] << ' ';

	return 0;
}

//*******************************************************************************************************************************************************
//****************************************************** Stopping implicit conversion ************************************************
template <class T>				//Templates don't allow implicit conversions, hence we can avoid it by defining a template for the function.
T min(T a, T b) {
	return a < b ? a : b;
}

int runnerMin() {
	const int a = 10;
	const short b = 5;
//	min(a, b); //ERROR
	return 0;
}

//*******************************************************************************************************************************************************
//****************************************************** Templates Specialization ************************************************
template <typename T>
bool equal(const T& val1, const T& val2) {
	return val1 == val2;
}

//Template specialization
template <>
bool equal(const double& a, const double& b) {
	return std::abs(a - b) < 0.00001;
}

void runnerEqual() {
	int a = 2;
	int b = 1;
	cout << equal(a, b) << endl;

	double d = 3.0;
	double f = 4.0;
	cout << equal(d, f) << endl;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~ Example 2, copying the actual string in the specialization ~~~~~~~~~~~~~~~~~~~~~~
//https://www.learncpp.com/cpp-tutorial/function-template-specialization/

namespace template_specialization_example_2 {
	template <typename T>
	class Storage
	{
	private:
		T m_value{};
	public:
		Storage(T value)
			: m_value{ value }
		{
			std::cout << "Template constructor running" << std::endl;
		}
		~Storage() {}; // ############## need an explicitly defined destructor to specialize ###################

		void print()
		{
			std::cout << m_value << '\n';
		}
	};

	template <>
	Storage<char*>::Storage(char* value)
	{
		std::cout << "Spcialization constructor running" << std::endl;
		if (!value)
			return;

		// Figure out how long the string in value is
		int length{ 0 };
		while (value[length] != '\0')
			++length;
		++length; // +1 to account for null terminator

		// Allocate memory to hold the value string
		m_value = new char[length];

		// Copy the actual value string into the m_value memory we just allocated
		for (int count = 0; count < length; ++count)
			m_value[count] = value[count];
	}

	template <>
	Storage<char*>::~Storage()	// ########### Destructor specialization ##########
	{
		delete[] m_value;
	}

	int runnerStorage()
	{
		// Dynamically allocate a temporary string
		//std::string s;
		char *s= new char[40];

		// Ask user for their name
		std::cout << "Enter your name: ";
		std::cin >> s;

		// Store the name
		Storage<char*> storage(s);

		storage.print(); // Prints our name

		delete []s; // clear the std::string

		storage.print(); // Prints our name
		return 0;
	}
}
