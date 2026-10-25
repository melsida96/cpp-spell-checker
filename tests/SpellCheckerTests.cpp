#include "SpellChecker.h"

#include <iostream>
#include <string_view>
#include <vector>

int main() {
    int passed = 0;
    int failed = 0;

    auto check = [&](std::string_view name, bool condition) {
        if (condition) {
            ++passed;
        } else {
            ++failed;
            std::cerr << "FAIL: " << name << '\n';
        }
    };

    auto checkSuggestions = [&](std::string_view name,
                                const SpellChecker::SpellChecker& checker,
                                std::string_view input,
                                std::vector<std::string_view> expected) {
        const auto actual = checker.Suggest(input);
        const bool equal = (actual == expected);
        check(name, equal);

        if (!equal) {
            std::cerr << "  Input:    " << input << "\n  Expected:";
            for (auto word : expected) std::cerr << ' ' << word;
            std::cerr << "\n  Actual:  ";
            for (auto word : actual) std::cerr << ' ' << word;
            std::cerr << '\n';
        }
    };

    // Case-insensitive lookup and removal of duplicate dictionary words.
    {
        SpellChecker::SpellChecker checker({"Apple", "APPLE", "banana", "BANANA", "Orange"});
        check("Contains lowercase", checker.Contains("apple"));
        check("Contains uppercase", checker.Contains("APPLE"));
        check("Contains mixed case", checker.Contains("BaNaNa"));
        check("Missing word", !checker.Contains("pear"));
        checkSuggestions("Duplicate words", checker, "appl", {"Apple"});
        checkSuggestions("Exact word needs no suggestion", checker, "apple", {});
    }

    // One deletion; also check two non-adjacent deletions.
    {
        SpellChecker::SpellChecker checker({"abc"});
        checkSuggestions("Delete first char", checker, "xabc", {"abc"});
        checkSuggestions("Delete middle char", checker, "abxc", {"abc"});
        checkSuggestions("Delete last char", checker, "abcx", {"abc"});
        checkSuggestions("Delete repeated char", checker, "abbc", {"abc"});
        checkSuggestions("Cannot fix with one deletion", checker, "axyd", {});
        checkSuggestions("Two non-adjacent deletions", checker, "axbyc", {"abc"});
        checkSuggestions("Non-adjacent repeated chars", checker, "aabbc", {"abc"});
        checkSuggestions("Adjacent deletions not allowed", checker, "abxyc", {});
        checkSuggestions("Adjacent deletions at end", checker, "abcxy", {});
    }

    // One insertion, two non-adjacent insertions, and forbidden adjacent ones.
    {
        SpellChecker::SpellChecker checker({"abxc"});
        checkSuggestions("One insertion", checker, "abc", {"abxc"});
    }
    {
        SpellChecker::SpellChecker checker({"axbyc"});
        checkSuggestions("Two non-adjacent insertions", checker, "abc", {"axbyc"});
    }
    {
        SpellChecker::SpellChecker checker({"abxyc"});
        checkSuggestions("Adjacent insertions not allowed", checker, "abc", {});
    }

    // Same length: one deletion followed by one insertion costs 2.
    {
        SpellChecker::SpellChecker checker({"axc", "acb"});
        checkSuggestions("Replacement or letter move", checker, "abc", {"axc", "acb"});
    }

    // Prefer cost-1 suggestions, and preserve dictionary order.
    {
        SpellChecker::SpellChecker checker({"axbyc", "axc", "abxc", "abbc", "abc", "axbc"});
        checkSuggestions("Cost 1 has priority", checker, "abc", {"abxc", "abbc", "axbc"});
    }
    {
        SpellChecker::SpellChecker checker({"axbyc", "acb", "axc"});
        checkSuggestions("Cost 2 keeps dictionary order", checker, "abc", {"axbyc", "acb", "axc"});
    }
    {
        SpellChecker::SpellChecker checker({"banana", "orange"});
        checkSuggestions("No correction", checker, "zzzz", {});
    }
    {
        SpellChecker::SpellChecker checker({});
        check("Empty dictionary lookup", !checker.Contains("abc"));
        checkSuggestions("Empty dictionary suggestions", checker, "abc", {});
    }

    std::cout << "Passed: " << passed << ", Failed: " << failed << '\n';
    return failed == 0 ? 0 : 1;
}
