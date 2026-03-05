#include "..\Include\BasicCalculator.h"

template <class T>
BasicCalculator<T>::BasicCalculator()
{
	// Initialize with basic operators
	legalOperators_ = {'+', '-', '*', '/'};
}

template <class T>
bool BasicCalculator<T>::isLegalOperaotor(char op) const {
	return std::find(legalOperators_.begin(),
		legalOperators_.end(), op) != legalOperators_.end();
}

template <class T>
void BasicCalculator<T>::addLegalOperator(char op) {
	if (!isLegalOperaotor(op)) {
		legalOperators_.push_back(op);
	}
}

template <class T>
BasicCalculator<T>::~BasicCalculator() = default;

template <class T>
T BasicCalculator<T>::add(T a, T b) { return a + b; }

template <class T>
T BasicCalculator<T>::sub(T a, T b) { return a - b; }

template <class T>
T BasicCalculator<T>::mul(T a, T b) { return a * b; }

template <class T>
T BasicCalculator<T>::div(T a, T b) { 
	if (b == T(0)) {
		throw std::runtime_error("Division by zero");
	}
	return a / b; 
}

template <class T>
T BasicCalculator<T>::applyOp(std::vector<T>& values, char op)
{
	if (!isLegalOperaotor(op)) {
		throw std::runtime_error(std::string("Illegal operator '") + op + "'");
	}
	if (values.size() < 2)
	{
		throw std::runtime_error(std::string("Not enough operands for operator '") + op + "'");
	}

	T b = values.back(); values.pop_back();
	T a = values.back(); values.pop_back();

	static std::map<char, std::function<T(T, T)>> opMap = {
		{'+', [this](T x, T y) { return add(x, y); }},
		{'-', [this](T x, T y) { return sub(x, y); }},
		{'*', [this](T x, T y) { return mul(x, y); }},
		{'/', [this](T x, T y) { return div(x, y); }}
	};

	auto iterator = opMap.find(op);
	if (iterator != opMap.end()) {
		T res = iterator->second(a, b);
		values.push_back(res);
		return res;
	} else {
		throw std::runtime_error(std::string("Operator '") + op + "' not implemented");
	}
}

template <class T>
T BasicCalculator<T>::strToValue(const std::string& exp, int& pos)
{
	if (!std::is_same_v<T, int>)
	{
        throw std::runtime_error(std::string("Error: expecting only integer"));
	}
	if (pos >= (int)exp.size())
	{
		throw std::out_of_range(std::string("Error: index out of range"));
	}
	bool negative = false;
	int startPos = pos;
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
	while (pos < exp.size() && std::isdigit(exp[pos]))
	{
		pos++;
	}
	// Check for invalid characters after the number
	if (pos < exp.size() && (exp[pos] == '.' || exp[pos] == 'i'))
	{
		throw std::runtime_error(std::string("Error: expecting only integer"));
	}
	T rawValue = static_cast<T>(std::stoi(exp.substr(startPos, pos - startPos)));
	rawValue = negative ? -rawValue : rawValue;
	return rawValue;
}

template <class T>
T BasicCalculator<T>::solve(const std::string& expression)
{
    std::vector<T> values;
    std::vector<char> operators;

    int index = 0;
    bool expectNumber = true;

	//stops when reach the end of the expression
    while (true)
    {
        // Skip whitespace
        while (index < (int)expression.size() &&
            std::isspace(expression[index]))
            index++;

        if (index >= (int)expression.size())
            break;

        if (expectNumber)
        {
            // Number (with optional sign)
            if (std::isdigit(expression[index]) ||
                expression[index] == '+' ||
                expression[index] == '-')
            {
                int startIndex = index;
                T value = strToValue(expression, startIndex);
                values.push_back(value);

                index = startIndex;
                expectNumber = false;
            }
            // Opening parenthesis
            else if (expression[index] == '(')
            {
                operators.push_back('(');
                index++;
                expectNumber = true;
            }
            else
            {
				throw std::runtime_error(std::string("Error: Expected number but found '")
                    + expression[index] + "'");
            }
        }
        else
        {
            char op = expression[index];

            // Closing parenthesis
            if (op == ')')
            {
                while (!operators.empty() && operators.back() != '(')
                {
                    applyOp(values, operators.back());
                    operators.pop_back();
                }

                if (operators.empty())
                {
					throw std::runtime_error("Error: Mismatched parentheses");
                }

                operators.pop_back(); // remove '('
                index++;
                expectNumber = false;
            }
            // Operator
            else if (isLegalOperaotor(op))
            {
                int currentPrecedence =
                    (op == '+' || op == '-') ? 1 : 2;

                while (!operators.empty() &&
                    operators.back() != '(')
                {
                    char topOp = operators.back();
                    int topPrecedence =
                        (topOp == '+' || topOp == '-') ? 1 : 2;

                    if (topPrecedence >= currentPrecedence)
                    {
                        applyOp(values, topOp);
                        operators.pop_back();
                    }
                    else break;
                }

                operators.push_back(op);
                index++;
                expectNumber = true;
            }
            else
            {
				throw std::runtime_error(std::string
                ("Error: Illegal character '")+ op + "'");

            }
        }
    }

    // Apply remaining operators
    while (!operators.empty())
    {
        if (operators.back() == '(')
        {
			throw std::runtime_error("Error: Mismatched parentheses");
        }

        applyOp(values, operators.back());
        operators.pop_back();
    }

    if (values.size() != 1)
    {
		throw std::runtime_error(std::string("Error: Invalid expression"));
    }
    return values.back();
}

template <class T>
void BasicCalculator<T>::printFileContent(const std::string& path) const
{
	std::ifstream file(path);
    if (!file.is_open()) {
		throw std::runtime_error(std::string
        ("Error: Could not open file '") + path + "'");
	}
    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << "\n";
	}
}
template <class T>
void BasicCalculator<T>::fileHendeler(const std::string& path, std::ostream& output)
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
            output << exp << " = " << result << "\n";
        }
        catch (const std::exception& e) {
            output << exp << " = Error (" << e.what() << ")\n";
        }
        ++expression;
    }
    file.close();
}

template<class T>
void BasicCalculator<T>::printAndSolveFile(const std::string& path)
{
	fileHendeler(path, std::cout);
}

template <class T>
void BasicCalculator<T>::solveFileAndAppend(const std::string& inputPath, const std::string& outputPath)
{
    std::ofstream outputFile(outputPath, std::ios::app);
    if (!outputFile.is_open()) {
        throw std::runtime_error(std::string
        ("Error: Could not open file '") + outputPath + "'");
    }
	fileHendeler(inputPath, outputFile);
    outputFile.close();
}
template class BasicCalculator<int>;
template class BasicCalculator<float>;
template class BasicCalculator<std::complex<float>>;