#include "InputParser.hpp"
#include "InputData.hpp"
#include <iostream> 
#include <map>
#include "taskranger/util/StrUtil.hpp"
#include <regex>

namespace taskranger {

InputParser::InputParser() {

}

std::shared_ptr<InputData> InputParser::parseInput(int argc, const char* argv[]) {
    
    std::shared_ptr<InputData> data = std::make_shared<InputData>();
    
    if (argc == 1) {
        // The first argument is always the binary execution.
        // When this ends up being properly installed, the first argument
        // will for an instance be "taskranger"
        // By default, this shows the upcoming tasks
        data->tokens["subcommand"] = "next"; 

        return data;
    }
    
    /*
      A note on various formats:
      --fullform 
      --fullform=variable
      -switch
      field:value   - NOTE: can contain quotes ("") 
      plain text    
     */

    // Identify the command 
    // Taskranger uses a rigid structure on commands vs. arguments to avoid parsing guesses
    // For an instance, taken the input:
    // taskranger 1 2 3 modify text project:abcd
    // what's the command, what's text, and what's arguments? 
    // Requiring that to be:
    // taskranger modify 1 2 3 text project:abcd 
    // (where the arguments after 123 can be in more or less any order, but still assumes it's bundled)
    // This is mainly to allow for predictability, as well as making it easier to code 
    // TODO: move the above comment into TaskData
    std::vector<std::string> words; 
    for (int i = 1; i < argc; i++) { 
        words.push_back(std::string(argv[i]));
    }

    std::string subCommand = words[0];
    words.erase(words.begin());
    std::cout << "Subcommand: " << subCommand << std::endl;
    
    // Static keys:
    // 1. subcommand - the invoked subcommand 
    // 2. content    - string content 
    // Content may or may not be present. Aside these, there are static 
    // labels associated with specific values, but these are TBD at a 
    // later convenience. 
    // Custom labels can also go in here.

    auto& tokens = data->tokens;
    auto& tags = data->tags;
    tokens["subcommand"] = subCommand;
    
    // word: toggles when the word ends 
    // completedWord: whether we've processed the word or not. Any 
    // further words are discarded for semantic reasons. 
    bool isInWord = false, completedWord = false;
    std::string currentToken;
    
    for (unsigned long i = 0; i < words.size(); i++) {
        auto& word = words[i];
        if (word.length() == 0)
            continue;
        if (std::regex_search(word, InputParser::labelRegex)){ 
            // Label of some type 
            std::vector<std::string> value;
            Util::splitString(word, ":", value, 1); 

            tokens[value[0]] = value[1];
            
        } else if (word.at(0) == '+') {
            // We have a tag!
            // Caveat: this cannot detect:
            // > "+some thing"
            // vs
            // > +"some thing"
            // because of the way shells convert quotes into strings or whatever *shrug*
            tags.push_back(word); 
        } else {
            if (!completedWord) {
                currentToken += word + " ";
                if (!isInWord)
                    isInWord = true;
            }
            // Exit case; if it's the last letter, it's time to abort anyway
            if (i != words.size() - 1) 
                continue;
        }

        if(isInWord) {
            isInWord = false;
            completedWord = true;
            tokens["content"] = currentToken;
        }
    }

    return data;
}

}
