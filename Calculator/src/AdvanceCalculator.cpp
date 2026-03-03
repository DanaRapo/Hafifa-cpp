#include "..\Include\AdvanceCalculator.h"

template <class T>
AdvanceCalculator<T>::AdvanceCalculator()
{
	// Add advanced operators
	this->addLegalOperator('#'); // sqrt operator
	this->addLegalOperator('^'); // power operator
}

template <class T>
AdvanceCalculator<T>::~AdvanceCalculator() = default;

template <class T>
T AdvanceCalculator<T>::sqrtOp(T a) {
	return static_cast<T>(std::sqrt(a));
}

template <class T>
T AdvanceCalculator<T>::powOp(T a, T b) {
	return static_cast<T>(std::pow(a, b));
}

template <class T>
T AdvanceCalculator<T>::applyOp(std::vector<T>& values, char op)
{
	if (!isLegalOperaotor(op)) {
		throw std::runtime_error(std::string("Illegal operator '") + op + "'");
	}
	if (op == '#') {
		if (values.size() < 1) {
			throw std::runtime_error("Not enough operands for sqrt operator");
		}
		T a = values.back(); values.pop_back();
		T res = sqrtOp(a);
		values.push_back(res);
		return res;
	}
	if (values.size() < 2) {
		throw std::runtime_error(std::string("Not enough operands for operator '") + op + "'");
	}
	T b = values.back(); values.pop_back();
	T a = values.back(); values.pop_back();

	static std::map<char, std::function<T(T, T)>> opMap = {
	{'+', [this](T x, T y) { return this->add(x, y); }},
	{'-', [this](T x, T y) { return this->sub(x, y); }},
	{'*', [this](T x, T y) { return this->mul(x, y); }},
	{'^', [this](T x, T y) { return powOp(x, y); }},
	{'/', [this](T x, T y) { return this->div(x, y); }}
	};

	auto iterator = opMap.find(op);
	if (iterator != opMap.end()) {
		T res = iterator->second(a, b);
		values.push_back(res);
		return res;
	}
	else {
		throw std::runtime_error(std::string("Operator '") + op + "' not implemented");
	}
	
}

template <>
float AdvanceCalculator<float>::strToValue(const std::string& exp, int& pos) 
{
	if (pos >= (int)exp.size())
	{
		throw std::runtime_error(std::string("Error: index out of range"));
	}

	bool negative = false;
	int startPos = pos;
	bool hasDecimalPoint = false;
	// Skip leading whitespace
	while (pos < exp.size() && std::isspace(exp[pos])) pos++;
	// Check for optional sign
	if (pos < exp.size() && exp[pos] == '-')
	{
		negative = true;
		pos++;
	}
	else if (pos < exp.size() && exp[pos] == '+')
	{
		pos++;
	}
	while (pos < exp.size() && 
		(std::isdigit(exp[pos]) || exp[pos] == '.'))
	{
		if (exp[pos] == '.')
		{
			if (hasDecimalPoint)
			{
				throw std::runtime_error(std::string("Error: Multiple decimal points in number"));
			}
			hasDecimalPoint = true;
		}
		pos++;
	}
	// Check for invalid characters after the number
	if (pos < exp.size() && (exp[pos] == '.' || exp[pos] == 'i'))
	{
		throw std::runtime_error(std::string("Error: expecting only integer"));
	}
	float rawValue = static_cast<float>(std::stof(exp.substr(startPos, pos - startPos)));
	rawValue = negative ? -rawValue : rawValue;
	return rawValue;
}

template <>
std::complex<float> AdvanceCalculator<std::complex<float>>::strToValue(const std::string& exp, int& pos)
{
	float realPart = 0.0f;
	float imagPart = 0.0f;

	// skip spaces
	while (pos < (int)exp.size() && std::isspace(exp[pos]))
		pos++;
	if (pos >= (int)exp.size())
	{
		throw std::runtime_error(std::string("Error: index out of range"));
	}

	// check if the current part is imaginary (ending with 'i') or real
	int startPos = pos;
	bool negative = false;
	if (exp[pos] == '-')
	{
		negative = true;
		pos++;
	}
	else if (exp[pos] == '+')
	{
		pos++;
	}
	bool hasDecimal = false;
	while (pos < (int)exp.size() && (std::isdigit(exp[pos]) || exp[pos] == '.'))
	{
		if (exp[pos] == '.')
		{
			if (hasDecimal)
			{
				throw std::runtime_error(std::string
				("Error: multiple decimal points in number"));
			}
			hasDecimal = true;
		}
		pos++;
	}
	std::string numberPart = exp.substr(startPos, pos - startPos);

	// if next char is 'i', this is pure imaginary
	if (pos < (int)exp.size() && exp[pos] == 'i')
	{
		if (numberPart.empty() || numberPart == "+" || numberPart == "-")
		{
			imagPart = (numberPart == "-") ? -1.0f : 1.0f;
		}
		else
		{
			imagPart = std::stof(numberPart);
		}
		pos++; // skip 'i'
		return std::complex<float>(0.0f, imagPart);
	}
	else
	{
		// pure real
		if (numberPart.empty())
		{
			throw std::runtime_error(std::string("Error: expecting number"));
		}
		realPart = std::stof(numberPart);
		return std::complex<float>(realPart, 0.0f);
	}
}

