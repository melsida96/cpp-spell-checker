#include "SpellChecker.h"

namespace SpellChecker
{
    namespace
    {
        std::string ToLowerAscii(std::string_view word)
        {
            std::string result(word);

            for (char& c : result)
            {
                if (c >= 'A' && c <= 'Z')
                    c = static_cast<char>(c - 'A' + 'a');
            }

            return result;
        }


        bool IsOneDeletionAway(std::string_view longer, std::string_view shorter)
        {
            if (longer.size() != shorter.size() + 1)
                return false;

            std::size_t i = 0;

            while (i < shorter.size() && longer[i] == shorter[i])
                ++i;

            return longer.substr(i + 1) == shorter.substr(i);
        }

        bool IsDeletionPlusInsertionAway(std::string_view a, std::string_view b)
        {
            if (a.size() != b.size() || a == b)
                return false;

            const std::size_t n = a.size();

            std::size_t prefix = 0;
            while (prefix < n && a[prefix] == b[prefix])
                ++prefix;

            std::size_t suffix = 0;
            while (suffix < n - prefix && a[n - 1 - suffix] == b[n - 1 - suffix])
                ++suffix;

            const std::size_t len = n - prefix - suffix;

            const auto x = a.substr(prefix, len);
            const auto y = b.substr(prefix, len);

            return (x.substr(1) == y.substr(0, len - 1)) || (x.substr(0, len - 1) == y.substr(1));
        }

        bool IsTwoNonAdjacentDeletionsAway(std::string_view longer, std::string_view shorter)
        {
            // i is the first deleted index; the second index must be >= i + 2.
            for (std::size_t i = 0; i + 2 < longer.size(); ++i)
            {
                if (i > 0 && longer[i - 1] != shorter[i - 1])
                    break;  // the kept prefix no longer matches

                const std::string_view rest_long = longer.substr(i + 1);
                const std::string_view rest_short = shorter.substr(i);
                if (rest_long[0] == rest_short[0] &&
                    IsOneDeletionAway(rest_long.substr(1), rest_short.substr(1)))
                    return true;
            }
            return false;
        }

        int EditCost(std::string_view a, std::string_view b) {
            if (a == b)
                return 0;

            if (a.size() < b.size())
                std::swap(a, b); // ensure a is the longer one

            switch (a.size() - b.size())
            {
            case 0:
                return IsDeletionPlusInsertionAway(a, b) ? 2 : kTooFar;
            case 1:
                return IsOneDeletionAway(a, b) ? 1 : kTooFar;
            case 2:
                return IsTwoNonAdjacentDeletionsAway(a, b) ? 2 : kTooFar;
            default:
                return kTooFar;
            }
        }
    }

    std::vector<std::string_view> SpellChecker::Suggest(std::string_view word) const
    {
        const std::string query = ToLowerAscii(word);
        std::vector<std::string_view> one_edit;
        std::vector<std::string_view> two_edits;
        for (std::size_t i = 0; i < m_words.size(); ++i)
        {
            const int cost = EditCost(query, m_lowercase[i]);
            if (cost == 1)
                one_edit.push_back(m_words[i]);
            else if (cost == 2)
                two_edits.push_back(m_words[i]);
        }

        if (!one_edit.empty())
            return one_edit;

        return two_edits;
    }


    SpellChecker::SpellChecker(std::vector<std::string> words)
    {
        for (std::string& word : words)
        {
            std::string lowercase = ToLowerAscii(word);

            if (m_lookup.insert(lowercase).second)
            {
                m_lowercase.push_back(std::move(lowercase));
                m_words.push_back(std::move(word));
            }
        }
    }


    bool SpellChecker::Contains(std::string_view word) const
    {
        return m_lookup.find(ToLowerAscii(word)) != m_lookup.end();
    }

}