#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

namespace Lexis::SpellCheck {

    struct TrieNode {
        std::unordered_map<char, std::shared_ptr<TrieNode>> children;
        bool isEndOfWord = false;
    };

    class SpellChecker {
    private:
        std::shared_ptr<TrieNode> root;

    public:
        SpellChecker();
        void Insert(const std::string& word);
        bool Contains(const std::string& word) const;
        void LoadSampleDictionary();
    };
}