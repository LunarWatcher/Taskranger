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
        hijack();
    }

    ~StreamCapturer() {
        restore();
    }

    void hijack() {
        this->control.clear();
        this->control.str("");
        this->buffer = stream.rdbuf();
        this->stream.rdbuf(control.rdbuf());
    }

    void restore() {
        if (buffer != nullptr) {
            stream.rdbuf(buffer);
            buffer = nullptr;
        }
    }

    std::stringstream& getBuffer() {
        return this->control;
    }

    std::string getBufferContent() {
        return this->control.str();
    }
};

} // namespace CaptureStreams
