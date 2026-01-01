/*
  ==============================================================================

    PluginProcessor.h
    Created: 1 Jan 2026
    Author:  Antigravity

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class AbyssalGazeNewAudioProcessor  : public juce::AudioProcessor, public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    AbyssalGazeNewAudioProcessor();
    ~AbyssalGazeNewAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    //==============================================================================
    const juce::String getName() const override { return "Abyssal Gaze NEW"; }

    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    //==============================================================================
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int index) override {}
    const juce::String getProgramName (int index) override { return {}; }
    void changeProgramName (int index, const juce::String& newName) override {}

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    juce::AudioProcessorValueTreeState apvts;

    // Parameter IDs
    static const juce::String id_corruption;
    static const juce::String id_drown;
    static const juce::String id_obscura;
    static const juce::String id_void;
    static const juce::String id_erosion;
    static const juce::String id_whispers;
    static const juce::String id_tremor;
    static const juce::String id_revelation;

    // Audio Metering
    std::atomic<float> currentRMS { 0.0f };

private:
    //==============================================================================
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void parameterChanged (const juce::String& parameterID, float newValue) override;

    void updatePresets(int presetIndex);

    // DSP Objects
    juce::dsp::StateVariableTPTFilter<float> filter; // Obscura
    juce::dsp::Reverb reverb; // VOID
    juce::dsp::Reverb::Parameters reverbParams;
    
    // Delay (Whispers)
    juce::AudioBuffer<float> delayBuffer;
    int delayWritePosition = 0;

    // Tremolo (Tremor)
    float tremoloPhase = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AbyssalGazeNewAudioProcessor)
};
