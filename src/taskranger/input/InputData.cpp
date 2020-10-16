#include "InputData.hpp"
#include "taskranger/util/ColorPrinter.hpp"

namespace taskranger {

void InputData::removeTokens(const std::vector<std::string>& invalidFields) {
    for (const auto& key : invalidFields) {
        if (this->data.find(key) != this->data.end()) {
            ColorPrinter printer;
            printer << ANSIFeature::FOREGROUND << 9 << "WARNING!" << ANSIFeature::CLEAR
                    << " Attempted use of parameter \"" << key << "\""
                    << " is not valid in this context, and it has been dropped."
                    << " If you believe this to be a mistake, open an issue on GitHub: "
                    << "https://github.com/lunarwatcher/taskranger"
                    << "\n";
        }
    }
}

void InputData::removeInternalFields() {
    this->removeTokens({"id", "uuid"});
}

void InputData::createSelectiveFrom(std::shared_ptr<InputData> orig, std::vector<std::string> legalKeys) {
    for (auto& k : legalKeys) {
        if (orig->data.find(k) != orig->data.end()) {
            this->data[k] = orig->data[k];
        }
    }
}
} // namespace taskranger
