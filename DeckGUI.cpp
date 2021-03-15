/*
  ==============================================================================

    DeckGUI.cpp
    Created: 4 Mar 2021 1:35:09am
    Author:  jinator

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
                juce::AudioFormatManager& formatManagerToUse,
                juce::AudioThumbnailCache& cacheToUse,
                ReverbKnobs* _reverbKnobs,
                int _guiID
                ) : player(_player),
                    waveformDisplay(formatManagerToUse, cacheToUse),
                    reverbKnobs(_reverbKnobs),
                    guiID(_guiID)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(rewindButton);
    addAndMakeVisible(forwardButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(waveformDisplay);
    addAndMakeVisible(reverbKnobs);
    addAndMakeVisible(infoDisplay);

    playButton.addListener(this);
    stopButton.addListener(this);
    rewindButton.addListener(this);
    forwardButton.addListener(this);
    loadButton.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    configureSlider(&volSlider, true, " Volume", 0.0, 1.0, 0.7);
    configureSlider(&speedSlider, true, "x Speed", 0.1, 10.0, 1.0);
    configureSlider(&posSlider, false, " Pos", 0.0, 1.0, 0.0);
   
    // Initialize information display messages
    displayMessage = juce::String {"Deck " + std::to_string(guiID) + "\n" + "Track: "};
    infoDisplay.setText(displayMessage, juce::NotificationType::dontSendNotification);

    startTimer(500);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
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
    g.drawText ("", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void DeckGUI::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    double rowH = getHeight() / 10;

    infoDisplay.setBounds(0, 0, getWidth(), rowH);

    playButton.setBounds(getWidth()/4, rowH, getWidth()/2, rowH * 2/3 );
    loadButton.setBounds(getWidth()/4, rowH*5/3, getWidth()/2, rowH * 2/3 );    
    stopButton.setBounds(getWidth()/4, rowH*7/3, getWidth()/2, rowH * 2/3 );

    rewindButton.setBounds(0, rowH, getWidth()/4, rowH * 2 );
    forwardButton.setBounds(getWidth() * 3/4, rowH, getWidth()/4, rowH * 2 );

    volSlider.setBounds(0, rowH * 3, getWidth()/4, rowH * 4 );
    speedSlider.setBounds(getWidth() * 1/4, rowH * 3, getWidth()/4, rowH * 4 );
    reverbKnobs->setBounds(getWidth() * 2/4, rowH * 3, getWidth() * 2/4, rowH * 4 );

    posSlider.setBounds(0, rowH * 7, getWidth(), rowH);    
    waveformDisplay.setBounds(0, rowH * 8, getWidth(), rowH * 2 );
}

void DeckGUI::buttonClicked(juce::Button* button)
{   
    // Use pointer and memory address match
    if (button == &playButton)
    {
        std::cout << "Play button was clicked " << std::endl;
        player->start();
    }
    if (button == &stopButton)
    {
        std::cout << "Stop button was clicked " << std::endl;
        player->stop();
    }
    if (button == &rewindButton)
    {
        std::cout << "Rewind button was clicked " << std::endl;
        player->setRewindPosition();
    }
    if (button == &forwardButton)
    {
        std::cout << "Forward button was clicked " << std::endl;
        player->setForwardPosition();
    }
    if (button == &loadButton)
    {
        std::cout << "Load button was clicked " << std::endl;
        juce::FileChooser chooser{"Select a file..."};
        if (chooser.browseForFileToOpen())
        {
            player->loadURL(juce::URL {chooser.getResult()});
            waveformDisplay.loadURL(juce::URL {chooser.getResult()});
            infoDisplay.setText(displayMessage + chooser.getResult().getFileName(),
                                juce::NotificationType::dontSendNotification); // update info display
        }
    }
}

void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volSlider)
    {
        std::cout << "vol slider moved "  << slider->getValue() << std::endl;
        player->setGain(slider->getValue());
    }
    if (slider == &speedSlider)
    {
        std::cout << "speed slider moved "  << slider->getValue() << std::endl;
        // resampleSource.setResamplingRatio(slider->getValue());
        // dphase = volSlider.getValue() * 0.001;
        player->setSpeed(slider->getValue());
    }
    if (slider == &posSlider)
    {
        std::cout << "speed slider moved "  << slider->getValue() << std::endl;
        player->setPositionRelative(slider->getValue());
    }
}

bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
  std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
  return true;
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{
  std::cout << "DeckGUI::filesDropped" << std::endl;
  if (files.size() == 1)
  {
    player->loadURL(juce::URL{juce::File{files[0]}});
    waveformDisplay.loadURL(juce::URL{juce::File{files[0]}});
    infoDisplay.setText(displayMessage + juce::File{files[0]}.getFileName(),
                        juce::NotificationType::dontSendNotification); // update info display
  }
}

void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(player->getPositionRelative());
}

void DeckGUI::loadTrack(juce::URL sourceURL)
{
    player->loadURL(sourceURL);
    waveformDisplay.loadURL(sourceURL);
    infoDisplay.setText(displayMessage + sourceURL.getFileName(),
                        juce::NotificationType::dontSendNotification); // update info display
}

void DeckGUI::configureSlider(juce::Slider* slider, bool verticalSlider, std::string suffix,
                              double minRange, double maxRange, double initValue)
{
    slider->setRange(minRange, maxRange);    
    slider->setValue(initValue);

    if (verticalSlider)
    {
        slider->setSliderStyle (juce::Slider::LinearVertical);
        slider->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    }
    else {
        slider->setSliderStyle (juce::Slider::LinearHorizontal);    
    }
    slider->setTextValueSuffix(suffix);
    slider->setNumDecimalPlacesToDisplay(2);
}