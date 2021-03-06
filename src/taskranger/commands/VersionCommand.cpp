#include "VersionCommand.hpp"
#include "taskranger/metadata/Version.hpp"
#include "taskranger/util/ColorPrinter.hpp"

namespace taskranger {

VersionCommand::VersionCommand() {
    this->name = "version";
    this->help = "Shows the current version of Taskranger. "
                 "For alpha versions and pre-releases, this may not be representative of the current version";
    this->usage = "taskranger version";
}

void VersionCommand::run() {
    ColorPrinter printer;
    printer << ANSIFeature::BOLD << "Taskranger " << TASKRANGER_VERSION << ANSIFeature::CLEAR << "\n"
            << "License: " << TASKRANGER_LICENSE << "\n\n"
            << TASKRANGER_CHANGELOG << std::endl;
    printer << "\nOpen-source libraries used:\n" << TASKRANGER_DEPS_LICENSES << std::endl;
}

} // namespace taskranger
