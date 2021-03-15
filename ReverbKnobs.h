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
#include <string>

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

    /** Configure knob slider on initialization */
    void configureSlider(juce::Slider* slider, std::string suffix, float startValue);

    DJAudioPlayer* player;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbKnobs)
};
