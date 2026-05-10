#include "spellcheck/Trie.hpp"
#include <iostream>
#include <algorithm>

namespace Lexis::SpellCheck {

    SpellChecker::SpellChecker() {
        root = std::make_shared<TrieNode>();
    }

    void SpellChecker::Insert(const std::string& word) {
        auto current = root;
        for (char ch : word) {
            if (current->children.find(ch) == current->children.end()) {
                current->children[ch] = std::make_shared<TrieNode>();
            }
            current = current->children[ch];
        }
        current->isEndOfWord = true;
    }

    bool SpellChecker::Contains(const std::string& word) const {
        auto current = root;
        for (char ch : word) {
            if (current->children.find(ch) == current->children.end()) {
                return false;
            }
            current = current->children[ch];
        }
        return current->isEndOfWord;
    }

    void SpellChecker::LoadSampleDictionary() {
        std::vector<std::string> words = {"apple", "apply", "algorithm", "backend", "native", "interop"};
        for (const auto& w : words) Insert(w);
    }
}

int main() {
    using namespace Lexis::SpellCheck;
    
    SpellChecker checker;
    checker.LoadSampleDictionary();

    std::vector<std::string> testWords = {"algrithm", "apple", "swift", "native", "dotnet"};

    std::cout << "--- Lexis SpellCheck Engine ---" << std::endl;
    for (const auto& word : testWords) {
        bool exists = checker.Contains(word);
        std::cout << "[" << (exists ? "VALID" : "ERROR") << "] " << word << std::endl;
    }

    return 0;
}