#include "InputParser.hpp"
#include "InputData.hpp"
#include "taskranger/data/Environment.hpp"
#include "taskranger/util/StrUtil.hpp"
#include <iostream>
#include <map>
#include <regex>

namespace taskranger {

InputParser::InputParser() {}

void InputParser::parseInput(int argc, const char* argv[]) {

    std::shared_ptr<InputData> data = Environment::getInstance()->getInputData();

    if (argc == 1) {
        // The first argument is always the binary execution.
        // When this ends up being properly installed, the first argument
        // will for an instance be "taskranger"
        // By default, this shows the upcoming tasks
        data->data["subcommand"] = "next";

        return;
    }

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

    // Static keys:
    // 1. subcommand  - the invoked subcommand
    // 2. description - string content
    // Content may or may not be present. Aside these, there are static
    // labels associated with specific values, but these are TBD at a
    // later convenience.
    // Custom labels can also go in here.

    auto& tokens = data->data;
    auto& tags = data->tags;
    tokens["subcommand"] = subCommand;

    // word: toggles when the word ends
    // completedWord: whether we've processed the word or not. Any
    // further words are discarded for semantic reasons.
    bool isInWord = false, completedWord = false;

    // whether or not everything should be considered plain text.
    // This is set to true of -- is detected
    bool rawText = false;

    std::string currentToken;

    for (unsigned long i = 0; i < words.size(); i++) {
        std::string word = words[i];
        bool processed = false;
        if (word.length() == 0)
            continue;
        if (word == "--") {
            rawText = true;
            continue;
        } else if (!rawText) {
            if (std::regex_search(word, InputParser::labelRegex)) {
                // Label of some type
                std::vector<std::string> value = StrUtil::splitString(word, ":", 1);

                tokens[value[0]] = value[1];
                processed = true;
            } else if (word.at(0) == '+' || word.at(0) == '-') {
                // We have a tag!
                // Caveat: this cannot detect:
                // > "+some thing"
                // vs
                // > +"some thing"
                // because of the way shells convert quotes into strings or whatever *shrug*
                if (std::find(tags.begin(), tags.end(), word) == tags.end()) {
                    // ... and as a bonus, we need ot protect from duplicate tags.
                    // Super basic check though; this doesn't avoid +tag -tag, but
                    // meh. This avoids accidents; +tag -tag is more likely to be intentional.
                    tags.push_back(word);
                }
                processed = true;
            } else if (word.at(0) == '@') {
                // We have a project!
                tokens["project"] = word;
                processed = true;
            }
        }
        if (!processed) {
            if (!completedWord) {
                // Because both @ and + can result in parsing issues, this tries to detect
                // when the first char is a backslash.
                // This may potentially have unwanted side-effects, and may be removed
                // if it's determined that the side-effects outweigh the benefits.
                if (word.at(0) == '\\' && word.size() > 1 && word.at(1) != word.at(0))
                    word = word.substr(1);

                // Whitespace guarded to prevent accidental trailing spaces
                // for no reason what so ever.
                if (isInWord)
                    currentToken += " ";
                currentToken += word;

                if (!isInWord)
                    isInWord = true;
            }
            // Exit case; if it's the last word, it's time to abort anyway
            if (i != words.size() - 1)
                continue;
        }
        // Cleans up the word. Basically, this sets the description.
        if (isInWord) {
            isInWord = false;
            completedWord = true;
            tokens["description"] = currentToken;
        }
    }
}

} // namespace taskranger
