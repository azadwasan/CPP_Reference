#pragma once

//*******************************************************************************************************************************************************
//****************************************************** r-value and ref-qualifiers ************************************************
//https://codexpert.ro/blog/2014/10/17/c-gems-ref-qualifiers/

struct foo
{
	foo(int const value = 0) :value(value)
	{}

	foo& operator=(foo const & other)
	{
		this->value = other.value;
		return *this;
	}

	operator int() const { return value; }

private:
	int value;
};

foo get_foo()
{
	return foo(13);
}

//Now you can write

void runnerget_foo() {
	foo() = 42;

	if (get_foo() = 42)
	{
	}
}
//Probably the first example is a little bit silly, you don’t do that kind of mistake in real life, but it’s still legal code that executes, 
//and is not right because there’s an rvalue reference on the left side of the assignment operator. The second example is definitely a much realistic example. 
//Sometimes we just type = instead of == in conditional expressions and what the code will do is assigning 42 to temporary, instead of testing their equality.

//If we changed the signature of foo’s operator= to include a ref-qualifier (as shown below) the compiler would flag immediately both examples above as errors:

//	foo& operator=(foo const & other) &
//	{
//		// ...
//	}

