#pragma once
// ****************************************************************************************************************
// ************************************************ Ref-qualifiers ************************************************
//Problem: We would like to know if the object on which the member function is being executed is l-value or r-value, so that we may take appropriate action accordingly.
// Solution: Qualify the functions with ref-qualifiers. Add "&" or "&&" before the body of the function to denote if it is to be executed for a l-value or r-value object.
// Here is a simple illustration from https://en.cppreference.com/w/cpp/utility/forward
struct Arg
{
	int i = 1;
	int  get() && { return i; } // call to this overload is rvalue
	int& get() & { return i; } // call to this overload is lvalue
};

//Here is an elaborate explanation from //https://akrzemi1.wordpress.com/2014/06/02/ref-qualifiers/
//In the following "if" condition the object "v" is move constructed.
template <typename T>
void someFunc() {
	if (optional<T> v = read()) // has value?
		use(*v);                  // get the value
}
//However, std::optional also allows various other possibilities like

//T t = read().value();     // (1)
//T u = read().value_or(0); // (2)
//T v = *read();            // (3)

//In order to enable such assignment we must know whether the object is l-value or r-value so that we can either copy it or move it.
// Hence, we can use the ref-qualifiers for the function to determine this and accordingly return the corresponding values.
//Given such symantics, it can easily be achieved as follows:
template <typename T>
class optional
{
	// ...
	T&       value() &;
	T&&      value() && ;
	T const& value() const&;
};

template <typename T>
optional<T> read() {
} // may return T or not-a-T

//Actual background: Like in the case of CV qualifiers, const and volatile are the qualifiers for the implicit "this"
//pointer passed to the function and depending on the type of the pointer the corresponding function (const or non-const) is called.
//Similarly, ref-qualifier is the qualifier for the "*this" implicit object passed to the function and then the complier
//checks if it was a l-value or an r-value and accordingly call the corresponding function (l-value or r-value ref-qualified).
//Here are some really nice explanations:https://riptutorial.com/cplusplus/example/24493/this-pointer-ref-qualifiers
//	"Similarly to this cv-qualifiers, we can also apply ref-qualifiers to *this. Ref-qualifiers are used to choose between normal and rvalue reference semantics, allowing the compiler to use either copy or move semantics depending on which are more appropriate, and are applied to *this instead of this."
//Also an other very nicely written articles: https://codexpert.ro/blog/2014/10/17/c-gems-ref-qualifiers/
//NOTE: Do check the    foo& operator=(foo const & other) example at the above link, as it shows that one can do "foo() = 42;" which shouldn't be allowed because its a r-value. Check article for futher details.

//There is one last catch, if a function is ref-qualified, then all the overloads also need to be ref-qualified, otherwise there could be ambiguit in resolution by the compiler.
//The above links discuss this issue and this thread explicitly discusses it: https://stackoverflow.com/questions/35389911/why-is-overloading-on-just-one-ref-qualifier-not-allowed

// ****************************************************************************************************************


