/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 3 Mar 2021 11:29:40am
    Author:  jinator

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer()
{

}

DJAudioPlayer::~DJAudioPlayer()
{

}

//==============================================================================
void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
  formatManager.registerBasicFormats();

  transportSource.prepareToPlay(
    samplesPerBlockExpected,
    sampleRate
    );
  resampleSource.prepareToPlay(
    samplesPerBlockExpected,
    sampleRate
    );
}

void DJAudioPlayer::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
  resampleSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources()
{
  transportSource.releaseResources();
  resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(juce::URL audioURL)
{
  juce::AudioFormatReader* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
  if (reader != nullptr)
  {
      std::unique_ptr<juce::AudioFormatReaderSource> newSource (new juce::AudioFormatReaderSource (reader,
      true));
      transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);
      readerSource.reset (newSource.release());
  } 
  else
  {
      std::cout << "Something went wrong loading the file " << std::endl;
  }
}

void DJAudioPlayer::setGain(double gain)
{
  if (gain < 0 || gain > 1.0)
  {
    std::cout << "DJAudioPlayer::setGain gain should be between 0 and 1" << std::endl;
  }
  else {
    transportSource.setGain(gain);
  }
}

void DJAudioPlayer::setSpeed(double ratio)
{
  if (ratio < 0 || ratio > 100.0)
  {
    std::cout << "DJAudioPlayer::setGain gain should be between 0 and 100" << std::endl;
  }
  else {
    resampleSource.setResamplingRatio(ratio);
  }
}

void DJAudioPlayer::setPosition(double posInSecs)
{
  transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
  if (gain < 0 || gain > 1.0)
  {
    std::cout << "DJAudioPlayer::setPos posRelative should be between 0 and 1" << std::endl;
  }
  else {
    double posInSecs = transportSource.getLengthInSeconds() * pos;
    setPosition(posInSecs);
  }
}

void DJAudioPlayer::start()
{
  transportSource.start();
}

void DJAudioPlayer::stop()
{
  transportSource.stop();
}