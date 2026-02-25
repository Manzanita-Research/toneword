#pragma once

#include <cmath>

/**
 * Shared helpers for visualization panels.
 *
 * freqToX maps a frequency (20Hz-20kHz) to a horizontal pixel position
 * on a log scale. Matches the web prototype's mapping exactly.
 */

namespace GUIHelpers
{

/** Map frequency to x-coordinate on log scale (20Hz = 0, 20kHz = width). */
inline float freqToX (float freq, float width)
{
    return (std::log10 (freq / 20.0f) / std::log10 (1000.0f)) * width;
}

} // namespace GUIHelpers
