#pragma once

#include <ostream>

class mock_DebugStream : public std::ostream {
public:
    size_t write(uint8_t c) {
        std::cout.put(c);
        return 1;
    }

    size_t write(const uint8_t *buffer, size_t size)
    {
        for (auto i = 0; i<size; ++i)
        {
            write(buffer[i]);
        }   
        return size;
    }

    int available() { return 0; }
    int read() { return -1; }
    int peek() { return -1; }
};
