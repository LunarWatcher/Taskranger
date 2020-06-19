#pragma once

#include <memory>
#include <string>

namespace taskranger {

// Pre-declaration of Attribute
class Attribute;

namespace UDAAttribute {

std::shared_ptr<Attribute> makeAttribute(const std::string& attributeName);

}

} // namespace taskranger
