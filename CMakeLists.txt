cmake_minimum_required(VERSION 3.25)

# Pamplejuce cmake helpers
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(PamplejuceVersion)
include(CPM)

# macOS deployment target — Monterey minimum per CONTEXT.md
set(CMAKE_OSX_DEPLOYMENT_TARGET "12.0" CACHE STRING "macOS Monterey minimum" FORCE)

# Universal Binary — always build both architectures
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "Universal Binary" FORCE)

# Xcode scheme defaults
set(CMAKE_XCODE_GENERATE_SCHEME OFF)

# JUCE defaults
include(JUCEDefaults)

# Project identity
set(PROJECT_NAME "Toneword")
set(PRODUCT_NAME "Toneword")
set(COMPANY_NAME "Manzanita Research")
set(BUNDLE_ID "com.manzanita-research.toneword")
set(FORMATS Standalone AU VST3)

# Force JUCE to work with sccache
set(CMAKE_POLICY_DEFAULT_CMP0141        NEW      CACHE STRING "" FORCE)
set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT Embedded CACHE STRING "" FORCE)

project(${PROJECT_NAME} VERSION ${CURRENT_VERSION})

# JUCE as submodule
add_subdirectory(JUCE)

# Plugin definition
juce_add_plugin("${PROJECT_NAME}"
    ICON_BIG "${CMAKE_CURRENT_SOURCE_DIR}/packaging/icon.png"
    COMPANY_NAME "${COMPANY_NAME}"
    BUNDLE_ID "${BUNDLE_ID}"
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE Manz
    PLUGIN_CODE Tnwd
    FORMATS "${FORMATS}"
    PRODUCT_NAME "${PRODUCT_NAME}"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    VST3_CATEGORIES Fx EQ
    AU_MAIN_TYPE kAudioUnitType_Effect
    DESCRIPTION "Semantic EQ")

# Shared code library for plugin + tests
add_library(SharedCode INTERFACE)

include(SharedCodeDefaults)

# Source files
file(GLOB_RECURSE SourceFiles CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/*.h")
target_sources(SharedCode INTERFACE ${SourceFiles})

# Assets
include(Assets)

# Xcode organization
include(XcodePrettify)

# Preprocessor definitions
target_compile_definitions(SharedCode
    INTERFACE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    CMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE}"
    VERSION="${CURRENT_VERSION}"
    PRODUCT_NAME_WITHOUT_VERSION="Toneword")

# Link JUCE modules
target_link_libraries(SharedCode
    INTERFACE
    Assets
    juce_audio_utils
    juce_audio_processors
    juce_dsp
    juce_gui_basics
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)

target_link_libraries("${PROJECT_NAME}" PRIVATE SharedCode)

# Tests (Catch2)
include(Tests)

# CI config output
include(GitHubENV)
