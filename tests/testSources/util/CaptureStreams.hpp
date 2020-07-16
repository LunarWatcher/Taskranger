#pragma once

#include <iostream>
#include <sstream>
#include <type_traits>

namespace CaptureStreams {

template <class T, typename = std::enable_if_t<std::is_base_of_v<std::ios_base, T>>>
class StreamCapturer {
private:
    T& stream;
    // We need to store the buffer to restore it afterwards
    decltype(stream.rdbuf()) buffer;
    std::stringstream control;

public:
    StreamCapturer(T& stream) : stream(stream) {
        this->buffer = stream.rdbuf();
        this->stream.rdbuf(control.rdbuf());
    }

    ~StreamCapturer() {
        restore();
    }

    void restore() {
        if (buffer != nullptr) {
            stream.rdbuf(buffer);
        }
    }

    void reset() {
        stream.rdbuf(buffer);
    }

    std::stringstream& getBuffer() {
        return this->control;
    }

    std::string getBufferContent() {
        return this->control.str();
    }
};

} // namespace CaptureStreams
