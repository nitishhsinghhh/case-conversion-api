#ifndef LEXIS_SPELLCHECKER_HPP
#define LEXIS_SPELLCHECKER_HPP

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <nuspell/dictionary.hxx> // For m_dict

namespace Lexis::SpellCheck {

    // Result structure for the Check() method
    struct SpellResult {
        bool isCorrect;
        std::vector<std::string> suggestions;
    };

    // Trie Node for the internal fallback dictionary
    struct TrieNode {
        std::unordered_map<char, std::shared_ptr<TrieNode>> children;
        bool isEndOfWord = false;
    };

    class SpellChecker {
    public:
        SpellChecker();

        // Trie-based methods
        void Insert(const std::string& word);
        bool Contains(const std::string& word) const;
        void LoadSampleDictionary();

        // Nuspell-based methods
        bool LoadDictionary(const std::string& path);
        SpellResult Check(const std::string& word) const;

        void LoadFromFile();

    private:
        std::shared_ptr<TrieNode> root;
        std::unique_ptr<nuspell::Dictionary> m_dict; // Use unique_ptr for RAII ownership
        std::string m_dataPath = "dictionary.txt"; // Default filename
    };

} // namespace Lexis::SpellCheck

#endif // LEXIS_SPELLCHECKER_HPP