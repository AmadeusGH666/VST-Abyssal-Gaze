cmake_minimum_required(VERSION 3.15)

project(AbyssalGazeNew VERSION 0.0.1)

# Add JUCE
# Using FetchContent to get JUCE. You can also point this to your local JUCE installation.
include(FetchContent)
FetchContent_Declare(
    juce
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(juce)

# Create the plugin
juce_add_plugin(AbyssalGazeNew
    COMPANY_NAME "Antigravity"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE Juce
    PLUGIN_CODE Abys
    FORMATS VST3 Standalone
    PRODUCT_NAME "Abyssal Gaze NEW"
)

# Add source files
target_sources(AbyssalGazeNew PRIVATE
    Source/AbyssalLookAndFeel.h
    Source/PluginProcessor.h
    Source/PluginProcessor.cpp
    Source/PluginEditor.h
    Source/PluginEditor.cpp
)

# Link to JUCE libraries
target_link_libraries(AbyssalGazeNew PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_processors
    juce::juce_core
    juce::juce_data_structures
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
    juce::juce_dsp
)

# Generate JuceHeader.h
juce_generate_juce_header(AbyssalGazeNew)

# Compile definitions
target_compile_definitions(AbyssalGazeNew PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

# Standard C++ version
target_compile_features(AbyssalGazeNew PUBLIC cxx_std_17)
