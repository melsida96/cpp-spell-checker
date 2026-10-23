#ifndef SPELL_CHECKER_H
#define SPELL_CHECKER_H

#include <string>
#include <string_view>
#include <vector>
#include <unordered_set>

namespace SpellChecker
{
    constexpr int kTooFar = 3;

    class SpellChecker
    {
    public:
        explicit SpellChecker(std::vector<std::string> words);
        bool Contains(std::string_view word) const;
        std::vector<std::string_view> Suggest(std::string_view word) const;

    private:
        std::vector<std::string> m_words;
        std::vector<std::string> m_lowercase;
        std::unordered_set<std::string> m_lookup;
    };

}

#endif  // SPELL_CHECKER_H