#pragma once

#include <array>
#include <cmath>

/**
 * Band configuration — ported from web/src/audio/bands.ts and types.ts.
 * Every numeric value matches the prototype exactly.
 *
 * Dimension order in gainWeights: warmth=0, bite=1, air=2, body=3, glass=4, velvet=5
 * Unset dimensions default to 0.0f (no contribution).
 */

enum class FilterType { LowShelf, Peaking, HighShelf };

enum class Dimension : int { Warmth = 0, Bite = 1, Air = 2, Body = 3, Glass = 4, Velvet = 5 };

constexpr int NUM_DIMENSIONS = 6;
constexpr int NUM_BANDS = 11;
constexpr float MAX_GAIN_DB = 12.0f;

struct BandConfig
{
    float freq;
    FilterType type;
    float baseQ;
    std::array<float, NUM_DIMENSIONS> gainWeights; // [warmth, bite, air, body, glass, velvet]
};

/**
 * 11-band EQ configuration — matches web/src/audio/bands.ts exactly.
 *
 * Dimension indices:
 *   0 = warmth, 1 = bite, 2 = air, 3 = body, 4 = glass, 5 = velvet
 */
// clang-format off
constexpr std::array<BandConfig, NUM_BANDS> BANDS = {{
    //  freq   type                    Q     warmth  bite    air     body    glass   velvet
    {   60,    FilterType::LowShelf,   0.7f, {{ 0.2f,  0.0f,  0.0f,  0.6f,  0.0f,  0.3f  }} },
    {  150,    FilterType::Peaking,    1.0f, {{ 0.4f,  0.0f,  0.0f,  0.8f,  0.0f,  0.2f  }} },
    {  300,    FilterType::Peaking,    1.2f, {{ 0.9f,  0.0f,  0.0f,  0.3f, -0.3f,  0.3f  }} },
    {  500,    FilterType::Peaking,    1.5f, {{ 0.4f, -0.3f,  0.0f,  0.0f, -0.4f,  0.4f  }} },
    {  800,    FilterType::Peaking,    1.4f, {{ 0.0f,  0.5f,  0.0f,  0.0f,  0.4f, -0.5f  }} },
    { 1500,    FilterType::Peaking,    1.2f, {{-0.3f,  0.8f,  0.0f,  0.0f,  0.3f, -0.6f  }} },
    { 2500,    FilterType::Peaking,    1.0f, {{-0.5f,  0.9f,  0.0f,  0.0f,  0.6f, -0.8f  }} },
    { 4000,    FilterType::Peaking,    0.9f, {{-0.6f,  0.4f,  0.3f,  0.0f,  0.9f, -0.7f  }} },
    { 6500,    FilterType::Peaking,    0.8f, {{-0.4f,  0.0f,  0.6f,  0.0f,  0.6f, -0.5f  }} },
    {10000,    FilterType::HighShelf,  0.7f, {{-0.5f,  0.0f,  0.9f,  0.0f,  0.3f, -0.6f  }} },
    {14000,    FilterType::Peaking,    0.6f, {{ 0.0f,  0.0f,  0.7f,  0.0f,  0.2f, -0.3f  }} },
}};
// clang-format on

/**
 * Calculate band gain in dB from dimension values.
 * Formula: weighted sum of dimensions, tanh soft-clipped to +-MAX_GAIN_DB.
 *
 * This is a free function so tests can call it directly.
 * Matches web/src/audio/AudioEngine.ts applyState() exactly.
 *
 * @param band     The band configuration with gain weights
 * @param dims     Dimension values, each -100 to +100
 * @return         Gain in dB, soft-clipped to [-MAX_GAIN_DB, +MAX_GAIN_DB]
 */
inline float calculateBandGainDb (const BandConfig& band, const std::array<float, NUM_DIMENSIONS>& dims)
{
    float g = 0.0f;
    for (int d = 0; d < NUM_DIMENSIONS; ++d)
    {
        g += (dims[static_cast<size_t> (d)] / 100.0f) * band.gainWeights[static_cast<size_t> (d)] * MAX_GAIN_DB;
    }
    return std::tanh (g / MAX_GAIN_DB) * MAX_GAIN_DB;
}
