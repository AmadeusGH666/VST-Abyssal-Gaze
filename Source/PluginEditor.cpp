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
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // 1. Load Asset
    // Using absolute path as found in the workspace. In a real project, this should be a BinaryResource.
    // For now, we load from file.
    juce::File bgFile("d:/AI/antigravity/VST 20260101/background3transeye.png");
    if (bgFile.existsAsFile())
    {
        backgroundImage = juce::ImageFileFormat::loadFrom(bgFile);
    }

    // 2. Setup LookAndFeel
    juce::LookAndFeel::setDefaultLookAndFeel(&abyssalLookAndFeel);

    // 3. Setup Controls
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
    revelationBox.setColour(juce::ComboBox::backgroundColourId, juce::Colours::black);
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
    // Layer 1: Visualizer (Placeholder for now, centered at 400, 300)
    g.setColour(juce::Colours::black);
    g.fillAll();
    
    g.setColour(juce::Colours::darkred);
    g.fillEllipse(350, 250, 100, 100); // Simple "Blood Red" center

    // Layer 2: Background Image
    if (backgroundImage.isValid())
    {
        // Scale to fit 800x600
        g.drawImage(backgroundImage, getLocalBounds().toFloat());
    }
    else
    {
        g.setColour(juce::Colours::red);
        g.drawText("Background Image Not Found", getLocalBounds(), juce::Justification::centred);
    }

    // Layer 3: Controls are drawn by themselves on top
}

void AbyssalGazeNewAudioProcessorEditor::resized()
{
    // Coordinate System: Positions are relative percentages (0.0 - 1.0).
    // Component Size: 50 x 50 px
    
    auto setBoundsRel = [&](juce::Component& comp, float xRel, float yRel) {
        int w = 50;
        int h = 50;
        int x = (int)(xRel * 800.0f) - w / 2;
        int y = (int)(yRel * 600.0f) - h / 2;
        comp.setBounds(x, y, w, h);
    };

    // 1. Revelation (Left Eye @ 9:00)
    // Pos: x=0.12, y=0.50
    // Note: ComboBox might need to be wider than 50px to show text?
    // User said "Component Size: Set Component bounds to 50 x 50 px (Safety margin)."
    // But for a menu, 50px is small. However, "The Plug Logic" implies it fills the hole.
    // I will stick to 50x50 as requested, maybe the text will be small or clipped, or it's just a click target.
    // Actually, for a ComboBox, it usually draws a box.
    // Let's stick to the requested bounds.
    setBoundsRel(revelationBox, 0.12f, 0.50f);

    // 2. Corruption (Top-Left @ 10:30) Pos: x=0.23, y=0.26.
    setBoundsRel(corruptionSlider, 0.23f, 0.26f);

    // 3. Drown (Top @ 12:00) Pos: x=0.50, y=0.16.
    setBoundsRel(drownSlider, 0.50f, 0.16f);

    // 4. Obscura (Top-Right @ 1:30) Pos: x=0.77, y=0.26.
    setBoundsRel(obscuraSlider, 0.77f, 0.26f);

    // 5. VOID (Right @ 3:00) Pos: x=0.88, y=0.50.
    setBoundsRel(voidSlider, 0.88f, 0.50f);

    // 6. Erosion (Bottom-Right @ 4:30) Pos: x=0.77, y=0.74.
    setBoundsRel(erosionSlider, 0.77f, 0.74f);

    // 7. Whispers (Bottom @ 6:00) Pos: x=0.50, y=0.84.
    setBoundsRel(whispersSlider, 0.50f, 0.84f);

    // 8. Tremor (Bottom-Left @ 7:30) Pos: x=0.23, y=0.74.
    setBoundsRel(tremorSlider, 0.23f, 0.74f);
}
