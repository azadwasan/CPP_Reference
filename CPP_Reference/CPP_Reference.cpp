// CPP_Reference.cpp : This file contains the 'main' function. Program execution begins and ends there.
// This is intended to be a the most comprehensive CPP reference. I will be adding everything related to CPP in this project.



#include <iostream>
#include "templates_basics.h"
#include "dp_decorator_dynamic.h"
#include "dp_decorator_static.h"
#include "CRTP.h"
#include "testing.h"
#include "dp_SOLID_OCP.h"
#include "versions_cpp_20.h"
#include "ds_linked_list.h"
#include "217_Contains_Duplicate.h"
#include "219_Contains_Duplicate_II.h"

int main()
{
//	dp_decorator_dynamic::main();
//	db_decorator_static::main();
//	temp_crtp_1::main();
//	temp_crtp::main();
//	temp_testing::main();
//	temp_testing2::main();

//	dp_SOLID_Specification::main();

//	std::vector<int> vals{ 1,2,3,4,5 };
	//std::vector<ds_linked_list::MyType> vals{ {"Str1", 1}, {"Str2", 2}, {"Str3", 3}, {"Str4", 4}, {"Str5", 5} };
	//auto ll = ds_linked_list::create_linked_list(vals);
	//ds_linked_list::display_linked_list(ll);

	//constexpr int vals2 [] { 5, 10, 15, 20, 25};
	//auto ll2 = ds_linked_list::create_linked_list(vals2);
	//ds_linked_list::display_linked_list(ll2);

	contains_duplicate_II::main();

	return 0;
} 