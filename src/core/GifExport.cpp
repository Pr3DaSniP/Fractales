#include "GifExport.h"

#include <chrono>
#include <cmath>
#include <cstring>
#include <ctime>
#include <filesystem>
#include <iomanip>
#include <sstream>
#include <vector>

#include <gif.h>

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

    bool renderFrameRGBA(Fractale& fractale, GLuint quadVAO, int width, int height, std::vector<unsigned char>& outFlipped)
    {
        GLuint fbo = 0;
        GLuint colorTexture = 0;

        glGenFramebuffers(1, &fbo);
        glGenTextures(1, &colorTexture);

        glBindTexture(GL_TEXTURE_2D, colorTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);

        bool ok = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;

        if (ok)
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

            std::vector<unsigned char> pixels(static_cast<size_t>(width) * height * 4);
            glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

            glViewport(previousViewport[0], previousViewport[1], previousViewport[2], previousViewport[3]);

            int rowSize = width * 4;
            outFlipped.resize(pixels.size());
            for (int y = 0; y < height; y++)
            {
                std::memcpy(
                    outFlipped.data() + static_cast<size_t>(y) * rowSize,
                    pixels.data() + static_cast<size_t>(height - 1 - y) * rowSize,
                    rowSize);
            }
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteTextures(1, &colorTexture);
        glDeleteFramebuffers(1, &fbo);

        return ok;
    }
}

std::string exportZoomGif(
    Fractale& fractale, GLuint quadVAO, int width, int height,
    int frameCount, double zoomStep, int delayMs)
{
    std::filesystem::create_directory("gifs");
    std::string filename = "gifs/zoom_" + currentTimestamp() + ".gif";

    // gif.h attend un délai en centisecondes (1/100e de seconde).
    uint32_t delayCs = static_cast<uint32_t>(delayMs / 10);

    GifWriter writer{};
    if (!GifBegin(&writer, filename.c_str(), static_cast<uint32_t>(width), static_cast<uint32_t>(height), delayCs))
        return {};

    double startZoom = fractale.shader().getDouble("zoom");

    std::vector<unsigned char> frame;
    bool allOk = true;

    for (int i = 0; i < frameCount && allOk; i++)
    {
        fractale.shader().setDouble("zoom", startZoom + static_cast<double>(i) * zoomStep);

        if (!renderFrameRGBA(fractale, quadVAO, width, height, frame))
        {
            allOk = false;
            break;
        }

        GifWriteFrame(&writer, frame.data(), static_cast<uint32_t>(width), static_cast<uint32_t>(height), delayCs);
    }

    GifEnd(&writer);

    fractale.shader().setDouble("zoom", startZoom);

    return allOk ? filename : std::string{};
}

std::string exportParameterGif(
    Fractale& fractale, GLuint quadVAO, int width, int height,
    int frameCount, int delayMs)
{
    std::filesystem::create_directory("gifs");
    std::string filename = "gifs/parameters_" + currentTimestamp() + ".gif";

    uint32_t delayCs = static_cast<uint32_t>(delayMs / 10);

    GifWriter writer{};
    if (!GifBegin(&writer, filename.c_str(), static_cast<uint32_t>(width), static_cast<uint32_t>(height), delayCs))
        return {};

    std::vector<unsigned char> frame;
    bool allOk = true;

    for (int i = 0; i < frameCount && allOk; i++)
    {
        float t = static_cast<float>(i) / static_cast<float>(frameCount);
        fractale.animateParameters(t);

        if (!renderFrameRGBA(fractale, quadVAO, width, height, frame))
        {
            allOk = false;
            break;
        }

        GifWriteFrame(&writer, frame.data(), static_cast<uint32_t>(width), static_cast<uint32_t>(height), delayCs);
    }

    GifEnd(&writer);

    fractale.animateParameters(0.0f);

    return allOk ? filename : std::string{};
}
