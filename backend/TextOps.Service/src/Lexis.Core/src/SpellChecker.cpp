#include "spellcheck/SpellChecker.hpp"
#include <fstream>   // Fixes basic_ofstream error
#include <algorithm>
#include <cctype>

namespace Lexis::SpellCheck {

    SpellChecker::SpellChecker() : root(std::make_shared<TrieNode>()), m_dict(nullptr) {}

    // ONLY ONE Insert function
    void SpellChecker::Insert(const std::string& word) {
        std::string normalized = word;
        std::transform(normalized.begin(), normalized.end(), normalized.begin(), ::tolower);
        
        auto curr = root;
        for (char ch : normalized) {
            if (curr->children.find(ch) == curr->children.end()) 
                curr->children[ch] = std::make_shared<TrieNode>();
            curr = curr->children[ch];
        }

        if (!curr->isEndOfWord) {
            curr->isEndOfWord = true;
            // DICTIONARY_PATH comes from your CMake target_compile_definitions
            std::ofstream outfile(DICTIONARY_PATH, std::ios_base::app);
            if (outfile.is_open()) {
                outfile << normalized << "\n";
            }
        }
    }

    bool SpellChecker::Contains(const std::string& word) const {
        std::string normalized = word;
        std::transform(normalized.begin(), normalized.end(), normalized.begin(), ::tolower);
        
        auto curr = root;
        for (char ch : normalized) {
            if (curr->children.find(ch) == curr->children.end()) return false;
            curr = curr->children[ch];
        }
        return curr->isEndOfWord;
    }

    void SpellChecker::LoadFromFile() {
        std::ifstream infile(DICTIONARY_PATH);
        std::string word;
        while (infile >> word) {
            auto curr = root;
            for (char ch : word) {
                if (curr->children.find(ch) == curr->children.end()) 
                    curr->children[ch] = std::make_shared<TrieNode>();
                curr = curr->children[ch];
            }
            curr->isEndOfWord = true;
        }
    }

    void SpellChecker::LoadSampleDictionary() { 
        for (auto w : {"apple", "native"}) Insert(w); 
    }

    bool SpellChecker::LoadDictionary(const std::string& p) { 
        try { 
            auto dict_obj = nuspell::Dictionary::load_from_path(p);
            m_dict = std::make_unique<nuspell::Dictionary>(std::move(dict_obj));
            return m_dict != nullptr; 
        } catch(...) { return false; } 
    }

    SpellResult SpellChecker::Check(const std::string& w) const {
        SpellResult r{false, {}};
        if (m_dict) { 
            r.isCorrect = m_dict->spell(w); 
            if (!r.isCorrect) m_dict->suggest(w, r.suggestions); 
        }
        return r;
    }
} // namespace ends here