#include "Screenshot.h"

#include <chrono>
#include <cstring>
#include <ctime>
#include <filesystem>
#include <iomanip>
#include <sstream>
#include <vector>

#include <stb_image_write.h>

#include "core/Fractale.h"

namespace
{
    std::string currentTimestamp()
    {
        auto now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(now);

        std::tm localTime{};
#ifdef _WIN32
        localtime_s(&localTime, &time);
#else
        localtime_r(&time, &localTime);
#endif

        std::ostringstream oss;
        oss << std::put_time(&localTime, "%Y%m%d_%H%M%S");
        return oss.str();
    }
}

std::string exportFractalToPNG(Fractale& fractale, GLuint quadVAO, int width, int height)
{
    GLuint fbo = 0;
    GLuint colorTexture = 0;

    glGenFramebuffers(1, &fbo);
    glGenTextures(1, &colorTexture);

    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);

    bool framebufferOk = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;

    std::string resultPath;

    if (framebufferOk)
    {
        GLint previousViewport[4];
        glGetIntegerv(GL_VIEWPORT, previousViewport);
        glViewport(0, 0, width, height);

        fractale.shader().setInt("width", width);
        fractale.shader().setInt("height", height);
        fractale.render();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(quadVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        std::vector<unsigned char> pixels(static_cast<size_t>(width) * height * 3);
        glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

        glViewport(previousViewport[0], previousViewport[1], previousViewport[2], previousViewport[3]);

        int rowSize = width * 3;
        std::vector<unsigned char> flipped(pixels.size());
        for (int y = 0; y < height; y++)
        {
            std::memcpy(
                flipped.data() + static_cast<size_t>(y) * rowSize,
                pixels.data() + static_cast<size_t>(height - 1 - y) * rowSize,
                rowSize);
        }

        std::filesystem::create_directory("screenshots");
        std::string filename = "screenshots/fractale_" + currentTimestamp() + ".png";

        if (stbi_write_png(filename.c_str(), width, height, 3, flipped.data(), rowSize))
        {
            resultPath = filename;
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteTextures(1, &colorTexture);
    glDeleteFramebuffers(1, &fbo);

    return resultPath;
}
