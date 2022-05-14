#pragma once
#include <string>
#include <sstream>

//In addition to the dynamic decorator, the following code also shows how to handle a situation where we need virtual dispatch but
//we can't make the funciton virtual. Concretely, we have operator<< that we can't make virtual but we would like to behave differently
//for different object. In order to solve this problem we define a virtual printer function which is called by a common operator<< overload

//The dynamic decorators allow us to choose the decorators at run-time.
//However, it also hides the implementation of the original class that is decorated, 
//hence if it offer any additional interface, it is hidden and can no longer be accessed directly.
//This will be solved through static decorators.
namespace dp_decorator_dynamic {
	class Shape {
	public:
		virtual std::string str() const noexcept = 0;
		virtual ostream& printer(ostream& out) = 0;
	};

	std::ostream& operator<<(std::ostream& out, Shape& s) {
		s.printer(out);
		return out;
	}

	class Circle : public Shape {
		float radius;
	public:
		Circle(float r) :radius(r) {}

		std::string str() const noexcept override {
			std::ostringstream oss;
			oss << "The radius of the circle is " << radius << std::endl;
			return oss.str();
		}

		ostream& printer(ostream& out) override {
			out << "The radius of the circle is " << radius << std::endl;
			return out;
		}
	};

	class Square : public Shape {
		float side;
	public:
		Square(float s) :side(s) {}

		std::string str() const noexcept override {
			std::ostringstream oss;
			oss << "The length of the square side is " << side << std::endl;
			return oss.str();
		}

		ostream& printer(ostream& out) override {
			out << "The length of the square side is " << side << std::endl;
			return out;
		}
	};

	//Now, we are going to extend the Circle and Square classes with the decorators.
	class ColorDec: public Shape {
	public:
		enum class Color:std::int16_t {
			Red,
			Green,
			Blue,
			White,
			Black,
			Brown
		};
		friend std::ostream& operator<<(std::ostream& os, Color c) {
			switch (c) {
			case Color::Red:		os << "Red" << endl;	break;
			case Color::Green:	os << "Green" << endl;	break;
			case Color::Blue:		os << "Blue" << endl;	break;
			case Color::White:	os << "White" << endl;	break;
			case Color::Black:	os << "Black" << endl;	break;
			case Color::Brown:	os << "Brown" << endl;	break;
			}
			return os;
		}
	public:
		ColorDec(Shape& s, enum Color c) : shp(s), shp_color{ c } {}
//		ColorDec(Shape&& s, enum Color c) : shp(s), shp_color{ c } {}
		std::string str() const noexcept override {
			std::ostringstream oss;
			//oss << shp.str()
			//	<< "and has color " << static_cast<std::underlying_type<Color>::type>(shp_color) << std::endl;

			oss << shp.str()
				<< "and has color " << shp_color << std::endl;
			return oss.str();
		}

		std::ostream& printer(std::ostream& out) {
			out << shp.str()
				<< "and has color " << shp_color << std::endl;
			return out;
		}

	private:
		Shape& shp;
		Color shp_color;
	};

	class TransDec: public Shape {
		float transparancy;
		Shape& shp;
	public:
		TransDec(Shape& s, float t) : shp{ s }, transparancy(t){}
//		TransDec(Shape&& s, float t) : shp{ s }, transparancy(t){}

		std::ostream& printer(std::ostream& out) {
			out << shp
				<< "The transprancy level is " << transparancy << std::endl;
			return out;
		}

		std::string str() const noexcept override {
			std::ostringstream oss;
			oss << shp
				<< "The transprancy level is " << transparancy << std::endl;
			return oss.str();
		}
	};

	void main() {
		Circle c{ 5 };
		Square s{ 10 };
		ColorDec red_circle{ c, ColorDec::Color::Red };
//		std::cout << red_circle.str() <<std::endl;
		TransDec trans_square{ s, 12 };
//		std::cout << trans_square.str() << std::endl;

		TransDec trans_red_circle{ red_circle, 111 };
		
		std::cout << trans_red_circle.str() << std::endl;
//		TransDec{ ColorDec{c, ColorDec::Color::Green}, 111 };
	//	ColorDec

		//std::cout << c.str();
		//std::cout << s.str();
	}
}
