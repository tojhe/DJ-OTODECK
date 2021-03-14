/*
  ==============================================================================

    ReverbKnobs.cpp
    Created: 15 Mar 2021 3:26:47am
    Author:  jinator

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ReverbKnobs.h"

//==============================================================================
ReverbKnobs::ReverbKnobs(DJAudioPlayer* _player) : player(_player)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

  configureSlider(&roomSizeSlider);
  configureSlider(&dampingSlider);
  configureSlider(&wetLevelSlider);
  configureSlider(&dryLevelSlider);
  configureSlider(&widthLevelSlider);
  configureSlider(&freezeSlider);

  addAndMakeVisible(roomSizeSlider);
  addAndMakeVisible(dampingSlider);
  addAndMakeVisible(wetLevelSlider);
  addAndMakeVisible(dryLevelSlider);
  addAndMakeVisible(widthLevelSlider);
  addAndMakeVisible(freezeSlider);
}

ReverbKnobs::~ReverbKnobs()
{
}

void ReverbKnobs::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("ReverbKnobs", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void ReverbKnobs::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    double rowH = getHeight() / 3;

    roomSizeSlider.setBounds(0, 0, getWidth()/2, rowH);
    dampingSlider.setBounds(getWidth()/2, 0, getWidth()/2, rowH);
    wetLevelSlider.setBounds(0, rowH, getWidth()/2, rowH);
    dryLevelSlider.setBounds(getWidth()/2, rowH, getWidth()/2, rowH);
    widthLevelSlider.setBounds(0, rowH * 2, getWidth()/2, rowH);
    freezeSlider.setBounds(getWidth()/2, rowH * 2, getWidth()/2, rowH);

}

void ReverbKnobs::sliderValueChanged (juce::Slider* slider)
{
    if (slider == &roomSizeSlider)    
    {   
        std::cout << "room size slider moved "  << slider->getValue() << std::endl;
        player->setRoomSize(float(slider->getValue()));
    }
    if (slider == &dampingSlider)    
    {   
        std::cout << "damping slider moved "  << slider->getValue() << std::endl;
        player->setDamping(float(slider->getValue()));
    }
    if (slider == &wetLevelSlider)    
    {   
        std::cout << "wet level slider moved "  << slider->getValue() << std::endl;
        player->setWetLevel(float(slider->getValue()));
    }
    if (slider == &dryLevelSlider)    
    {   
        std::cout << "dry level slider moved "  << slider->getValue() << std::endl;
        player->setDryLevel(float(slider->getValue()));
    }
    if (slider == &widthLevelSlider)    
    {   
        std::cout << "width slider moved "  << slider->getValue() << std::endl;
        player->setWidthLevel(float(slider->getValue()));
    }
    if (slider == &freezeSlider)    
    {   
        std::cout << "freeze level slider moved "  << slider->getValue() << std::endl;
        player->setFreezeLevel(float(slider->getValue()));
    }
}

void ReverbKnobs::configureSlider(juce::Slider* slider)
    {
      slider->addListener(this);
      slider->setRange(0.0, 1.0);
      slider->setSliderStyle(juce::Slider::Rotary);
      slider->setNumDecimalPlacesToDisplay(2);
      slider->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    }