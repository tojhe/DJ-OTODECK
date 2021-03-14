/*
  ==============================================================================

    AudioTrack.cpp
    Created: 13 Mar 2021 9:30:19pm
    Author:  jinator

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AudioTrack.h"

//==============================================================================
AudioTrack::AudioTrack(juce::File _file) : trackFile(_file),
                                           title(_file.getFileNameWithoutExtension()),
                                           sourceURL(juce::URL{_file})                                           
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

AudioTrack::~AudioTrack()
{
}