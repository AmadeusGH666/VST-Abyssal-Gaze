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
    VisualizerComponent(std::atomic<float>& rmsValue, std::atomic<float>* corruptionVal) 
        : currentRMS(rmsValue), corruptionParam(corruptionVal)
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
        float sensitivity = std::sqrt(smoothedRMS); 
        
        // Dynamic Radius
        float minRadius = juce::jmin(bounds.getWidth(), bounds.getHeight()) * 0.20f; 
        float maxRadius = juce::jmin(bounds.getWidth(), bounds.getHeight()) * 0.55f;
        float currentRadius = minRadius + (maxRadius - minRadius) * sensitivity;
        
        // Color Logic (V0.6): Cold vs Hot based on Corruption
        float corruption = (corruptionParam != nullptr) ? corruptionParam->load() : 0.0f;
        
        // Define Palettes
        // Cold (0.0): Cyan Core -> Blue Mid -> Transparent
        // Hot (1.0): White Core -> Red Mid -> Transparent
        
        juce::Colour coldCore = juce::Colours::cyan;
        juce::Colour coldMid  = juce::Colours::blue;
        juce::Colour coldEdge = juce::Colours::transparentBlack;

        juce::Colour hotCore  = juce::Colours::white;
        juce::Colour hotMid   = juce::Colours::red;
        juce::Colour hotEdge  = juce::Colours::transparentBlack;

        // Interpolate
        juce::Colour coreColor = coldCore.interpolatedWith(hotCore, corruption);
        juce::Colour midColor  = coldMid.interpolatedWith(hotMid, corruption);
        // Edge is always transparent, but maybe we want a slight tint? Let's keep it transparent for fade.
        
        // Gradient Fill
        juce::ColourGradient gradient(
            coreColor,
            center.x, center.y,
            juce::Colours::transparentBlack,
            center.x, center.y - currentRadius,
            true
        );
        
        gradient.addColour(0.0, coreColor);
        gradient.addColour(0.5, midColor);
        gradient.addColour(1.0, juce::Colours::transparentBlack);

        g.setGradientFill(gradient);
        g.fillEllipse(center.x - currentRadius, center.y - currentRadius, currentRadius * 2.0f, currentRadius * 2.0f);
    }

private:
    std::atomic<float>& currentRMS;
    std::atomic<float>* corruptionParam = nullptr;
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
