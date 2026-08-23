#pragma once

#include <string>

#include <glad/gl.h>

class Fractale;

std::string exportZoomGif(
    Fractale& fractale, GLuint quadVAO, int width, int height,
    int frameCount, double zoomStep, int delayMs);

std::string exportParameterGif(
    Fractale& fractale, GLuint quadVAO, int width, int height,
    int frameCount, int delayMs);
