#include "spellcheck/SpellChecker.hpp"
#include <iostream>
#include <string>
#include <limits>

int main() {
    using namespace Lexis::SpellCheck;
    SpellChecker checker;

    // 1. Load your persistent personal dictionary
    checker.LoadFromFile();

    // 2. Load the main Nuspell dictionary
    if (!checker.LoadDictionary("/usr/share/hunspell/en_US")) {
        std::cout << "Warning: Could not load Nuspell dictionary. Suggestions disabled." << std::endl;
    }

    std::string input;
    while (true) {
        std::cout << "\nEnter word to spellcheck (or 'exit' to quit): ";
        std::cin >> input;

        // Check for exit condition
        if (input == "exit") {
            break; 
        }

        // Step A: Check Nuspell first
        SpellResult result = checker.Check(input);

        if (result.isCorrect) {
            std::cout << "  [✓] '" << input << "' is spelled correctly (Nuspell)." << std::endl;
        } 
        // Step B: If Nuspell fails, check if it's in our personal Trie
        else if (checker.Contains(input)) {
            std::cout << "  [✓] '" << input << "' found in your personal dictionary." << std::endl;
        }
        // Step C: Misspelled everywhere
        else {
            std::cout << "  [✗] '" << input << "' is misspelled!" << std::endl;
            
            if (!result.suggestions.empty()) {
                std::cout << "  Suggestions: ";
                for (size_t i = 0; i < result.suggestions.size() && i < 5; ++i) {
                    std::cout << result.suggestions[i] << (i < 4 ? ", " : "");
                }
                std::cout << std::endl;
            }

            // Ask to add to the text dictionary
            std::cout << "  Add '" << input << "' to your personal dictionary? (y/n): ";
            char choice;
            std::cin >> choice;
            
            if (choice == 'y' || choice == 'Y') {
                checker.Insert(input); // This updates Trie AND dictionary.txt
                std::cout << "  [+] Saved to Personal Dictionary " << std::endl;
            }
            
            // Clean the buffer for the next loop
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    // Final exit message
    std::cout << "\n[Exiting Lexis Engine... All changes saved. Goodbye!]" << std::endl;
    
    return 0;
}