#pragma once
#include "Expressions.h"

class Function
{
private: 
	Expression* basic_expression;
	std::string function_name;

	size_t num_of_variables;
public:
	Function(std::string to_build);
	~Function();

	void print_table();
	//Normal forms of function
	std::string build_FDNF(); // Build full disjunctive normal form
	std::string build_FCNF(); // Build full conjunctive normal form
	std::string build_ANF(); // Build algebraic normal form
private:
	void ANF_recursive(std::vector<bool>& value_vec, size_t begin, size_t end);
};

