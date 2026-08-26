#pragma once

#include <atomic>
#include <vector>

std::vector<unsigned char> generateNebulabrot(
    int width, int height, long long samplesPerChannel,
    int redMaxIter, int greenMaxIter, int blueMaxIter,
    std::atomic<long long>* samplesCompleted = nullptr);
