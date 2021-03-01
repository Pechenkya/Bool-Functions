#include <iostream>
#include <array>
#include "Function.h"
/*
	Algebra:
	"0", "1", "!", "OR", "->", "<-", "<->", "AND", "XOR", "NOR", "NAND"
	0 - constant 0
	1 - constant 1
	! - negation
	-> - implication
	<-> - equivalence
	<- - converse implication
	OR - disjunction
	AND - conjuction
	XOR - exclusive OR
	NOR - Peirce arrow (not OR)
	NAND - Sheffer stroke (not AND)

	Input example:
	f(x, y, z) = x XOR (y AND z)

	TODO: Fix negation of par formula like: !(p -> !r) // fixed 01.03.2021
*/

int main()
{
	std::string base_form = "f(p, q, r, s) = !(p -> !r)";
	std::cout << base_form << std::endl;
	Function func(base_form);
	func.print_table();

	////Building FDNF
	//std::string FDNF = func.build_FDNF();
	//std::cout << FDNF << std::endl;
	//Function func2(FDNF);
	//func2.print_table();

	////Building FCNF
	//std::string FCNF = func.build_FCNF();
	//std::cout << FCNF << std::endl;
	//Function func3(FCNF);
	//func3.print_table();

	////Building ANF
	//std::string ANF = func.build_ANF();
	//std::cout << ANF << std::endl;
	//Function func4(ANF);
	//func4.print_table();
	//return 0;
}
