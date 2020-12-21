#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string_view>

class Expression
{
private:
	//Expresion tree structure
	struct Node
	{
		Node* left;
		Node* right;
		std::string token;
		~Node();
	};

	Node* root;
	//

	//Variables and expression
	std::vector<std::string> var_names;
	std::string full_expr;

	//Operations
	static const char* operation_tokens[];
	static int operations_count;
public:
	//Constructors and Destructors
	~Expression();
	Expression(const std::string& expr, const std::vector<std::string>& _var_names);
	//

	//Interface functions
	void parse_expression(const std::string& expr, const std::vector<std::string>& _var_names);
	bool calculate_expression(const std::vector<bool>& values);
	//

	//Getters
	std::string get_name(size_t index);
	//
private:


	//Recursive methods
	void parse_recursive(Node* current, std::string_view left_to_parse);
	bool calculate_recursive(Node * current, const std::map<std::string, bool>& values);
	//

	//Helping functions
	uint8_t is_binary_operation(const char* a);
	bool check_parentheses(std::string_view str);
};