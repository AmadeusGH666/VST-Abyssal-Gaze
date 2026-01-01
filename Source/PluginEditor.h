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

struct Particle
{
    float x, y;
    float angle;
    float speed;
    float alpha;
    float size;
};

class VisualizerComponent : public juce::Component, public juce::Timer
{
public:
    VisualizerComponent(std::atomic<float>& rmsValue, std::atomic<float>* corruptionVal) 
        : currentRMS(rmsValue), corruptionParam(corruptionVal)
    {
        startTimerHz(60);
        // Initialize particles
        particles.resize(50); // 50 particles
        for (auto& p : particles) resetParticle(p);
    }

    ~VisualizerComponent() override
    {
        stopTimer();
    }

    void resetParticle(Particle& p)
    {
        auto bounds = getLocalBounds().toFloat();
        auto center = bounds.getCentre();
        p.x = center.x;
        p.y = center.y;
        p.angle = juce::Random::getSystemRandom().nextFloat() * juce::MathConstants<float>::twoPi;
        p.speed = 1.0f + juce::Random::getSystemRandom().nextFloat() * 2.0f;
        p.alpha = 1.0f;
        p.size = 1.0f + juce::Random::getSystemRandom().nextFloat() * 3.0f;
    }

    void timerCallback() override
    {
        // Smooth the RMS value
        float target = currentRMS.load();
        
        // Transient Detection for Shockwave
        if (target > lastRMS + 0.15f) // Threshold for transient
        {
            shockwaveRadius = 0.0f;
            shockwaveAlpha = 1.0f;
        }
        lastRMS = target;
        
        smoothedRMS += (target - smoothedRMS) * 0.1f; // Simple smoothing
        
        // Update Shockwave
        if (shockwaveAlpha > 0.0f)
        {
            shockwaveRadius += 10.0f; // Expand speed
            shockwaveAlpha -= 0.05f;  // Fade speed
            if (shockwaveAlpha < 0.0f) shockwaveAlpha = 0.0f;
        }
        
        // Update Particles
        for (auto& p : particles)
        {
            p.x += std::cos(p.angle) * p.speed * (1.0f + smoothedRMS * 5.0f); // Speed up with volume
            p.y += std::sin(p.angle) * p.speed * (1.0f + smoothedRMS * 5.0f);
            p.alpha -= 0.02f;
            
            if (p.alpha <= 0.0f)
            {
                resetParticle(p);
            }
        }
        
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
        
        juce::Colour coldCore = juce::Colours::cyan;
        juce::Colour coldMid  = juce::Colours::blue;
        
        juce::Colour hotCore  = juce::Colours::white;
        juce::Colour hotMid   = juce::Colours::red;
        
        // Interpolate
        juce::Colour coreColor = coldCore.interpolatedWith(hotCore, corruption);
        juce::Colour midColor  = coldMid.interpolatedWith(hotMid, corruption);
        
        // 1. Draw The Core (EXISTING)
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
        
        // 2. Draw Shockwaves (NEW)
        if (shockwaveAlpha > 0.0f)
        {
            g.setColour(coreColor.withAlpha(shockwaveAlpha));
            g.drawEllipse(center.x - shockwaveRadius, center.y - shockwaveRadius, shockwaveRadius * 2.0f, shockwaveRadius * 2.0f, 4.0f);
        }
        
        // 3. Draw Particles (NEW)
        for (const auto& p : particles)
        {
            g.setColour(midColor.withAlpha(p.alpha));
            g.fillEllipse(p.x - p.size / 2.0f, p.y - p.size / 2.0f, p.size, p.size);
        }
    }

private:
    std::atomic<float>& currentRMS;
    std::atomic<float>* corruptionParam = nullptr;
    float smoothedRMS = 0.0f;
    
    // V0.7 Additions
    std::vector<Particle> particles;
    float shockwaveRadius = 0.0f;
    float shockwaveAlpha = 0.0f;
    float lastRMS = 0.0f;
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
