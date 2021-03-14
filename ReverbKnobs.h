/*
  ==============================================================================

    ReverbKnobs.h
    Created: 15 Mar 2021 3:26:47am
    Author:  jinator

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"

//==============================================================================
/*
*/
class ReverbKnobs  : public juce::Component,
                     public juce::Slider::Listener
{
public:
    ReverbKnobs(DJAudioPlayer* player);
    ~ReverbKnobs() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    /** implement Slider::Listener */
    void sliderValueChanged (juce::Slider* slider) override;

private:

    juce::Slider roomSizeSlider;
    juce::Slider dampingSlider;
    juce::Slider wetLevelSlider;
    juce::Slider dryLevelSlider;
    juce::Slider widthLevelSlider;
    juce::Slider freezeSlider;

    // juce::Reverb::Parameters reverbParameters;

    void configureSlider(juce::Slider* slider);

    DJAudioPlayer* player;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbKnobs)
};
