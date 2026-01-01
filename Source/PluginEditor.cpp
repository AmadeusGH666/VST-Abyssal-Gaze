/*
  ==============================================================================

    PluginEditor.cpp
    Created: 1 Jan 2026
    Author:  Antigravity

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AbyssalGazeNewAudioProcessorEditor::AbyssalGazeNewAudioProcessorEditor (AbyssalGazeNewAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), 
      visualizer(p.currentRMS, p.apvts.getRawParameterValue(AbyssalGazeNewAudioProcessor::id_corruption))
{
    // 1. Load Asset
    juce::File bgFile("d:/AI/antigravity/VST 20260101/background3transeye.png");
    juce::Image bgImage;
    if (bgFile.existsAsFile())
    {
        bgImage = juce::ImageFileFormat::loadFrom(bgFile);
    }
    backgroundComponent.setImage(bgImage);
    backgroundComponent.setImagePlacement(juce::RectanglePlacement::stretchToFit);
    backgroundComponent.setInterceptsMouseClicks(false, false);

    // 2. Setup LookAndFeel
    juce::LookAndFeel::setDefaultLookAndFeel(&abyssalLookAndFeel);

    // 3. Add Components (Layering Order: Bottom -> Top)
    
    // Layer 1: Visualizer
    addAndMakeVisible(visualizer);

    // Layer 2: Background Image (Overlay)
    addAndMakeVisible(backgroundComponent);

    // Layer 3: Controls
    auto setupSlider = [&](juce::Slider& slider, const juce::String& id) {
        slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        addAndMakeVisible(slider);
    };

    setupSlider(corruptionSlider, AbyssalGazeNewAudioProcessor::id_corruption);
    setupSlider(drownSlider,      AbyssalGazeNewAudioProcessor::id_drown);
    setupSlider(obscuraSlider,    AbyssalGazeNewAudioProcessor::id_obscura);
    setupSlider(voidSlider,       AbyssalGazeNewAudioProcessor::id_void);
    setupSlider(erosionSlider,    AbyssalGazeNewAudioProcessor::id_erosion);
    setupSlider(whispersSlider,   AbyssalGazeNewAudioProcessor::id_whispers);
    setupSlider(tremorSlider,     AbyssalGazeNewAudioProcessor::id_tremor);

    // Revelation (Menu)
    revelationBox.setJustificationType(juce::Justification::centred);
    revelationBox.setColour(juce::ComboBox::backgroundColourId, juce::Colours::transparentBlack);
    revelationBox.setColour(juce::ComboBox::textColourId, juce::Colours::red);
    revelationBox.setColour(juce::ComboBox::arrowColourId, juce::Colours::red);
    revelationBox.setColour(juce::ComboBox::outlineColourId, juce::Colours::transparentBlack);
    
    // Populate menu items from APVTS parameter to ensure sync
    auto* param = audioProcessor.apvts.getParameter(AbyssalGazeNewAudioProcessor::id_revelation);
    if (auto* choiceParam = dynamic_cast<juce::AudioParameterChoice*>(param))
    {
        revelationBox.addItemList(choiceParam->choices, 1);
    }
    
    addAndMakeVisible(revelationBox);

    // 4. Attachments
    corruptionAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, AbyssalGazeNewAudioProcessor::id_corruption, corruptionSlider);
    drownAttachment      = std::make_unique<SliderAttachment>(audioProcessor.apvts, AbyssalGazeNewAudioProcessor::id_drown,      drownSlider);
    obscuraAttachment    = std::make_unique<SliderAttachment>(audioProcessor.apvts, AbyssalGazeNewAudioProcessor::id_obscura,    obscuraSlider);
    voidAttachment       = std::make_unique<SliderAttachment>(audioProcessor.apvts, AbyssalGazeNewAudioProcessor::id_void,       voidSlider);
    erosionAttachment    = std::make_unique<SliderAttachment>(audioProcessor.apvts, AbyssalGazeNewAudioProcessor::id_erosion,    erosionSlider);
    whispersAttachment   = std::make_unique<SliderAttachment>(audioProcessor.apvts, AbyssalGazeNewAudioProcessor::id_whispers,   whispersSlider);
    tremorAttachment     = std::make_unique<SliderAttachment>(audioProcessor.apvts, AbyssalGazeNewAudioProcessor::id_tremor,     tremorSlider);
    
    revelationAttachment = std::make_unique<ComboBoxAttachment>(audioProcessor.apvts, AbyssalGazeNewAudioProcessor::id_revelation, revelationBox);

    // 5. Fixed Size
    setSize (800, 600);
}

AbyssalGazeNewAudioProcessorEditor::~AbyssalGazeNewAudioProcessorEditor()
{
    juce::LookAndFeel::setDefaultLookAndFeel(nullptr);
}

void AbyssalGazeNewAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Layer 0: Background Fill (Black)
    g.setColour(juce::Colours::black);
    g.fillAll();
    
    // Layer 1: Visualizer (Handled by VisualizerComponent)
    // Layer 2: Background Image (Handled by backgroundComponent)
    // Layer 3: Controls (Handled by child components)
    
    // Draw Black Plug for Revelation Menu (since it's transparent now)
    // Center: x=218, y=291. Diameter: 42px (Radius 21px).
    g.setColour(juce::Colours::black);
    g.fillEllipse(218 - 21, 291 - 21, 42, 42);
}

void AbyssalGazeNewAudioProcessorEditor::resized()
{
    // Exact Layout Logic (V0.2)
    // Component Size: 50 x 50 px
    // Coordinates are CENTER POINTS (cx, cy). Top-left = cx - 25, cy - 25.
    
    auto setBoundsCenter = [&](juce::Component& comp, int cx, int cy) {
        comp.setBounds(cx - 25, cy - 25, 50, 50);
    };

    // 1. Revelation (Menu - Left) Center: x=218, y=291
    setBoundsCenter(revelationBox, 218, 291);

    // 2. Corruption (Top-Left) Center: x=274, y=182
    setBoundsCenter(corruptionSlider, 274, 182);

    // 3. Drown (Top) Center: x=420, y=122
    setBoundsCenter(drownSlider, 420, 122);

    // 4. Obscura (Top-Right) Center: x=565, y=174
    setBoundsCenter(obscuraSlider, 565, 174);

    // 5. VOID (Right) Center: x=620, y=290
    setBoundsCenter(voidSlider, 620, 290);

    // 6. Erosion (Bottom-Right) Center: x=561, y=397
    setBoundsCenter(erosionSlider, 561, 397);

    // 7. Whispers (Bottom) Center: x=420, y=437
    setBoundsCenter(whispersSlider, 420, 437);

    // 8. Tremor (Bottom-Left) Center: x=280, y=397
    setBoundsCenter(tremorSlider, 280, 397);

    // Visualizer Bounds
    // Center: x=417, y=284. Max Bounds: Width=250, Height=180.
    // Top-Left = (417 - 125, 284 - 90) = (292, 194)
    visualizer.setBounds(292, 194, 250, 180);

    // Background Image Bounds
    backgroundComponent.setBounds(0, 0, 800, 600);
}
