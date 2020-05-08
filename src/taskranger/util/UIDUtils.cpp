#include "UIDUtils.hpp"

#include <random>
#include <sstream>

namespace taskranger {

std::string uuid::generateUuidV4() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> distrib(0, 15);
    static std::uniform_int_distribution<> distrib2(8, 11);

    std::stringstream ss;
    ss << std::hex;

    for (int i = 0; i < 30; i++) {
        if (i == 8 || i == 18) {
            ss << "-";
        } else if (i == 12)
            ss << "-4";
        else if (i == 15)
            ss << "-" << distrib2(gen);
        ss << distrib(gen);
    }

    return ss.str();
}


}
