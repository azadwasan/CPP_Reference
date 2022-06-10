#pragma once
#include <iostream>

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
	std::cout << equal(a, b) << std::endl;

	double d = 3.0;
	double f = 4.0;
	std::cout << equal(d, f) << std::endl;
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
		char *s = new char[40];

		// Ask user for their name
		std::cout << "Enter your name: ";
		std::cin >> s;

		// Store the name
		Storage<char*> storage(s);

		storage.print(); // Prints our name

		delete[]s; // clear the std::string

		storage.print(); // Prints our name
		return 0;
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~ Example 3, Template Specilization to take a function as parameter (Taken from function decorator) ~~~~~~~~~~~~~~~~~~~~~~

double add(double a, double b) {
	std::cout << a << "+" << b << " = " << (a + b) << std::endl;
	return a + b;
}

//When we see below is a trick to pass generic functions to a class template.
//It uses partial template specialization. Here is a detailed explanation of what/how it is achieved
// https://stackoverflow.com/questions/34033586/what-does-this-syntax-mean-class-template-class-r-class-args-class-name


//As we want to have a template class that accepts a function that can return a value and accept any number of arguments
//Hence, the trick to achieve it is to define a generic template first, as done in the following line.
template <typename> class Logger3;

//Then we define a template partial specialization such that it accepts a function that returns R and accepts Args...
template <typename R, typename... Args>
class Logger3<R(Args...)> {
	std::function<R(Args ...)> func;
	std::string name;
public:
	Logger3(std::function<R(Args...)> f, const std::string& name_) : func{ f }, name{ name_ }{}

	R operator() (Args ...args) const {
		std::cout << "Starting execution " << name << std::endl;
		R result = func(args...);
		std::cout << "Execution finished " << name << std::endl;
		return result;
	}
};

template <typename R, typename... Args>
auto make_logger3(R(*func)(Args...), const std::string& name) {
	return Logger3<R(Args...)>{ std::function<R(Args...)>{func}, name };
}
