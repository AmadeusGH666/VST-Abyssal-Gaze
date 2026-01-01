/*
  ==============================================================================

    PluginProcessor.cpp
    Created: 1 Jan 2026
    Author:  Antigravity

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
const juce::String AbyssalGazeNewAudioProcessor::id_corruption = "corruption";
const juce::String AbyssalGazeNewAudioProcessor::id_drown      = "drown";
const juce::String AbyssalGazeNewAudioProcessor::id_obscura    = "obscura";
const juce::String AbyssalGazeNewAudioProcessor::id_void       = "void";
const juce::String AbyssalGazeNewAudioProcessor::id_erosion    = "erosion";
const juce::String AbyssalGazeNewAudioProcessor::id_whispers   = "whispers";
const juce::String AbyssalGazeNewAudioProcessor::id_tremor     = "tremor";
const juce::String AbyssalGazeNewAudioProcessor::id_revelation = "revelation";

// Preset Data Table
struct PresetData {
    int corruption;
    int drown;
    int obscura;
    int voidVal;
    int erosion;
    int whispers;
    int tremor;
};

const PresetData presets[] = {
    {4, 8, 2, 10, 2, 6, 2}, // 01. Void Walker
    {9, 5, 8, 1, 10, 1, 9}, // 02. Neural Snap
    {8, 3, 4, 2, 5, 3, 4},  // 03. Steam Hammer
    {2, 9, 1, 10, 3, 9, 1}, // 04. Black Hole
    {3, 4, 2, 3, 1, 2, 3},  // 05. Heartbeat
    {7, 4, 5, 3, 8, 4, 5},  // 06. Rusty Gears
    {3, 6, 4, 8, 4, 7, 3},  // 07. Ghost Pipe
    {8, 6, 4, 2, 7, 2, 7},  // 08. Engine Fail
    {5, 7, 2, 8, 2, 5, 2},  // 09. Dark Mass
    {6, 6, 5, 7, 4, 5, 4}   // 10. Abyssal Gaze
};

//==============================================================================
AbyssalGazeNewAudioProcessor::AbyssalGazeNewAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                     .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
#endif
       apvts(*this, nullptr, "Parameters", createParameterLayout())
{
    apvts.addParameterListener(id_revelation, this);
}

AbyssalGazeNewAudioProcessor::~AbyssalGazeNewAudioProcessor()
{
    apvts.removeParameterListener(id_revelation, this);
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout AbyssalGazeNewAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(id_corruption, "Corruption", 0.0f, 1.0f, 0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(id_drown,      "Drown",      0.0f, 1.0f, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(id_obscura,    "Obscura",    0.0f, 1.0f, 1.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(id_void,       "VOID",       0.0f, 1.0f, 0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(id_erosion,    "Erosion",    0.0f, 1.0f, 0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(id_whispers,   "Whispers",   0.0f, 1.0f, 0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(id_tremor,     "Tremor",     0.0f, 1.0f, 0.0f));

    juce::StringArray presetNames;
    presetNames.add("Void Walker");
    presetNames.add("Neural Snap");
    presetNames.add("Steam Hammer");
    presetNames.add("Black Hole");
    presetNames.add("Heartbeat");
    presetNames.add("Rusty Gears");
    presetNames.add("Ghost Pipe");
    presetNames.add("Engine Fail");
    presetNames.add("Dark Mass");
    presetNames.add("Abyssal Gaze");

    layout.add(std::make_unique<juce::AudioParameterChoice>(id_revelation, "Revelation", presetNames, 0));

    return layout;
}

void AbyssalGazeNewAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    if (parameterID == id_revelation)
    {
        updatePresets((int)newValue);
    }
}

void AbyssalGazeNewAudioProcessor::updatePresets(int presetIndex)
{
    if (presetIndex < 0 || presetIndex >= 10) return;

    const auto& data = presets[presetIndex];

    // Map 1-10 to 0.0-1.0
    auto mapVal = [](int val) { return (float)(val - 1) / 9.0f; };

    // We need to set the parameters on the message thread or via a safe mechanism if this is called from audio thread.
    // parameterChanged can be called from audio thread. However, setValueNotifyingHost is not safe on audio thread usually.
    // But since this is a VST3, the host might be controlling it.
    // Ideally we update the APVTS parameters.
    
    // CAUTION: Setting parameters inside parameterChanged can cause feedback loops if not careful.
    // But here we are setting OTHER parameters based on one.
    
    // We will use callAsync to update parameters on the message thread to be safe and update UI.
    juce::MessageManager::callAsync([this, data, mapVal]()
    {
        auto setParam = [&](const juce::String& id, int val) {
            auto* param = apvts.getParameter(id);
            if (param) param->setValueNotifyingHost(mapVal(val));
        };

        setParam(id_corruption, data.corruption);
        setParam(id_drown,      data.drown);
        setParam(id_obscura,    data.obscura);
        setParam(id_void,       data.voidVal);
        setParam(id_erosion,    data.erosion);
        setParam(id_whispers,   data.whispers);
        setParam(id_tremor,     data.tremor);
    });
}

//==============================================================================
void AbyssalGazeNewAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    filter.prepare(spec);
    filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    
    reverb.prepare(spec);
    reverbParams.roomSize = 0.5f;
    reverbParams.damping = 0.5f;
    reverb.setParameters(reverbParams);

    delayBuffer.setSize(getTotalNumOutputChannels(), (int)(2.0 * sampleRate)); // 2 seconds max delay
    delayBuffer.clear();
    delayWritePosition = 0;

    tremoloPhase = 0.0f;
}

void AbyssalGazeNewAudioProcessor::releaseResources()
{
}

bool AbyssalGazeNewAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

void AbyssalGazeNewAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Get Parameters
    float corruptionVal = *apvts.getRawParameterValue(id_corruption); // Distortion
    float drownVal      = *apvts.getRawParameterValue(id_drown);      // Dry/Wet
    float obscuraVal    = *apvts.getRawParameterValue(id_obscura);    // Filter Cutoff (1=Bright/Open, 0=Dark/Closed)
    float voidVal       = *apvts.getRawParameterValue(id_void);       // Reverb Size
    float erosionVal    = *apvts.getRawParameterValue(id_erosion);    // Bitcrush
    float whispersVal   = *apvts.getRawParameterValue(id_whispers);   // Delay Feedback
    float tremorVal     = *apvts.getRawParameterValue(id_tremor);     // Tremolo Rate

    // Chain: Input -> [Corruption] -> [Obscura] -> [Erosion] -> [Tremor] -> [Whispers] -> [VOID] -> [Drown] -> Output
    
    // We need a dry buffer for the final mix
    juce::AudioBuffer<float> dryBuffer;
    dryBuffer.makeCopyOf(buffer);

    auto* channelDataL = buffer.getWritePointer(0);
    auto* channelDataR = totalNumOutputChannels > 1 ? buffer.getWritePointer(1) : nullptr;
    int numSamples = buffer.getNumSamples();
    double sampleRate = getSampleRate();

    // 1. Corruption (Distortion)
    // Simple hard clipping / tanh
    if (corruptionVal > 0.0f)
    {
        float drive = 1.0f + corruptionVal * 10.0f;
        for (int i = 0; i < numSamples; ++i)
        {
            channelDataL[i] = std::tanh(channelDataL[i] * drive);
            if (channelDataR) channelDataR[i] = std::tanh(channelDataR[i] * drive);
        }
    }

    // 2. Obscura (Filter)
    // Map 0.0-1.0 to 20Hz-20000Hz
    // User req: 1 = Open/Bright, 0 = Closed/Dark. So 1.0 -> 20kHz, 0.0 -> 20Hz
    float cutoff = 20.0f * std::pow(1000.0f, obscuraVal); 
    filter.setCutoffFrequency(cutoff);
    
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    filter.process(context);

    // 3. Erosion (Bitcrush)
    if (erosionVal > 0.0f)
    {
        float crush = 1.0f - erosionVal * 0.9f; // 1.0 to 0.1
        // Simple quantization
        float steps = 4.0f + (1.0f - erosionVal) * 60.0f; // 4 to 64 steps
        for (int i = 0; i < numSamples; ++i)
        {
            channelDataL[i] = std::round(channelDataL[i] * steps) / steps;
            if (channelDataR) channelDataR[i] = std::round(channelDataR[i] * steps) / steps;
        }
    }

    // 4. Tremor (Tremolo)
    if (tremorVal > 0.0f)
    {
        float rate = 0.5f + tremorVal * 10.0f; // 0.5Hz to 10.5Hz
        float phaseInc = (float)(rate * 2.0 * juce::MathConstants<double>::pi / sampleRate);
        
        for (int i = 0; i < numSamples; ++i)
        {
            float mod = 0.5f + 0.5f * std::sin(tremoloPhase);
            tremoloPhase += phaseInc;
            if (tremoloPhase > 2.0f * juce::MathConstants<float>::pi) tremoloPhase -= 2.0f * juce::MathConstants<float>::pi;
            
            // Mix tremolo based on intensity? User just said "Tremolo Rate". 
            // Usually tremolo has depth too. We'll assume full depth or scale depth with rate? 
            // Let's just apply it.
            channelDataL[i] *= mod;
            if (channelDataR) channelDataR[i] *= mod;
        }
    }

    // 5. Whispers (Delay)
    // Simple feedback delay
    if (whispersVal > 0.0f)
    {
        int delaySamples = (int)(0.5 * sampleRate); // Fixed 500ms delay time for "Whispers"
        float feedback = whispersVal * 0.9f; // Up to 90% feedback

        const float* delayL = delayBuffer.getReadPointer(0);
        const float* delayR = delayBuffer.getReadPointer(1); // Assuming stereo delay buffer
        float* delayWriteL = delayBuffer.getWritePointer(0);
        float* delayWriteR = delayBuffer.getWritePointer(1);
        int delayLen = delayBuffer.getNumSamples();

        for (int i = 0; i < numSamples; ++i)
        {
            int readPos = (delayWritePosition - delaySamples + delayLen) % delayLen;
            
            float inL = channelDataL[i];
            float inR = channelDataR ? channelDataR[i] : inL;

            float delayedL = delayL[readPos];
            float delayedR = delayR ? delayR[readPos] : delayedL;

            channelDataL[i] += delayedL; // Add delay
            if (channelDataR) channelDataR[i] += delayedR;

            delayWriteL[delayWritePosition] = inL + delayedL * feedback;
            if (delayWriteR) delayWriteR[delayWritePosition] = inR + delayedR * feedback;

            delayWritePosition++;
            if (delayWritePosition >= delayLen) delayWritePosition = 0;
        }
    }

    // 6. VOID (Reverb)
    if (voidVal > 0.0f)
    {
        reverbParams.roomSize = voidVal;
        reverbParams.dryLevel = 0.0f; // We are inserting it, so we handle dry/wet manually or just process
        reverbParams.wetLevel = 1.0f;
        reverb.setParameters(reverbParams);
        
        // Reverb expects stereo usually
        reverb.process(context);
    }

    // 7. Drown (Dry/Wet Mix)
    // Mix dryBuffer with processed buffer
    for (int ch = 0; ch < totalNumInputChannels; ++ch)
    {
        auto* dry = dryBuffer.getReadPointer(ch);
        auto* wet = buffer.getWritePointer(ch);
        
        for (int i = 0; i < numSamples; ++i)
        {
            wet[i] = dry[i] * (1.0f - drownVal) + wet[i] * drownVal;
        }
    }
}

//==============================================================================


juce::AudioProcessorEditor* AbyssalGazeNewAudioProcessor::createEditor()
{
    return new AbyssalGazeNewAudioProcessorEditor (*this);
}

//==============================================================================
void AbyssalGazeNewAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void AbyssalGazeNewAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AbyssalGazeNewAudioProcessor();
}
