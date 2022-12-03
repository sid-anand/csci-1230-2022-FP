#pragma once
#include <GL/glew.h>
#include <iostream>

namespace Debug
{
    inline void glErrorCheck(const char *filename, int line) {
        GLenum errorNumber = glGetError();
        while (errorNumber != GL_NO_ERROR) {
            std::string buf(filename);
            buf.append(" " + std::to_string(line) + ": " + std::to_string(errorNumber));
            std::cout << buf << std::endl;
            errorNumber = glGetError();
        }
    }
}

#define glErrorCheck() glErrorCheck(__FILE__, __LINE__)
