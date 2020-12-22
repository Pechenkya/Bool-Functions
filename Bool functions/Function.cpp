#include "Function.h"
#include <limits>
#include <iostream>

Function::Function(std::string to_build)
{
	num_of_variables = 0;
	std::stringstream temp;
	for (size_t i = 0, variables = 0; to_build[i] != '='; ++i)
	{
		char ch = to_build[i];
		function_name += ch;

		if (ch == '(')
			variables = 1;
		else if (ch == ')')
			variables = 0;
		else if (variables && ch != ',')
		{
			temp << ch;
		}
		else
		{
			temp << ' ';
		}
	}

	std::vector<std::string> names;

	while (!temp.eof())
	{
		std::string str;
		temp >> str;
		++num_of_variables;
		names.push_back(str);
	}

	if (names[names.size() - 1] == "")
	{
		--num_of_variables;
		names.pop_back();
	}
	names.shrink_to_fit();
	
	basic_expression = new Expression(to_build.substr(to_build.find('=') + 1), names);
}

Function::~Function()
{
	delete basic_expression;
}

void Function::print_table()
{
	std::vector<bool> var_value_vector(num_of_variables);
	size_t table_h = 1u << num_of_variables;
	for (size_t i = 0; i < table_h; ++i)
	{
		for (size_t j = 0; j < num_of_variables; ++j)
		{
			var_value_vector[j] = i & (1u << num_of_variables - 1 - j);
			std::cout << var_value_vector[j] << ' ';
		}
		std::cout << "| " << basic_expression->calculate_expression(var_value_vector) << std::endl;
	}
}

std::string Function::build_FDNF()
{
	std::string formula;
	std::vector<bool> var_value_vector(num_of_variables);

	bool f1 = true;
	size_t table_h = 1u << num_of_variables;
	for (size_t i = 0; i < table_h; ++i)
	{
		for (size_t j = 0; j < num_of_variables; ++j)
			var_value_vector[j] = i & (1u << num_of_variables - 1 - j);

		if (basic_expression->calculate_expression(var_value_vector))
		{
			if (f1)
				f1 = false;
			else
				formula += " OR ";

			formula += '(';
			bool f2 = true;
			for (size_t j = 0; j < num_of_variables; ++j)
			{
				if (f2)
					f2 = false;
				else
					formula += " AND ";
				if(var_value_vector[j])
					formula += basic_expression->get_name(j);
				else
					formula += '!' + basic_expression->get_name(j);
			}
			formula += ')';
		}
	}

	if (formula.empty())
		formula = "0";

	formula = function_name + "= " + formula;

	return formula;
}

std::string Function::build_FCNF()
{
	std::string formula;
	std::vector<bool> var_value_vector(num_of_variables);

	bool f1 = true;
	size_t table_h = 1u << num_of_variables;
	for (size_t i = 0; i < table_h; ++i)
	{
		for (size_t j = 0; j < num_of_variables; ++j)
			var_value_vector[j] = i & (1u << num_of_variables - 1 - j);

		if (!basic_expression->calculate_expression(var_value_vector))
		{
			if (f1)
				f1 = false;
			else
				formula += " AND ";

			formula += '(';
			bool f2 = true;
			for (size_t j = 0; j < num_of_variables; ++j)
			{
				if (f2)
					f2 = false;
				else
					formula += " OR ";
				if (var_value_vector[j])
					formula += '!' + basic_expression->get_name(j);
				else
					formula += basic_expression->get_name(j);
			}
			formula += ')';
		}
	}

	if (formula.empty())
		formula = "1";

	formula = function_name + "= " + formula;

	return formula;
}

std::string Function::build_ANF()
{
	std::string formula;
	std::vector<bool> calc_vec((size_t)std::pow(2, (num_of_variables)));

	{
		std::vector<bool> var_value_vector(num_of_variables);

		size_t table_h = 1u << num_of_variables;
		for (size_t i = 0; i < table_h; ++i)
		{
			for (size_t j = 0; j < num_of_variables; ++j)
			{
				var_value_vector[j] = i & (1u << num_of_variables - 1 - j);
			}
			calc_vec[i] = basic_expression->calculate_expression(var_value_vector);
		}
	}
	
	ANF_recursive(calc_vec, 0, calc_vec.size() - 1);

	bool f1 = true;

	if (calc_vec[0] == 1)
	{
		formula += '1';
		f1 = false;
	}

	for (size_t i = 1; i < calc_vec.size(); ++i)
	{
		if (calc_vec[i] == 1)
		{
			if (f1)
				f1 = false;
			else
				formula += " XOR ";

			formula += '(';
			bool f2 = true;
			for (size_t j = 0; j < num_of_variables; ++j)
			{
				if (i & (size_t)std::pow(2, (num_of_variables - 1 - j)))
				{
					if (f2)
						f2 = false;
					else
						formula += " AND ";
					formula += basic_expression->get_name(j);
				}

			}
			formula += ')';
		}
	}

	if (formula.empty())
		formula = "0";

	formula = function_name + "= " + formula;

	return formula;
}

void Function::ANF_recursive(std::vector<bool>& calc_vec, size_t begin, size_t end)
{
	if (begin - end < 1)
		return;

	for (size_t i = begin, j = (end + begin) / 2 + 1; i <= (begin + end) / 2; ++i, ++j)
	{
		calc_vec[j] = (calc_vec[i] && !calc_vec[j]) || (!calc_vec[i] && calc_vec[j]);
	}

	ANF_recursive(calc_vec, begin, (end + begin) / 2);
	ANF_recursive(calc_vec, (end + begin) / 2 + 1, end);
}