template <class T>
T AdvanceCalculator<T>::solve(const std::string& expression)
{
	std::vector <T> values;
	std::vector<char> operators;
	int index = 0;
	bool expectNumber = true;

	// precedence map
	std::map<char, int> precedence = {
		{'+', 1}, {'-', 1},
		{'*', 2}, {'/', 2},
		{'^', 3}, {'#', 3} // sqrt '#' treated as unary operator
	};
	// associativity map: true = right associative, false = left associative
	std::map<char, bool> rightAssoc = {
		{'+', false}, {'-', false},
		{'*', false}, {'/', false},
		{'^', true}, {'#', true} // right-associative
	};

	//loop on the expression until the end of it
	while (true)
	{
		// skip spaces
		while (index < (int)expression.size() &&
			std::isspace(expression[index]))
			index++;
		//end of the expression
		if (index >= (int)expression.size())
			break;

		//expecting a number
		if (expectNumber)
		{
			//translate to number if the current char is a digit or a sign or 'i' (for complex numbers)
			if (std::isdigit(expression[index]) || expression[index] == 'i' ||
				expression[index] == '+' || expression[index] == '-')
			{
				int startIndex = index;
				T value = strToValue(expression, startIndex);
				if (startIndex == index)  
				{
					throw std::runtime_error(std::string
					("Error: expecting number at position ") + std::to_string(index));
				}
				values.push_back(value);
				index = startIndex;
				expectNumber = false;
			}
			else if (expression[index] == '(')
			{
				operators.push_back('(');
				index++;
				expectNumber = true;
			}
			else if (expression[index] == '#')
			{
				operators.push_back('#');
				index++;
				expectNumber = true;
			}
			//if there are two operators one after the other without a number in between, it's an error
			else
			{
				throw std::runtime_error(std::string
				("Error: expecting number but found '") + expression[index] + "'");
			}
		}
		//expecting an operator
		else
		{
			char op = expression[index];
			//if the operator is a closing parenthesis, we need to apply all operators until we find the matching opening parenthesis
			if (op == ')')
			{
				while (!operators.empty() && operators.back() != '(')
				{
					T calcVal = applyOp(values, operators.back());
					operators.pop_back();
				}
				//missing (
				if (operators.empty())
				{
					throw std::runtime_error(std::string("Error: Mismatched parentheses"));
				}
				operators.pop_back(); // remove '('
				index++;
				expectNumber = false;
			}

			else if (isLegalOperaotor(op))
			{
				// pop from operator stack according to Shunting Yard rules
				while (!operators.empty() && operators.back() != '(')
				{
					char topOp = operators.back();
					int topPrecedence = precedence[topOp];
					int currentPrecedence = precedence[op];

					// check associativity
					if (!rightAssoc[op] && topPrecedence >= currentPrecedence)
					{
						T calcVal = applyOp(values, topOp);
						operators.pop_back();
					}
					else if (rightAssoc[op] && topPrecedence > currentPrecedence)
					{
						T calcVal = applyOp(values, topOp);
						operators.pop_back();
					}
					else break;
				}
				operators.push_back(op);
				index++;
				expectNumber = true;
			}
			//if the operator is not legal, it's an error
			else
			{
				throw std::runtime_error(std::string
				("Error: Illegal character '") + op + "'");
				//index++; // move past illegal character to avoid infinite loop
			}
		}
	}
	//apply the remaining operators in the stack
	while (!operators.empty())
	{
		if (operators.back() == '(')
		{
			throw std::runtime_error(std::string("Error: Mismatched parentheses"));
		}
		T calcVal = applyOp(values, operators.back());
		operators.pop_back();
		if (calcVal == std::numeric_limits<T>::max())
		{
			return std::numeric_limits<T>::max();
		}
	}
	if (values.size() != 1)
	{
		throw std::runtime_error(std::string("Error: Invalid expression"));
	}
	return values.back();
}

template<class T>
void AdvanceCalculator<T>::printAndSolveFile(const std::string& path)
{
	std::ifstream file(path);
	if (!file.is_open()) {
		throw std::runtime_error(std::string
		("Error: Could not open file '") + path + "'");
	}
	//transform the file content into a single string, then split it by \n to get each expression
	std::stringstream contentBuf;
	contentBuf << file.rdbuf();
	std::string fileContent = contentBuf.str();
	std::regex seperator("\\\\n");
	std::sregex_token_iterator expression(fileContent.begin(), fileContent.end(), seperator, -1);
	std::sregex_token_iterator end;
	while (expression != end)
	{
		std::string exp = *expression;
		try {
			T result = solve(exp);
			std::cout << exp << " = " << result << "\n";
		}
		catch (const std::exception& e) {
			std::cout << exp << " = Error (" << e.what() << ")\n";
		}
		++expression;
	}
}

template <class T>
void AdvanceCalculator<T>::solveFileAndAppend(const std::string& inputPath, const std::string& outputPath)
{
	std::ifstream inputFile(inputPath);
	if (!inputFile.is_open()) {
		throw std::runtime_error(std::string
		("Error: Could not open file '") + inputPath + "'");
	}
	std::ofstream outputFile(outputPath, std::ios::app);
	if (!outputFile.is_open()) {
		throw std::runtime_error(std::string
		("Error: Could not open file '") + outputPath + "'");
	}
	//transform the file content into a single string, then split it by \n to get each expression
	std::stringstream contentBuf;
	contentBuf << inputFile.rdbuf();
	std::string fileContent = contentBuf.str();
	std::regex seperator("\\\\n");
	std::sregex_token_iterator expression(fileContent.begin(), fileContent.end(), seperator, -1);
	std::sregex_token_iterator end;
	while (expression != end)
	{
		std::string exp = *expression;
		try {
			T result = solve(exp);
			outputFile << exp << " = " << result << "\n";
		}
		catch (const std::exception& e) {
			outputFile << exp << " = Error (" << e.what() << ")\n";
		}
		++expression;
	}
	inputFile.close();
	outputFile.close();
}
template class AdvanceCalculator<float>;
template class AdvanceCalculator<std::complex<float>>;