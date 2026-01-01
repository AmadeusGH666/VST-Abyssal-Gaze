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

class VisualizerComponent : public juce::Component, public juce::Timer
{
public:
    VisualizerComponent(std::atomic<float>& rmsValue) : currentRMS(rmsValue)
    {
        startTimerHz(60);
    }

    ~VisualizerComponent() override
    {
        stopTimer();
    }

    void timerCallback() override
    {
        // Smooth the RMS value
        float target = currentRMS.load();
        smoothedRMS += (target - smoothedRMS) * 0.1f; // Simple smoothing
        repaint();
    }

    void paint(juce::Graphics& g) override
    {
        // "Living Pupil" Logic
        // Center: x=417, y=284 (Relative to parent, but this component will be placed at 0,0 of parent? 
        // No, better to place this component covering the hole or the whole window?
        // User said "Center Point: x=417, y=284. Max Bounds: Width=250, Height=180".
        // Let's make the component bounds 250x180 centered at 417,284.
        // So inside paint, center is local center.
        
        auto bounds = getLocalBounds().toFloat();
        auto center = bounds.getCentre();
        
        // Dynamic Radius
        // Base size: 30%
        // Max size: 100%
        float minRadius = juce::jmin(bounds.getWidth(), bounds.getHeight()) * 0.15f; // 30% diameter / 2
        float maxRadius = juce::jmin(bounds.getWidth(), bounds.getHeight()) * 0.5f;
        
        float currentRadius = minRadius + (maxRadius - minRadius) * smoothedRMS;
        
        // Gradient Fill
        juce::ColourGradient gradient(
            juce::Colour(0xFFFF0000), // Center: Bright Blood Red
            center.x, center.y,
            juce::Colours::transparentBlack, // Edge: Transparent
            center.x, center.y - currentRadius, // Radial
            true // Radial
        );
        
        gradient.addColour(0.0, juce::Colour(0xFFFF4444)); // Hot core
        gradient.addColour(0.4, juce::Colour(0xFFFF0000)); // Red body
        gradient.addColour(0.8, juce::Colour(0xFF550000)); // Dark edge
        gradient.addColour(1.0, juce::Colours::transparentBlack); // Fade out

        g.setGradientFill(gradient);
        g.fillEllipse(center.x - currentRadius, center.y - currentRadius, currentRadius * 2.0f, currentRadius * 2.0f);
    }

private:
    std::atomic<float>& currentRMS;
    float smoothedRMS = 0.0f;
};

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
    juce::ImageComponent backgroundComponent;
    
    VisualizerComponent visualizer;

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
