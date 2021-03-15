/*
  ==============================================================================

    DeckGUI.h
    Created: 4 Mar 2021 1:35:09am
    Author:  jinator

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "ReverbKnobs.h"
#include <string>
//==============================================================================
/*
*/
class DeckGUI  : public juce::Component,
                 public juce::Button::Listener,
                 public juce::Slider::Listener,
                 public juce::FileDragAndDropTarget,
                 public Timer
{
public:
    DeckGUI(DJAudioPlayer* player, 
            juce::AudioFormatManager& formatManagerToUse,
            juce::AudioThumbnailCache& cacheToUse,
            ReverbKnobs* reverbKnobs,
            int guiID
            );
    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    /** implement Button::Listener **/
    void buttonClicked (juce::Button *) override;
    
    /** implement Slider::Listener */
    void sliderValueChanged (juce::Slider *slider) override;

    bool isInterestedInFileDrag (const StringArray &files) override;
    void filesDropped (const StringArray &files, int x, int y) override;

    void timerCallback() override;

    void loadTrack(juce::URL sourceURL);
 
private:

    /** Configure slider at initialization */
    void configureSlider(juce::Slider* slider, bool verticalSlider, std::string suffix,
                         double minRange, double maxRange, double initValue);

    juce::TextButton playButton{"PLAY"};
    juce::TextButton stopButton{"STOP"};
    juce::TextButton rewindButton{"<<REWIND"};
    juce::TextButton forwardButton{"FORWARD>>"};
    juce::TextButton loadButton{"LOAD"};
    juce::Label infoDisplay {{}, ""};
    
    juce::Slider volSlider;
    juce::Slider speedSlider;
    juce::Slider posSlider;

    DJAudioPlayer* player;

    WaveformDisplay waveformDisplay;
    
    ReverbKnobs* reverbKnobs;

    int guiID;
    juce::String displayMessage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
