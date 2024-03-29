/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 3 Mar 2021 11:29:40am
    Author:  jinator

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager)
: formatManager(_formatManager)
{

}

DJAudioPlayer::~DJAudioPlayer()
{

}

//==============================================================================
void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
  transportSource.prepareToPlay(
    samplesPerBlockExpected,
    sampleRate
    );
  resampleSource.prepareToPlay(
    samplesPerBlockExpected,
    sampleRate
    );
  reverbSource.prepareToPlay(
    samplesPerBlockExpected,
    sampleRate
  );
}

void DJAudioPlayer::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
  // resampleSource.getNextAudioBlock(bufferToFill);
  reverbSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources()
{
  transportSource.releaseResources();
  resampleSource.releaseResources();
  reverbSource.releaseResources();
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

void DJAudioPlayer::setRewindPosition()
{
  double currentPositionSeconds = transportSource.getCurrentPosition();
  if (currentPositionSeconds > 5.0)
  {
    setPosition(currentPositionSeconds - 5.0);
  }
  else
  {
    setPosition(0.0);
  }
}

void DJAudioPlayer::setForwardPosition()
{
  double currentPositionSeconds = transportSource.getCurrentPosition();
  double newForwardPosition = currentPositionSeconds + 5.0;
  double audioTrackLength = getLengthInSeconds();
  if (newForwardPosition > audioTrackLength)
  { 
    setPosition(audioTrackLength);
  }
  else {
    setPosition(newForwardPosition);
  }

}

void DJAudioPlayer::setPositionRelative(double pos)
{
  if (pos < 0 || pos > 1.0)
  {
    std::cout << "DJAudioPlayer::setPos posRelative should be between 0 and 1" << std::endl;
  }
  else {
    double posInSecs = transportSource.getLengthInSeconds() * pos;
    setPosition(posInSecs);
  }
}

void DJAudioPlayer::setRoomSize(float size)
{
  audioReverbParameters.roomSize = size;
  reverbSource.setParameters(audioReverbParameters);
}

void DJAudioPlayer::setDamping(float damp)
{
  audioReverbParameters.damping = damp;
  reverbSource.setParameters(audioReverbParameters);
}
void DJAudioPlayer::setWetLevel(float wet)
{
  audioReverbParameters.wetLevel = wet;
  reverbSource.setParameters(audioReverbParameters);
}
void DJAudioPlayer::setDryLevel(float dry)
{
  audioReverbParameters.dryLevel = dry;
  reverbSource.setParameters(audioReverbParameters);
}
void DJAudioPlayer::setWidthLevel(float width)
{
  audioReverbParameters.width = width;
  reverbSource.setParameters(audioReverbParameters);
}

void DJAudioPlayer::setFreezeLevel(float freeze)
{
  audioReverbParameters.freezeMode = freeze;
  reverbSource.setParameters(audioReverbParameters);
}

void DJAudioPlayer::start()
{
  transportSource.start();
}

void DJAudioPlayer::stop()
{
  transportSource.stop();
}

double DJAudioPlayer::getPositionRelative()
{
  return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

double DJAudioPlayer::getLengthInSeconds()
{
  return transportSource.getLengthInSeconds();
}