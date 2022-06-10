#pragma once
#include <vector>
#include <iostream>

//One of the most frequent uses for CRTP is Compile Time Polymorphism.
//The speciality of CRTP is that the derived calss derives from the base class but passes itself as the template argument.

namespace temp_crtp_1 {
	template <typename T>
	class Base {
	public:
		void fn() {
			static_cast<T*>(this)->GetState();
		}
	};

	class State0 : public Base<State0> {
	public:
		void GetState() { std::cout << "State0::getState Called.." << std::endl; }
	};

	class State1 : public Base<State1> {
	public:
		void GetState() { std::cout << "State1::getState Called..." << std::endl; }
	};

	template <typename T>
	void callFn(Base<T> bp) {
		bp.fn();
	}

	void main() {
		State0 s1;
		State1 s2;

		callFn(s1);
		callFn(s2);
	}
}




// https://www.youtube.com/watch?v=7-nHdQjSRe0
namespace temp_crtp {
	struct Shape {
		virtual Shape* Clone() = 0;
		virtual ~Shape() = default;
	};

	template <typename T>
	struct ShapeCRTP : public Shape {
		virtual Shape* Clone() override { return new T(*static_cast<T*>(this)); }
	};

	struct Square : public ShapeCRTP<Square>
	{
		int x = 1;
	};

	struct Rectangel : public ShapeCRTP<Rectangel>
	{
		int x = 1, y = 2;
	};

	void main()
	{
		std::vector<Shape*> v;
		v.push_back(new Square);
		v.push_back(new Rectangel);
		for (const auto& s : v) {
			Shape* c = s->Clone(); //We are leaking memory here!!!

		}
	}
}
