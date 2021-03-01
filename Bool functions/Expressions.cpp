#include "Expressions.h"

//Static variables (Operations)
const char* Expression::operation_tokens[] = {"0", "1", "!", "OR", "->", "<->", "<-", "AND", "XOR", "NOR", "NAND"};
const int Expression::operations_count = 11;
//

//Parsing and building tree
void Expression::parse_expression(const std::string& expr, const std::vector<std::string>& _var_names)
{
	if (root)
		delete root;

	var_names = _var_names;
	//removing spaces
	for (char a : expr)
	{
		if (a != ' ')
			full_expr += a;
	}
	std::string_view prepared = full_expr;


	root = new Node;
	if (check_parentheses(prepared))
		parse_recursive(root, prepared.substr(1, prepared.length() - 2));
	else
		parse_recursive(root, prepared);
}

void Expression::parse_recursive(Node * current, std::string_view left_to_parse)
{
	current->left = nullptr;
	current->right = nullptr;

	size_t str_sz = left_to_parse.length();
	int opened_parentheses = 0;

	for (size_t index = 0; index < str_sz; ++index)
	{
		while (left_to_parse[index] == operation_tokens[2][0])
		{
			index++;
		}

		if (left_to_parse[index] == '(')
		{
			opened_parentheses++;
			index++;
		}


		while (opened_parentheses)
		{
			if (left_to_parse[index] == '(')
				opened_parentheses++;

			if (left_to_parse[index] == ')')
				opened_parentheses--;

			++index;
		}

		while (index < str_sz - 1 && is_binary_operation(&left_to_parse[index]) == 255)
			index++;

		uint8_t op_index = is_binary_operation(&left_to_parse[index % str_sz]); // Can use mod cause index surely must be less then str_sz, if not - it's ! op
		if (op_index != 255)
		{
			uint8_t op_len = strlen(operation_tokens[op_index]);
			current->token = left_to_parse.substr(index, op_len);

			current->left = new Node;
			if (check_parentheses(left_to_parse.substr(0, index)))
				parse_recursive(current->left, left_to_parse.substr(1, index - 2));
			else
				parse_recursive(current->left, left_to_parse.substr(0, index));

			current->right = new Node;
			if(check_parentheses(left_to_parse.substr(index + op_len)))
				parse_recursive(current->right, left_to_parse.substr(index + op_len + 1, str_sz - index - op_len - 2));
			else
				parse_recursive(current->right, left_to_parse.substr(index + op_len, str_sz - index - op_len));

			return;
		}
		else
		{
			if (left_to_parse[0] == operation_tokens[2][0]) // Check for '!'
			{
				current->right = new Node;
				current->token = operation_tokens[2][0];
				current->right->left = nullptr;
				current->right->right = nullptr;
				if (check_parentheses(left_to_parse.substr(1)))
					parse_recursive(current->right, left_to_parse.substr(2, str_sz - 3));
				else
					parse_recursive(current->right, left_to_parse.substr(1, str_sz - 1));
				//current->right->token = left_to_parse.substr(1, left_to_parse.length());
			}
			else
				current->token = left_to_parse.substr(0, left_to_parse.length());
			return;
		}
	}
}

inline uint8_t Expression::is_binary_operation(const char* a)
{
	if ((*a < 65 || *a > 90) && *a != '<' && *a != '-')
		return -1;

	bool found = true;
	for (uint8_t i = 3; i < operations_count; ++i, found = true)
	{
		uint8_t len = strlen(operation_tokens[i]);
		for (uint8_t j = 0; j < len; ++j)
		{
			if (*(a + j) != operation_tokens[i][j])
			{
				found = false;
				break;
			}
		}
		if (found)
			return i;
	}

	return -1;
}

inline bool Expression::check_parentheses(std::string_view str)
{
	int parentheses = 0;
	if (str[0] == '(')
		++parentheses;
	else
		return false;

	size_t len = str.length();
	for (size_t i = 1; i < len - 2; ++i)
	{
		if (str[i] == ')')
			--parentheses;
		else if (str[i] == '(')
			++parentheses;

		if (parentheses == 0)
			return false;
	}

	return true;
}
//


//Calculating expression
bool Expression::calculate_expression(const std::vector<bool>& val_vec)
{
	std::map<std::string, bool> values;
	for (uint8_t i = 0; i < var_names.size(); ++i)
		values[var_names[i]] = val_vec[i];

	return calculate_recursive(root, values);
}

bool Expression::calculate_recursive(Node * current, const std::map<std::string, bool>& values)
{
	uint8_t op = 0;
	for (; op < operations_count; ++op)
	{
		if (current->token == operation_tokens[op])
			break;
	}

	switch (op)
	{
	case 0:
		return 0;
	case 1:
		return 1;
	case 2:
		return !calculate_recursive(current->right, values);
	case 3:
		return calculate_recursive(current->left, values) || calculate_recursive(current->right, values);
	case 4:
	{
		bool a = calculate_recursive(current->left, values);
		bool b = calculate_recursive(current->right, values);
		if (a)
			return b;

		return 1;
	}
	case 5:
		return calculate_recursive(current->left, values) == calculate_recursive(current->right, values);
	case 6:
	{
		bool a = calculate_recursive(current->left, values);
		bool b = calculate_recursive(current->right, values);
		if (b)
			return a;

		return 1;
	}
	case 7:
		return calculate_recursive(current->left, values) && calculate_recursive(current->right, values);
	case 8:
	{
		bool a = calculate_recursive(current->left, values);
		bool b = calculate_recursive(current->right, values);
		return (a && !b) || (b && !a);
	}
	case 9:
		return !(calculate_recursive(current->left, values) || calculate_recursive(current->right, values));
	case 10:
		return !(calculate_recursive(current->left, values) && calculate_recursive(current->right, values));
	default:
		return values.at(current->token);
	}
}
//


//Constructors and destructors
Expression::~Expression()
{
	if (root)
		delete root;
}

Expression::Expression(const std::string & expr, const std::vector<std::string>& _var_names) : var_names {_var_names}
{
	//removing spaces
	for (char a : expr)
	{
		if (a != ' ')
			full_expr += a;
	}
	std::string_view prepared = full_expr;


	root = new Node;
	if (check_parentheses(prepared))
		parse_recursive(root, prepared.substr(1, prepared.length() - 2));
	else
		parse_recursive(root, prepared);
}


//Nodes
Expression::Node::~Node()
{
	if (right)
		delete right;
	if (left)
		delete left;
}


//Getters
std::string Expression::get_name(size_t index)
{
	return var_names[index];
}