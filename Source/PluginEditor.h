/*
  ==============================================================================

    PluginEditor.h
    Created: 1 Jan 2026
    Author:  Antigravity

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "AbyssalLookAndFeel.h"

class AbyssalGazeNewAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    AbyssalGazeNewAudioProcessorEditor (AbyssalGazeNewAudioProcessor&);
    ~AbyssalGazeNewAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    AbyssalGazeNewAudioProcessor& audioProcessor;
    
    AbyssalLookAndFeel abyssalLookAndFeel;
    juce::Image backgroundImage;

    // Controls
    juce::ComboBox revelationBox; // Preset Menu
    juce::Slider corruptionSlider;
    juce::Slider drownSlider;
    juce::Slider obscuraSlider;
    juce::Slider voidSlider;
    juce::Slider erosionSlider;
    juce::Slider whispersSlider;
    juce::Slider tremorSlider;

    // Attachments
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    std::unique_ptr<ComboBoxAttachment> revelationAttachment;
    std::unique_ptr<SliderAttachment> corruptionAttachment;
    std::unique_ptr<SliderAttachment> drownAttachment;
    std::unique_ptr<SliderAttachment> obscuraAttachment;
    std::unique_ptr<SliderAttachment> voidAttachment;
    std::unique_ptr<SliderAttachment> erosionAttachment;
    std::unique_ptr<SliderAttachment> whispersAttachment;
    std::unique_ptr<SliderAttachment> tremorAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AbyssalGazeNewAudioProcessorEditor)
};
