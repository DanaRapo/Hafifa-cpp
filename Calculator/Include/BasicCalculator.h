#pragma once
#include <vector>       
#include <string>       
#include <complex>      
#include <cctype>
#include <iostream>
#include <limits>
#include <map>
#include <functional>

template <class T>
class BasicCalculator
{
public:
	BasicCalculator();
	~BasicCalculator();

	/**
	 * @brief Solve an expression string
	 * @param expression Expression to evaluate
	 * @return Result of type T
	 */
	virtual T solve(const std::string& expression);

protected:
	/**
	 * @brief Convert a substring of the expression to a value of type T
	 * @param exp The full expression string
	 * @param pos Current index in string (updated after parsing)
	 * @return The converted value of type T
	 */
	virtual T strToValue(const std::string& exp, int& pos);

	/**
	 * @brief Apply an operator on two values
	 * @param values The operands for the operation
	 * @param op The operator character (e.g., '+', '-', '*', '/')
	 * @return The result of applying the operator to the values
	 */
	virtual T applyOp(std::vector<T>& values, char op);

	bool isLegalOperaotor(char op) const;
	void addLegalOperator(char op);
	T add(T& a, T& b);
	T sub(T& a, T& b);
	T mul(T& a, T& b);
	T div(T& a, T& b);

private:
	std::vector<char> legalOperators_;
}; 