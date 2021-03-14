/*
  ==============================================================================

    AudioTrack.h
    Created: 13 Mar 2021 9:30:19pm
    Author:  jinator

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <string>

//==============================================================================
/*
*/
class AudioTrack
{
public:
    AudioTrack(juce::File file);
    ~AudioTrack();

    juce::File trackFile;
    juce::String title;
    std::string length;
    juce::URL sourceURL;
};
