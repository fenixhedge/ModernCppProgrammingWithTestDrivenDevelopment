#include "RomanConverter.h"
#include <string>

using namespace std;

RomanConverter::RomanConverter() {
}

string RomanConverter::convert(int arabies) {
	int arabics[] = { 1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
	string romans[] = { "M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
	int numsOfDigits = sizeof(arabics) / sizeof(int);

	string roman{""};
	for (int i = 0; i < numsOfDigits; i++) {
		while (arabies >= arabics[i]) {
			arabies -= arabics[i];
			roman += romans[i];
		}
	}

	return roman;
}
