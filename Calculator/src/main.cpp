#include "..\Include\BasicCalculator.h"
#include "..\Include\AdvanceCalculator.h"

int main()
{
	BasicCalculator<int> calculator;
	std::cout << "3+ 4 * 2 / (1 - 5) = " << calculator.solve("3 + 4 * 2 / (1 - 5)") <<"\n";
	std::cout << "10 + 2 * 6 = " << calculator.solve("10 + 2 * 6") << "\n";
	std::cout << "100 * (2 + 12) = " << calculator.solve("100 * (2 + 12)") << "\n";
	std::cout << "100 * (2 + 12) = " << calculator.solve("100 * 2 + 12)") << "\n";
	AdvanceCalculator<float> fcalculator;
	AdvanceCalculator<std::complex<float>> comcalc;
	std::cout << "sqrt(16) = " << fcalculator.solve("#16") << "\n";
	std::cout << "2^3 = " << fcalculator.solve("2^3") << "\n";
	std::cout << "100 * (2.5 + 12) = " << fcalculator.solve("100 * (2.5 + 12)") << "\n";
	std::cout << "100 * (2 + 12) = " << calculator.solve("100 * (2 + 12") << "\n";

	std::cout << "2+3i + 4-5i = " << comcalc.solve("2+3i + 4-5i") << "\n";
	std::cout << "sqrt(4-3i) = " << comcalc.solve("#(4-3i)") << "\n";
	std::cout << "(1+2i)^(3-4i) = " << comcalc.solve("(1+2i)^(3-4i)") << "\n";
	std::cout << "5+3i*8i= " << comcalc.solve("5+3i*8i") << "\n";



	return 0;
}