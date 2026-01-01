/*
  ==============================================================================

    AbyssalLookAndFeel.h
    Created: 1 Jan 2026
    Author:  Antigravity

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class AbyssalLookAndFeel : public juce::LookAndFeel_V4
{
public:
    AbyssalLookAndFeel()
    {
        setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::red);
        setColour(juce::Slider::thumbColourId, juce::Colours::red);
    }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override
    {
        auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat();
        auto center = bounds.getCentre();
        auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;

        // 1. The "Plug" Logic: Solid Opaque Black Circle
        g.setColour(juce::Colours::black);
        g.fillEllipse(center.x - radius, center.y - radius, radius * 2.0f, radius * 2.0f);

        // 2. The Knob Body: Opaque Black
        float knobRadius = radius * 0.76f; // 38px / 50px = 0.76
        g.setColour(juce::Colours::black);
        g.fillEllipse(center.x - knobRadius, center.y - knobRadius, knobRadius * 2.0f, knobRadius * 2.0f);

        // 3. The Pointer: Blood Red
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        float pointerLength = knobRadius * 0.8f;
        float pointerThickness = 4.0f;

        juce::Path p;
        p.addRectangle(-pointerThickness * 0.5f, -knobRadius, pointerThickness, pointerLength);
        p.applyTransform(juce::AffineTransform::rotation(angle).translated(center));

        g.setColour(juce::Colour(0xFFFF0000)); // Blood Red
        g.fillPath(p);
    }
};
