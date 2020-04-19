#ifndef TASKRANGER_UTIL_STRUTIL_HPP
#define TASKRANGER_UTIL_STRUTIL_HPP

#include <string>
#include <vector>

namespace taskranger {
namespace Util {

    inline void splitString(std::string input, const std::string& delimiter, std::vector<std::string>& out,
                            int limit = -1) {
        size_t pos = 0;
        std::string token;
        int count = 0;
        while ((pos = input.find(delimiter)) != std::string::npos) {
            token = input.substr(0, pos);
            out.push_back(token);
            input.erase(0, pos + delimiter.length());
            count++;
            if (count == limit) {
                out.push_back(input);
                return;
            }
        }
    }

} // namespace Util
} // namespace taskranger

#endif
