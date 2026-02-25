#pragma once

#include "../DSP/BandConfig.h"
#include <array>

/**
 * Factory preset data — 9 presets, from Init (all zeros) to named tone recipes.
 *
 * Value order: warmth, bite, air, body, glass, velvet (matches Dimension enum).
 * Each value is in the -100 to +100 range.
 */

struct PresetData
{
    const char* name;
    std::array<float, NUM_DIMENSIONS> values; // [warmth, bite, air, body, glass, velvet]
};

constexpr int NUM_FACTORY_PRESETS = 9;

// clang-format off
constexpr std::array<PresetData, NUM_FACTORY_PRESETS> FACTORY_PRESETS = {{
    //                                          warmth  bite    air     body    glass   velvet
    { "Init",              {{  0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f  }} },

    // Presets 1-3: ported from web prototype (tuned by ear)
    { "Nashville Twang",   {{ -20.0f,  60.0f,  40.0f, -10.0f,  50.0f, -40.0f }} },
    { "Midnight Jazz",     {{  50.0f, -60.0f, -30.0f,  40.0f, -40.0f,  80.0f }} },
    { "Punk Scoop",        {{ -30.0f,  80.0f, -10.0f,  50.0f, -20.0f, -60.0f }} },

    // Presets 4-8: reasonable starting points — re-tune by ear before release
    { "Bedroom Producer",  {{  30.0f, -20.0f, -40.0f,  15.0f, -30.0f,  45.0f }} },
    { "Blues Bark",         {{  35.0f,  55.0f,  10.0f,  45.0f, -15.0f, -25.0f }} },
    { "Shoegaze Wash",     {{  40.0f, -50.0f,  50.0f,  20.0f, -35.0f,  70.0f }} },
    { "Country Sparkle",   {{ -15.0f,  30.0f,  55.0f,  -5.0f,  60.0f, -50.0f }} },
    { "Neo Soul Cream",    {{  45.0f, -35.0f, -15.0f,  35.0f, -25.0f,  65.0f }} },
}};
// clang-format on
