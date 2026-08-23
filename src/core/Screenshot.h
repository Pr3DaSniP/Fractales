#pragma once

#include <string>

#include <glad/gl.h>

class Fractale;

std::string exportFractalToPNG(Fractale& fractale, GLuint quadVAO, int width, int height);
