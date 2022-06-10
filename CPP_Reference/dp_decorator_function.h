#pragma once
#include <functional>
#include <string>
#include <iostream>

namespace dp_decorator_function {
	/********************** Start *************************/
	//This is a just a simple function decorator that adds functionality to a certain function.
	//The disadvantage is that the function is required to be void(void). We will relax this limitation later.
	class Logger {
		std::function<void(void)> func;
		std::string name;

	public:
		Logger(const std::function<void(void)>& f, const std::string& name_) : func{ f }, name{ name_ }{}

		void operator() () const {
			std::cout << "Starting execution " << std::endl;
			func();
			std::cout << "Execution finished " << std::endl;
		}
	};
	/********************** End *************************/


	/********************** Start *************************/
	//In this version, we just templatize the class but don't make any other improvement.
	//We still haven't specified any function signature with return types and arguments
	//Check comments in main(), where we discuss that we can't instantiate this class without specifying the template parameters explicitly.
	//Hence, we provide a helper function (as the function has type deduction in case of templates).
	//However, this limitation is lifted if we use C++17 and we don't need a helper function as C++ 17 has template class type deduction.

	template <typename T>
	class Logger2 {
		T func;
		std::string name;
	public:
		Logger2(T f, const std::string& name_) : func{ f }, name{ name_ }{}

		void operator() () const {
			std::cout << "Starting execution " << name << std::endl;
			func();
			std::cout << "Execution finished " << name << std::endl;
		}
	};

	template <typename Func>
	auto make_logger2(Func func, const std::string& s) {
		//return Logger2<decltype(func)>{func,s};
		return Logger2<Func>{func, s};
	}

	/********************** End *************************/

	/********************** Start *************************/
	//This is the third iteration, where we can pass generic functions to the template class.
	//However, it is a bit complicated. Read the comments for the details.
	//We simplify this in the next iterations when we use C++ 17 features.

	double add(double a, double b) {
		std::cout << a << "+" << b << " = " << (a + b) << std::endl;
		return a + b;
	}

	//As we want to have a template class that accepts a function that can return a value and accept any number of arguments
	//Hence, the trick to achieve it is to define a generic template first, as done in the following line.
	template <typename> class Logger3;

	//Then we define a template partial specialization such that it accepts a function that returns R and accepts Args...
	//Here is a detailed explanation https://stackoverflow.com/questions/34033586/what-does-this-syntax-mean-class-template-class-r-class-args-class-name
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

	/********************** End *************************/

	/********************** Start *************************/
	//Next iteration, where we remove the partial template specialization.
	template <typename R, typename... Args>
	class Logger4 {
		std::function<R(Args ...)> func;
		std::string name;
	public:
		Logger4(std::function<R(Args...)> f, const std::string& name_) : func{ f }, name{ name_ }{}

		R operator() (Args ...args) const {
			std::cout << "Starting execution " << name << std::endl;
			R result = func(args...);
			std::cout << "Execution finished " << name << std::endl;
			return result;
		}
	};

	template <typename R, typename... Args>
	auto make_logger4(R(*func)(Args...), const std::string& name) {
		return Logger4<R, Args...>(std::function<R(Args...)>{func}, name);
	}

	/********************** End *************************/
	/********************** Start *************************/
	//Next iteration, for this we would need C++ 17, as we are not going to provide any helper function.
	//We will use type deduction feature of C++ 17.

	template <typename R, typename... Args>
	class Logger5 {
		std::function<R(Args ...)> func;
		std::string name;
	public:
		Logger5(R(*f)(Args...), const std::string& name_) : func{ std::function<R(Args...)>{ f } }, name{ name_ }{}

		R operator() (Args ...args) const {
			std::cout << "Starting execution " << name << std::endl;
			R result = func(args...);
			std::cout << "Execution finished " << name << std::endl;
			return result;
		}
	};

	/********************** End *************************/

	void main() {
		//Logger{ []() {std::cout << "I am an empty lambda" << std::endl; }, "Some random function" }();

		//C++ 17 the following works because of Class Template Argument Deduction https://en.cppreference.com/w/cpp/language/class_template_argument_deduction
		//Logger2{ []() {std::cout << "I am an empty lambda 2" << std::endl; }, "Some random function" }();

		//C++ 14 requires either of the following two
		//Logger2<std::function<void(void)>> { []() {std::cout << "I am an empty lambda 2" << std::endl; }, "Some random function" }();

		//The following is allowed from C++ 11 onward, as function template argument deduction is allowed
		//https://www.ibm.com/docs/en/zos/2.2.0?topic=only-template-argument-deduction-c
		auto log = make_logger2([]() {std::cout << "Hello " << std::endl; }, "HelloFunction");
		//log();

//		auto log2 = Logger3{ std::function<double(double,double)>{add}, "Logger 3 testing function " };
//		log2(1.1, 2.2);

		std::cout << std::endl << "******************************" << std::endl << std::endl;

		auto log3 = make_logger3(add, "Logger 3 test function 2");
		log3(3.3, 4.4);

		std::cout << std::endl << "******************************" << std::endl << std::endl;

		auto log4 = make_logger4(add, "Logger 4 test function 2");
		log4(5.5, 6.6);

		std::cout << std::endl << "******************************" << std::endl << std::endl;

		Logger5{ add, "Logger 5 test function " }(7.7, 8.8);
	}
}