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
        // V0.3 Logic: Apply margin to shrink circle
        float margin = 4.0f; // Reduces diameter from 50px -> 42px
        float diameter = juce::jmin(width, height) - (margin * 2.0f);
        float radius = diameter * 0.5f;
        float centerX = x + width * 0.5f;
        float centerY = y + height * 0.5f;
        auto center = juce::Point<float>(centerX, centerY);

        // 1. The "Plug" Logic: Solid Opaque Black Circle
        g.setColour(juce::Colours::black);
        g.fillEllipse(centerX - radius, centerY - radius, diameter, diameter);

        // 2. The Knob Body: Opaque Black (Same as plug, but maybe we want a visual distinction? 
        // User said "Draw the Black Circle using this reduced radius". 
        // So the "Plug" and "Body" are essentially the same circle now, or the Plug fills the hole and Body is smaller?
        // User said: "Knob Diameter... reduces diameter from 50px -> 42px... Draw the Black Circle using this reduced radius".
        // This implies the visual knob IS the 42px circle. The 50px component bounds just center it.
        
        // 3. The Pointer: Blood Red
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        float pointerLength = radius * 0.8f; // Shorter than radius
        float pointerThickness = 2.0f;       // Thinner

        juce::Path p;
        p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        p.applyTransform(juce::AffineTransform::rotation(angle).translated(center));

        g.setColour(juce::Colour(0xFFFF0000)); // Blood Red
        g.fillPath(p);
    }
};
