#include <iostream>
#include "SpellChecker.h"

int main()
{
    std::vector<std::string> fruits = { "apple", "banana", "orange" };

    SpellChecker::SpellChecker checker(fruits);

    std::cout << "Contains apple: " << checker.Contains("apple") << std::endl;
    std::cout << "Contains appl: "  << checker.Contains("appl") << std::endl;

    const auto suggestions = checker.Suggest("appl");

    std::cout << "Suggestions for 'appl':" << std::endl;

    for (std::string_view word : suggestions) {
        std::cout << "  " << word << std::endl;
    }

    return 0;
}