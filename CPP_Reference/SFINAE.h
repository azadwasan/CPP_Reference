#pragma once

//************************* Using enable_if to conditionally remove functions and class
//https://stackoverflow.com/questions/14600201/why-should-i-avoid-stdenable-if-in-function-signatures?rq=1
//std::enable_if can be used as a function argument, as a return type or as a class template or function template parameter to conditionally remove functions or classes from overload resolution.

//As function parameter:
template<typename T>
struct Check1
{
	template<typename U = T>
	U read(typename std::enable_if<
		std::is_same<U, int>::value >::type* = 0) {
		return 42;
	}

	template<typename U = T>
	U read(typename std::enable_if<
		std::is_same<U, double>::value >::type* = 0) {
		return 3.14;
	}
};

//As template parameter:

template<typename T>
struct Check2
{
	template<typename U = T, typename std::enable_if<
		std::is_same<U, int>::value, int>::type = 0>
		U read() { return 42; }

	template<typename U = T, typename std::enable_if<
		std::is_same<U, double>::value, int>::type = 0>
		U read() { return 3.14; }
};

//As return type:

template<typename T>
struct Check3
{
	template<typename U = T>
	typename std::enable_if<std::is_same<U, int>::value, U>::type read() {
		return 42;
	}

	template<typename U = T>
	typename std::enable_if<std::is_same<U, double>::value, U>::type read() {
		return 3.14;
	}
};
