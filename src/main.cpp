#include "SpellChecker.h"

#include <iostream>
#include <sstream>
#include <utility>

namespace
{
    bool IsAsciiLetter(char c)
    {
        return (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z');
    }

    void PrintCorrectedWord(std::string_view word, const SpellChecker::SpellChecker& checker)
    {
        if (checker.Contains(word))
        {
            std::cout << word;
            return;
        }

        const auto suggestions = checker.Suggest(word);

        if (suggestions.empty())
            std::cout << '{' << word << "?}";
        else if (suggestions.size() == 1)
            std::cout << suggestions.front();
        else
        {
            std::cout << '{';

            for (std::size_t i = 0; i < suggestions.size(); ++i)
            {
                if (i != 0)
                    std::cout << ' ';

                std::cout << suggestions[i];
            }

            std::cout << '}';
        }
    }

    void ProcessTextLine(const std::string& line, const SpellChecker::SpellChecker& checker)
    {
        std::size_t i = 0;

        while (i < line.size())
        {
            if (!IsAsciiLetter(line[i]))
            {
                std::cout << line[i++];
                continue;
            }

            const std::size_t start = i;

            while (i < line.size() && IsAsciiLetter(line[i]))
                ++i;

            const std::string_view word(line.data() + start, i - start);

            PrintCorrectedWord(word, checker);
        }

        std::cout << std::endl;
    }

}

int main()
{
    std::vector<std::string> dictionary;
    std::string line;
    bool foundSeparator = false;

    // Read dictionary
    while (std::getline(std::cin, line))
    {
        if (line == "===")
        {
            foundSeparator = true;
            break;
        }

        std::istringstream words(line);
        std::string word;

        while (words >> word)
            dictionary.push_back(std::move(word));
    }

    if (!foundSeparator)
        return 0;

    const SpellChecker::SpellChecker checker(std::move(dictionary));

    // Process text
    while (std::getline(std::cin, line) && line != "===")
        ProcessTextLine(line, checker);

    return 0;
}
