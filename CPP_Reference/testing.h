#pragma once
#include <string>
#include <sstream>
#include <utility>
#include <iostream>
#include <functional>
#include <type_traits>

namespace temp_testing {

	class Shape {
	public:
		virtual std::string str() const noexcept = 0;
	};

	class Circle : public Shape {
	public:
		float radius;
		Circle(float r) :radius{ r } {}
		std::string str() const noexcept override {
			std::ostringstream oss;
			oss << "The Circle has radius of " << radius << std::endl;
			return oss.str();
		}

		void resize(float scale) { radius *= scale; }
	};

	class Square : public Shape {
	public:
		float side;
		Square(float s) :side{ s } {}
		std::string str() const noexcept override {
			std::ostringstream oss;
			oss << "The square has a side length " << side << std::endl;
			return oss.str();
		}
	};

	class TransDec : public Shape {
		float transparancy;
		Shape& shp;
	public:
		TransDec(Shape& s, float t) : shp{ s }, transparancy(t){}

		std::string str() const noexcept override {
			std::ostringstream oss;
			oss << shp.str()
				<< "The transprancy level is " << transparancy << std::endl;
			return oss.str();
		}
	};

	class ColorDec : public Shape {
	public:
		ColorDec(Shape& s, const std::string& c) : shp(s), shp_color{ c } {}
		std::string str() const noexcept override {
			std::ostringstream oss;
			oss << shp.str()
				<< "and has color " << shp_color << std::endl;
			return oss.str();
		}

	private:
		Shape& shp;
		std::string shp_color;
	};

	template <typename T>
	class ColorDecorator : public T {
	public:
		std::string color;
		template <typename... Args>
		ColorDecorator(std::string c, Args ...args) :
			color{ c }, T{ std::forward<Args>(args)... }{}

		std::string str() const noexcept override{
			std::ostringstream oss;
			oss << T::str() << "The Color is " << color << std::endl;
			return oss.str();
		}
	};

	template <typename T>
	class TransparancyDecorator : public T {
	public:
		float transparancy;
		template <typename... Args>
		TransparancyDecorator(float t, Args... args):
			transparancy{ t }, T{ std::forward<Args>(args)...}{}
		std::string str() const noexcept override {
			std::ostringstream oss;
			oss << T::str() << "Transparancy is " << transparancy << std::endl;
			return oss.str();
		}
	};

	void main() {
		std::cout << ColorDecorator<TransparancyDecorator<Square>> {"Red", 111, 1.2}.str() << std::endl;
		Square a1{ 1.2 };
		TransparancyDecorator<Square> a2{ 111, 1.2 };
		ColorDecorator<TransparancyDecorator<ColorDecorator<Square>>> a3{ "Red", 111, "Blue", 1.2 };

		Circle c{ 5 };
		ColorDec red_circle{ c, "Red" };
		TransDec trans_red_circle{ red_circle, 111 };
		std::cout << std::endl;
	}
}

namespace temp_testing2 {
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
	class Logger3<R(Args...)>{
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
	class Logger4{
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
		return Logger4<R, Args...>(std::function<R(Args...)>{func}, name );
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