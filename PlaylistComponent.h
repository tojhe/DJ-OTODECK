/*
  ==============================================================================

    PlaylistComponent.h
    Created: 8 Mar 2021 10:18:27pm
    Author:  jinator

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "AudioTrack.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include <vector>
#include <string>
#include <cmath>
#include <fstream>
#include <iostream>
#include <stdio.h>

//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
                           public juce::TableListBoxModel,
                           public juce::Button::Listener,
                           public juce::TextEditor::Listener
{
public:
    PlaylistComponent(DJAudioPlayer* player,
                      DeckGUI* deckGUI1,
                      DeckGUI* deckGUI2);
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    int getNumRows() override;
    void paintRowBackground (Graphics & g,
                             int rowNumber,
                             int width,
                             int height,
                             bool rowIsSelected) override;

    void paintCell (Graphics & g,
                    int rowNumber,
                    int columnId,
                    int width,
                    int height,
                    bool rowIsSelected) override;

    juce::Component * refreshComponentForCell (int rowNumber,
                                               int columnId,
                                               bool isRowSelected,
                                               juce::Component *existingComponentToUpdate) override;
                            
    void buttonClicked(juce::Button* button) override;


private:

    void saveTracks();
    void loadTracks();
    /** convert track length from seconds to min:sec in string */
    std::string secondsToMinutes(double seconds);

    /** search tracks on playlist by name */
    void searchPlaylist(juce::String inputText);

    /** Logical operators for buttons clicked on playlist table */
    void handleTrackButtons(juce::String buttonClicked, int id);

    juce::TableListBox playlist;

    std::vector<AudioTrack> tracks;
    DJAudioPlayer* player;
    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;

    juce::TextButton importButton{"IMPORT TRACKS"};
    juce::TextEditor searchField;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
