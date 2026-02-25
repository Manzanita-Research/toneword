#pragma once
#include <PluginProcessor.h>

[[maybe_unused]] static void runWithinPluginEditor (const std::function<void (TonewordAudioProcessor& plugin)>& testCode)
{
    TonewordAudioProcessor plugin;
    const auto editor = plugin.createEditorIfNeeded();

    testCode (plugin);

    plugin.editorBeingDeleted (editor);
    delete editor;
}
