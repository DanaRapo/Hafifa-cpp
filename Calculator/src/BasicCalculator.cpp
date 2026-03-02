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
		std::cout <<"Error: Division by zero\n";
		return {};
	}
	return a / b; 
}

template <class T>
T BasicCalculator<T>::applyOp(std::vector<T>& values, char op)
{
	if (!isLegalOperaotor(op)) {
		std::cout <<"Error: Illegal operator '" << op << "'\n";
		return std::numeric_limits<T>::max();
	}
	if (values.size() < 2)
	{
		std::cout << "Error: Not enough operands for operator '" << op << "'\n";
		return std::numeric_limits<T>::max();
	}

	T b = values.back(); values.pop_back();
	T a = values.back(); values.pop_back();

	static std::map<char, std::function<T(T&, T&)>> opMap = {
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
		std::cout << "Error: Operator '" << op << "' not implemented\n";
		return std::numeric_limits<T>::max();
	}
}

template <class T>
T BasicCalculator<T>::strToValue(const std::string& exp, int& pos)
{
	if (!std::is_same_v<T, int>)
	{
		std::cout << "Error: expecting only integer\n";
		return std::numeric_limits<T>::max();
	}
	if (pos >= (int)exp.size())
	{
		std::cout << "Error: index out of range\n";
		return std::numeric_limits<T>::max();
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
		std::cout << "Error: expecting only integer\n";
		return std::numeric_limits<T>::max();
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
                std::cout << "Error: Expected number but found '"
                    << expression[index] << "'\n";
                return std::numeric_limits<T>::max();
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
                    std::cout << "Error: Mismatched parentheses\n";
                    return std::numeric_limits<T>::max();
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
                std::cout << "Error: Illegal character '"
                    << op << "'\n";
                return std::numeric_limits<T>::max();
            }
        }
    }

    // Apply remaining operators
    while (!operators.empty())
    {
        if (operators.back() == '(')
        {
            std::cout << "Error: Mismatched parentheses\n";
            return std::numeric_limits<T>::max();
        }

        applyOp(values, operators.back());
        operators.pop_back();
    }

    if (values.size() != 1)
    {
        std::cout << "Error: Invalid expression\n";
        return std::numeric_limits<T>::max();
    }

    return values.back();
}

template class BasicCalculator<int>;
template class BasicCalculator<float>;
template class BasicCalculator<std::complex<float>>;