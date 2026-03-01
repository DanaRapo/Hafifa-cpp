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
	T sqrtOp(T& a);
	T powOp(T& a, T& b);
	using BasicCalculator<T>::isLegalOperaotor;
	using BasicCalculator<T>::addLegalOperator;
};

