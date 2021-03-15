/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 3 Mar 2021 11:29:40am
    Author:  jinator

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>

class DJAudioPlayer : public juce::AudioSource {
  public:
    DJAudioPlayer(juce::AudioFormatManager& _formatManager);
    ~DJAudioPlayer();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void loadURL(juce::URL audioURL);
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);
    void setRoomSize(float size);
    void setDamping(float damp);
    void setWetLevel(float wet);
    void setDryLevel(float dry);
    void setWidthLevel(float width);
    void setFreezeLevel(float freeze);

    /** get the relative position of the playhead*/
    double getPositionRelative();
    
    double getLengthInSeconds();

    /** Rewind track  by 5 sec*/
    void setRewindPosition();
    /** Fast forward track by 5 sec*/
    void setForwardPosition();

    void start();
    void stop();

  private:
    juce::AudioFormatManager& formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;

    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource {&transportSource, false, 2};
    juce::ReverbAudioSource reverbSource {&resampleSource, false};

    juce::Reverb::Parameters audioReverbParameters;
};