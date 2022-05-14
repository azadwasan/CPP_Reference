#pragma once
//Static decorators are implemented using "Curiously Recurring Template Pattern (CRTP)".
//CRTP is also a mechanism to implement mixins in C++.
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <type_traits>

namespace db_decorator_static {
	class Shape {
	public:
		virtual std::string str() const noexcept = 0;
	};

	class Square : public Shape {
		float side;
	public:
		Square(float s) :side{ s } {}

		std::string str() const noexcept override {
			std::ostringstream oss;
			oss << "The square side is " << side << std::endl;
			return oss.str();
		}
	};

	class Circle : public Shape {
		float radius;
	public:
		Circle(float r) : radius{ r } {}

		std::string str() const noexcept override {
			std::ostringstream oss;
			oss << "The circle radius is " << radius << std::endl;
			return oss.str();
		}
	};

	template <typename> struct IsBase : std::false_type {};

	// We want to be able to do something as follows:
	//TransparentShape<ColoredShape<Square>> sq{10,			"red",		44};
	//											Transparancy, color, size of square

	template <typename T>
	class ColoredShape :T {	//Mixin inheritance
		static_assert(is_base_of<Shape, T>::value, "Template argument must be a shape");//Ensure its Shape type.
		std::string color;
		ColoredShape() {}

	public:
		template <typename... Args>
		ColoredShape(const std::string& color, Args ...args)
			: T(std::forward<Args>(args)...), color{ color } {}

		std::string str() const noexcept override {
			std::ostringstream oss;
			oss << T::str() << ", the color is " << color << std::endl;
			return oss.str();
		}
	};

	template <typename T,
		typename std::enable_if<(std::is_base_of<Shape, T>::value)>::type* = nullptr>
		//We can also restrict the class to only Shape derivatives using enable_if instead of static_assert (though the letter is simpler and I prefer it, hence this is just for reference).
		//https://stackoverflow.com/questions/57345481/how-can-a-template-type-be-restricted-to-a-base-class-excluding-a-subclass-of-th
	
	class TransparentShape :T {	//Mixin inheritance
		//static_assert(is_base_of<Shape, T>::value, "Template argument must be a shape");//Ensure its Shape type.
		uint8_t transparency;

	public:
		TransparentShape() {}
		template <typename... Args>
		TransparentShape(const uint8_t t, Args ...args)
			: T(std::forward<Args>(args)...), transparency{ t } {}

		std::string str() const noexcept override {
			std::ostringstream oss;
			oss << T::str() << " has " << static_cast<unsigned>(transparency) << " transparency." << std::endl;
			return oss.str();
		}
	};
	template <typename T>
	class MyClass {

	};

	void main() {
		ColoredShape<Circle> green_circle{ "green", 5 };
		std::cout << green_circle.str() << std::endl;
	
		TransparentShape<Circle> transparent_circle{ 123, 555 };
		std::cout << transparent_circle.str() << std::endl;

		std::cout << ColoredShape<TransparentShape<Circle>> {"Red", 111, 12}.str() << std::endl;
	}
}