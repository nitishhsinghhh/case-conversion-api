#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <nuspell/dictionary.hxx>

namespace Lexis::SpellCheck {
    struct SpellResult { bool isCorrect; std::vector<std::string> suggestions; };
    struct TrieNode { std::unordered_map<char, std::shared_ptr<TrieNode>> children; bool isEndOfWord = false; };

    class SpellChecker {
    public:
        SpellChecker();
        void Insert(const std::string& word);
        bool Contains(const std::string& word) const;
        void LoadSampleDictionary();
        bool LoadDictionary(const std::string& path);
        SpellResult Check(const std::string& word) const;
    private:
        std::shared_ptr<TrieNode> root;
        std::unique_ptr<nuspell::Dictionary> m_dict;
    };
}