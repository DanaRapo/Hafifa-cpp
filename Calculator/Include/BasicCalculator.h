#pragma once
#include <vector>       
#include <string>       
#include <complex>      
#include <cctype>
#include <iostream>
#include <limits>
#include <map>
#include <functional>
#include <fstream>
#include <regex>
#include <stdexcept>

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

	/**
	* @brief Print the contents of a file
	* @param path The file path to read and print
	*/
	void printFileContent(const std::string& path) const;

	/**
	* @brief Print the contents of a file and solve each expression in it
	* @param path The file path to read, solve and print
	*/
	virtual void printAndSolveFile(const std::string& path);

	/**
	* @brief solve each expression in a file and append the results to another file
	* @param path The file path to read and solve 
	* @param outputPath The file path to append the results to
	*/
	virtual void solveFileAndAppend(const std::string& inputPath, const std::string& outputPath);

protected:
	/**
	* @brief Print or write to a file the contents of an input file and solve each expression in it
	* @param path The file path to read, solve and print
	* @param output The output stream to write the results to (e.g., std::cout or an ofstream)
	*/
	virtual void fileHendeler(const std::string& path, std::ostream& output);

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
	T add(T a, T b);
	T sub(T a, T b);
	T mul(T a, T b);
	T div(T a, T b);

private:
	std::vector<char> legalOperators_;
}; 