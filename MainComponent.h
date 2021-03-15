#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"
#include "ReverbKnobs.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::AudioAppComponent,
                      public juce::MixerAudioSource
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;


private:
    //==============================================================================
    // Your private member variables go here...

    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbCache{100};

    DJAudioPlayer player1{formatManager};
    ReverbKnobs reverbKnobs1{&player1};
    DeckGUI deckGUI1 {&player1, formatManager, thumbCache, &reverbKnobs1, 1};
    
    DJAudioPlayer player2{formatManager};
    ReverbKnobs reverbKnobs2{&player2};
    DeckGUI deckGUI2 {&player2, formatManager, thumbCache, &reverbKnobs2, 2};

    MixerAudioSource mixerSource;

    DJAudioPlayer metadataCheckPlayer{formatManager};
    PlaylistComponent playlistComponent{&metadataCheckPlayer,
                                        &deckGUI1,
                                        &deckGUI2};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
