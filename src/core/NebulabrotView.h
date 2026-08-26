#pragma once

#include <atomic>
#include <thread>
#include <vector>

#include <glad/gl.h>

#include "core/Shader.h"

class NebulabrotView
{
public:
    NebulabrotView();
    ~NebulabrotView();

    NebulabrotView(const NebulabrotView&) = delete;
    NebulabrotView& operator=(const NebulabrotView&) = delete;

    void startGenerate(int width, int height, long long samplesPerChannel,
                        int redMaxIter, int greenMaxIter, int blueMaxIter);

    void update();

    void render(GLuint quadVAO);

    bool isGenerating() const { return m_generating.load(); }

    float progress() const;

    bool hasImage() const { return m_hasImage; }

private:
    Shader m_displayShader;
    GLuint m_texture = 0;
    bool m_hasImage = false;

    std::thread m_worker;
    std::atomic<bool> m_generating{false};
    std::atomic<bool> m_resultReady{false};
    std::atomic<long long> m_samplesCompleted{0};
    long long m_totalSamples = 1;

    int m_pendingWidth = 0;
    int m_pendingHeight = 0;
    std::vector<unsigned char> m_pendingPixels;
};
