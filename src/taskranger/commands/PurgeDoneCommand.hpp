#include "Command.hpp"

namespace taskranger {

class PurgeDoneCommand : public Command {
public:
    PurgeDoneCommand();
    void run() override;
};

} // namespace taskranger
