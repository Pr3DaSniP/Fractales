#include "NebulabrotView.h"

#include "core/NebulabrotGenerator.h"

NebulabrotView::NebulabrotView()
    : m_displayShader("shaders/vertex.vs", "shaders/nebulabrot_display.fs")
{
    glGenTextures(1, &m_texture);
}

NebulabrotView::~NebulabrotView()
{
    if (m_worker.joinable())
    {
        m_worker.join();
    }
    if (m_texture != 0)
    {
        glDeleteTextures(1, &m_texture);
    }
}

void NebulabrotView::startGenerate(
    int width, int height, long long samplesPerChannel,
    int redMaxIter, int greenMaxIter, int blueMaxIter)
{
    if (m_generating.load())
    {
        return; // génération déjà en cours, on ignore la demande
    }

    if (m_worker.joinable())
    {
        m_worker.join();
    }

    m_pendingWidth = width;
    m_pendingHeight = height;
    m_totalSamples = samplesPerChannel * 3;
    m_samplesCompleted.store(0);
    m_resultReady.store(false);
    m_generating.store(true);

    m_worker = std::thread([this, width, height, samplesPerChannel, redMaxIter, greenMaxIter, blueMaxIter]()
    {
        m_pendingPixels = generateNebulabrot(
            width, height, samplesPerChannel,
            redMaxIter, greenMaxIter, blueMaxIter, &m_samplesCompleted);

        m_resultReady.store(true);
        m_generating.store(false);
    });
}

void NebulabrotView::update()
{
    if (!m_resultReady.load())
    {
        return;
    }

    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGB8, m_pendingWidth, m_pendingHeight, 0,
        GL_RGB, GL_UNSIGNED_BYTE, m_pendingPixels.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    m_hasImage = true;
    m_resultReady.store(false);

    if (m_worker.joinable())
    {
        m_worker.join();
    }
}

float NebulabrotView::progress() const
{
    if (m_totalSamples <= 0)
    {
        return 0.0f;
    }
    return static_cast<float>(m_samplesCompleted.load()) / static_cast<float>(m_totalSamples);
}

void NebulabrotView::render(GLuint quadVAO)
{
    if (!m_hasImage)
    {
        return;
    }

    m_displayShader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    m_displayShader.setInt("tex", 0);

    glBindVertexArray(quadVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
}
