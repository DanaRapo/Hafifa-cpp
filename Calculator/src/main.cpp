#include "..\Include\BasicCalculator.h"
#include "..\Include\AdvanceCalculator.h"

int main()
{
	int choice;
	BasicCalculator<int> realCalc;
	AdvanceCalculator<float> fractuionCalc;
	AdvanceCalculator<std::complex<float>> complexCalc;
	complexCalc.printAndSolveFile("exeForCalc.txt");
	complexCalc.solveFileAndAppend("exeForCalc.txt", "answers.txt");
	while (true)
	{
		std::cout << "\nSelect calculator:\n";
		std::cout << "1 - Regular calculator\n";
		std::cout << "2 - Fraction calculator\n";
		std::cout << "3 - Complex number calculator\n";
		std::cout << "9 - Exit\n";
		std::cout << "Choice: ";
		std::cin >> choice;
		std::cin.ignore();

		if (choice == 9) {
			std::cout << "Exiting...\n";
			break;
		}
		if (choice != 1 && choice != 2 && choice != 3 && choice != 0)
		{
			std::cout << "invalid choice, please try again\n";
			continue;
		}
		while (true)
		{
			std::string expression;
			std::cout << "Enter expression (or '0' to return to menu): ";
			std::getline(std::cin, expression);

			if (expression == "0") {
				std::cout << "Returning to menu...\n";
				break;
			}
			if (choice == 1)
			{
				int result = realCalc.solve(expression);
				if (result != std::numeric_limits<int>::max())
				{
					std::cout << expression << " = " << result << "\n";
				}

			}
			else if (choice == 2)
			{
				float result = fractuionCalc.solve(expression);
				if (result != std::numeric_limits<float>::max())
				{
					std::cout << expression << " = " << result << "\n";
				}
			}
			else if (choice == 3)
			{
				std::complex<float> result = complexCalc.solve(expression);
				if (result != std::numeric_limits<std::complex<float>>::max())
				{
					std::cout << expression << " = " << result<< "\n";
				}
			}
		}
	}
	return 0;
}