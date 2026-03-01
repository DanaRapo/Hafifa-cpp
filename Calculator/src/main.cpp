#include "..\Include\BasicCalculator.h"

int main()
{
	BasicCalculator<int> calculator;
	std::cout << "3+ 4 * 2 / (1 - 5) = " << calculator.solve("3 + 4 * 2 / (1 - 5)") <<"\n";
	std::cout << "10 + 2 * 6 = " << calculator.solve("10 + 2 * 6") << "\n";
	std::cout << "100 * (2 + 12) = " << calculator.solve("100 * (2 + 12)") << "\n";
	std::cout << "100 * (2 + 12) = " << calculator.solve("100 * 2 + 12)") << "\n";
	return 0;
}