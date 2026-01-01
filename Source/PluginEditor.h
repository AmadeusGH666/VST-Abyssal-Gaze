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
        auto bounds = getLocalBounds().toFloat();
        auto center = bounds.getCentre();
        
        // Enhanced Dynamics (V0.5)
        // Use sqrt to make it more sensitive to lower volumes
        float sensitivity = std::sqrt(smoothedRMS); 
        
        // Dynamic Radius
        // Base size: 40% (Larger resting state)
        // Max size: 110% (Explodes slightly beyond bounds for impact)
        float minRadius = juce::jmin(bounds.getWidth(), bounds.getHeight()) * 0.20f; 
        float maxRadius = juce::jmin(bounds.getWidth(), bounds.getHeight()) * 0.55f;
        
        float currentRadius = minRadius + (maxRadius - minRadius) * sensitivity;
        
        // Richer Gradient Fill (V0.5)
        // White Core -> Orange -> Blood Red -> Deep Purple -> Transparent
        juce::ColourGradient gradient(
            juce::Colour(0xFFFFFFCC), // Center: Intense White-Yellow
            center.x, center.y,
            juce::Colours::transparentBlack, // Edge: Transparent
            center.x, center.y - currentRadius, // Radial
            true // Radial
        );
        
        gradient.addColour(0.0, juce::Colour(0xFFFFFFCC)); // Core
        gradient.addColour(0.2, juce::Colour(0xFFFF8800)); // Inner: Bright Orange
        gradient.addColour(0.5, juce::Colour(0xFFFF0000)); // Mid: Blood Red
        gradient.addColour(0.8, juce::Colour(0xFF330011)); // Outer: Deep Abyssal Purple
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
