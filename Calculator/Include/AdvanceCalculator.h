#pragma once
#include "..\Include\BasicCalculator.h"
#include <cmath>

template <class T>
class AdvanceCalculator : public BasicCalculator<T>
{
public:
	AdvanceCalculator();
	~AdvanceCalculator();

	/**
	 * @brief Solve an expression string supporting sqrt and power operations
	 * @param expression Expression to evaluate
	 * @return Result of type T
	 */
	T solve(const std::string& expression) override;

	/**
	* @brief Print the contents of a file and solve each expression in it
	* @param path The file path to read, solve and print
	*/
	void printAndSolveFile(const std::string& path) override;

	/**
	* @brief solve each expression in a file and append the results to another file
	* @param path The file path to read and solve
	* @param outputPath The file path to append the results to
	*/
	void solveFileAndAppend(const std::string& inputPath, const std::string& outputPath) override;

protected:
	/**
	 * @brief Convert a substring of the expression to a value of type T
	 * @param exp The full expression string
	 * @param pos Current index in string (updated after parsing)
	 * @return The converted value of type T
	 */
	T strToValue(const std::string& exp, int& pos) override;
	/**
	 * @brief Apply an operator on two values, including sqrt and power
	 * @param values The operands for the operation
	 * @param op The operator character (e.g., '+', '-', '*', '/', '#' for sqrt, '^' for power)
	 * @return The result of applying the operator to the values
	 */
	T applyOp(std::vector<T>& values, char op) override;
	T sqrtOp(T a);
	T powOp(T a, T b);
	using BasicCalculator<T>::isLegalOperaotor;
	using BasicCalculator<T>::addLegalOperator;
};

