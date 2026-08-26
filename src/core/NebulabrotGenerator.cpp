#include "NebulabrotGenerator.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <random>
#include <thread>

namespace
{
    constexpr double kRealCenter = -0.5;
    constexpr double kImagHalfHeight = 1.25;

    struct Bounds
    {
        double realMin, realMax, imagMin, imagMax;
    };

    Bounds computeBounds(int width, int height)
    {
        double imagRange = kImagHalfHeight * 2.0;
        double realRange = imagRange * static_cast<double>(width) / static_cast<double>(height);
        return Bounds{
            kRealCenter - realRange / 2.0, kRealCenter + realRange / 2.0,
            -kImagHalfHeight, kImagHalfHeight
        };
    }

    std::vector<uint32_t> computeChannelHistogram(
        int width, int height, long long samples, int maxIter,
        std::atomic<long long>* samplesCompleted)
    {
        Bounds bounds = computeBounds(width, height);
        unsigned int numThreads = std::max(1u, std::thread::hardware_concurrency());
        size_t pixelCount = static_cast<size_t>(width) * static_cast<size_t>(height);

        std::vector<std::vector<uint32_t>> localHistograms(
            numThreads, std::vector<uint32_t>(pixelCount, 0));

        std::vector<std::thread> threads;
        long long samplesPerThread = samples / numThreads;
        constexpr long long kProgressBatchSize = 5000;

        for (unsigned int t = 0; t < numThreads; t++)
        {
            threads.emplace_back([&, t]()
            {
                std::mt19937 rng(std::random_device{}() ^ (t * 2654435761u));
                std::uniform_real_distribution<double> distReal(bounds.realMin, bounds.realMax);
                std::uniform_real_distribution<double> distImag(bounds.imagMin, bounds.imagMax);

                std::vector<uint32_t>& hist = localHistograms[t];

                std::vector<double> orbitX;
                std::vector<double> orbitY;
                orbitX.reserve(static_cast<size_t>(maxIter));
                orbitY.reserve(static_cast<size_t>(maxIter));

                long long sinceLastReport = 0;

                for (long long s = 0; s < samplesPerThread; s++)
                {
                    double cx = distReal(rng);
                    double cy = distImag(rng);

                    double zx = 0.0;
                    double zy = 0.0;
                    orbitX.clear();
                    orbitY.clear();

                    bool escaped = false;
                    int iter = 0;

                    for (; iter < maxIter; iter++)
                    {
                        double newZx = zx * zx - zy * zy + cx;
                        double newZy = 2.0 * zx * zy + cy;
                        zx = newZx;
                        zy = newZy;

                        orbitX.push_back(zx);
                        orbitY.push_back(zy);

                        if (zx * zx + zy * zy > 4.0)
                        {
                            escaped = true;
                            iter++;
                            break;
                        }
                    }

                    if (escaped)
                    {
                        for (int i = 0; i < iter; i++)
                        {
                            int px = static_cast<int>((orbitX[i] - bounds.realMin) / (bounds.realMax - bounds.realMin) * width);
                            int py = static_cast<int>((orbitY[i] - bounds.imagMin) / (bounds.imagMax - bounds.imagMin) * height);

                            if (px >= 0 && px < width && py >= 0 && py < height)
                            {
                                hist[static_cast<size_t>(py) * width + px]++;
                            }
                        }
                    }

                    if (samplesCompleted != nullptr)
                    {
                        sinceLastReport++;
                        if (sinceLastReport >= kProgressBatchSize)
                        {
                            samplesCompleted->fetch_add(sinceLastReport, std::memory_order_relaxed);
                            sinceLastReport = 0;
                        }
                    }
                }

                if (samplesCompleted != nullptr && sinceLastReport > 0)
                {
                    samplesCompleted->fetch_add(sinceLastReport, std::memory_order_relaxed);
                }
            });
        }

        for (std::thread& th : threads)
            th.join();

        std::vector<uint32_t> merged(pixelCount, 0);
        for (const std::vector<uint32_t>& local : localHistograms)
        {
            for (size_t i = 0; i < merged.size(); i++)
                merged[i] += local[i];
        }

        return merged;
    }

    std::vector<unsigned char> normalizeChannel(const std::vector<uint32_t>& histogram)
    {
        uint32_t maxValue = 0;
        for (uint32_t v : histogram)
            maxValue = std::max(maxValue, v);

        std::vector<unsigned char> result(histogram.size(), 0);
        if (maxValue == 0)
            return result;

        constexpr double kContrastExponent = 0.75;
        double maxPow = std::pow(static_cast<double>(maxValue), kContrastExponent);
        for (size_t i = 0; i < histogram.size(); i++)
        {
            double normalized = std::pow(static_cast<double>(histogram[i]), kContrastExponent) / maxPow;
            result[i] = static_cast<unsigned char>(std::clamp(normalized * 255.0, 0.0, 255.0));
        }

        return result;
    }
}

std::vector<unsigned char> generateNebulabrot(
    int width, int height, long long samplesPerChannel,
    int redMaxIter, int greenMaxIter, int blueMaxIter,
    std::atomic<long long>* samplesCompleted)
{
    std::vector<uint32_t> redHist = computeChannelHistogram(width, height, samplesPerChannel, redMaxIter, samplesCompleted);
    std::vector<uint32_t> greenHist = computeChannelHistogram(width, height, samplesPerChannel, greenMaxIter, samplesCompleted);
    std::vector<uint32_t> blueHist = computeChannelHistogram(width, height, samplesPerChannel, blueMaxIter, samplesCompleted);

    std::vector<unsigned char> red = normalizeChannel(redHist);
    std::vector<unsigned char> green = normalizeChannel(greenHist);
    std::vector<unsigned char> blue = normalizeChannel(blueHist);

    std::vector<unsigned char> rgb(static_cast<size_t>(width) * static_cast<size_t>(height) * 3);
    for (size_t i = 0; i < red.size(); i++)
    {
        rgb[i * 3 + 0] = red[i];
        rgb[i * 3 + 1] = green[i];
        rgb[i * 3 + 2] = blue[i];
    }

    return rgb;
}
