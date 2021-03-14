/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 8 Mar 2021 10:18:27pm
    Author:  jinator

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(DJAudioPlayer* _player,
                                     DeckGUI* _deckGUI1,
                                     DeckGUI* _deckGUI2
                                     ): player(_player),
                                        deckGUI1(_deckGUI1),
                                        deckGUI2(_deckGUI2)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    // Create playlist table columns
    playlist.getHeader().addColumn("Title", 1, 200);
    playlist.getHeader().addColumn("Length", 2, 200);
    playlist.getHeader().addColumn("Deck 1", 3, 400/3);
    playlist.getHeader().addColumn("Deck 2", 4, 400/3);
    playlist.getHeader().addColumn("Remove", 5, 400/3);

    playlist.setModel(this);

    searchField.applyFontToAllText(juce::Font{14.0f});
    searchField.setJustification(juce::Justification::centred);
    searchField.setTextToShowWhenEmpty("SEARCH PLAYLIST (ESC KEY TO CLEAR)", juce::Colours::orange);
    searchField.setInputRestrictions (24);

    addAndMakeVisible(playlist);
    addAndMakeVisible(importButton);
    addAndMakeVisible(searchField);

    importButton.addListener(this);
    searchField.addListener(this);

    searchField.onTextChange = [this]{searchPlaylist(searchField.getText());};
    searchField.onEscapeKey = [this]{searchField.clear(); playlist.deselectAllRows();};

    loadTracks();
}

PlaylistComponent::~PlaylistComponent()
{
  saveTracks();
}

void PlaylistComponent::paint (juce::Graphics& g)
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
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..


    searchField.setBounds(0, 0, getWidth(), getHeight()/10);

    // Playlist table
    playlist.setBounds(0, getHeight() * 1/10, getWidth(), getHeight() * 4/5);
    // track title column
    playlist.getHeader().setColumnWidth(1 , getWidth()/4);
    // track length column
    playlist.getHeader().setColumnWidth(2 , getWidth()/4);
    // add track to Deck 1 column
    playlist.getHeader().setColumnWidth(3 , getWidth()/6);
    // add track to Deck 2 column
    playlist.getHeader().setColumnWidth(4 , getWidth()/6);
    // remove track column
    playlist.getHeader().setColumnWidth(5 , getWidth()/6);
    
    importButton.setBounds(0, getHeight() * 9/10, getWidth(), getHeight()/10);
}

int PlaylistComponent::getNumRows()
{
    return tracks.size();
}

void PlaylistComponent::paintRowBackground (Graphics & g,
                                            int rowNumber,
                                            int width,
                                            int height,
                                            bool rowIsSelected)
{
    if (rowIsSelected)
    {
      g.fillAll(juce::Colours::orange);
    }
    else {
      g.fillAll(juce::Colours::darkgrey);
    }
}

void PlaylistComponent::paintCell (Graphics & g,
                                   int rowNumber,
                                   int columnId,
                                   int width,
                                   int height,
                                   bool rowIsSelected)
{   
    if (columnId == 1)
    {
      g.drawText (tracks[rowNumber].title,
                  2, 0,
                  width - 4, height,
                  juce::Justification::centredLeft,
                  true);
    }
    else if (columnId == 2)
    {
      g.drawText (tracks[rowNumber].length,
                  2, 0,
                  width - 4, height,
                  juce::Justification::centredLeft,
                  true);
    }
}

juce::Component * PlaylistComponent::refreshComponentForCell (int rowNumber,
                                                              int columnId,
                                                              bool isRowSelected,
                                                              juce::Component *existingComponentToUpdate)
{
    if (columnId == 3)
    {
      if (existingComponentToUpdate == nullptr)
      {
        TextButton* btn = new juce::TextButton{"Add"};
        juce::String id{std::to_string(rowNumber) + "_addDeck1"};
        btn->setComponentID(id);
        btn->addListener(this);
        existingComponentToUpdate = btn;
      }
    }
    if (columnId == 4)
    {
      if (existingComponentToUpdate == nullptr)
      {
        TextButton* btn = new juce::TextButton{"Add"};
        juce::String id{std::to_string(rowNumber) + "_addDeck2"};
        btn->setComponentID(id);
        btn->addListener(this);
        existingComponentToUpdate = btn;
      }
    }
    if (columnId == 5)
    {
      if (existingComponentToUpdate == nullptr)
      {
        TextButton* btn = new juce::TextButton{"X"};
        juce::String id{std::to_string(rowNumber) + "_remove"};
        btn->setComponentID(id);
        btn->addListener(this);
        existingComponentToUpdate = btn;
      }
    }
    return existingComponentToUpdate;
}

std::string PlaylistComponent::secondsToMinutes(double seconds)
{
  // get minutes
  int minutes = int(trunc(seconds/60.0));
  std::string minStr = std::to_string(minutes);
  // get seconds
  int remainingSeconds = int(seconds) % 60;
  std::string secStr = std::to_string(remainingSeconds);

  // Extend single digit seconds
  if (secStr.length() <= 1)
  {
    secStr = '0' + secStr;
  }
  return minStr + ":" + secStr;
}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
  if (button == &importButton)
  {
    std::cout << "Add tracked button was clicked " << std::endl;
    juce::FileChooser chooser{"Select files..."};
    
    // Select multiple tracks
    if (chooser.browseForMultipleFilesOrDirectories())
    {   
        juce::Array<juce::File> files = chooser.getResults();
        for (juce::File& file : files)
        { 
          bool trackExists = false;

          // init audioTrack instance
          AudioTrack track {file};

          // get track length
          player->loadURL(track.sourceURL);
          double lengthInSeconds = player->getLengthInSeconds();
          track.length = secondsToMinutes(lengthInSeconds);

          // check if track already exists
          for (AudioTrack& existingTrack : tracks)
          {
            if (existingTrack.title == track.title)
            {
              trackExists = true;
            }
          }
          
          // add track if it does not exist
          if (!trackExists) 
          {
            // save track to tracks
            tracks.push_back(track);

            std::cout << track.title << " added" << std::endl;
          }
        }
        playlist.updateContent();
    }
  }
  else {
        int id = std::stoi(button->getComponentID().toStdString());
    std::cout << "PlaylistComponent::buttonClicked " << tracks[id].title << std::endl;

    juce::String buttonClicked = button->getComponentID().toStdString();
    std::cout << "PlaylistComponent::buttonClicked " << buttonClicked << std::endl;

    handleTrackButtons(buttonClicked, id);

    playlist.updateContent();
  }
}

void PlaylistComponent::handleTrackButtons(juce::String buttonClicked, int id)
{
    if (buttonClicked.contains(juce::String {"addDeck1"}))
    {
      std::cout << "Adding " << tracks[id].title << " to deck 1" << std::endl;
      //temporary. will need to create method in DeckGUI to accept URL and load
      deckGUI1->loadTrack(tracks[id].sourceURL);
    }
    if (buttonClicked.contains(juce::String {"addDeck2"}))
    {
      std::cout << "Adding " << tracks[id].title << " to deck 2" << std::endl;
      deckGUI2->loadTrack(tracks[id].sourceURL);
    }
    if (buttonClicked.contains(juce::String {"remove"}))
    {
      std::cout << "Removing " << tracks[id].title << std::endl;
      tracks.erase(tracks.begin() + id);
    }
}

void PlaylistComponent::searchPlaylist(juce::String inputText)
{
  // searching algorithm http://www.cplusplus.com/reference/algorithm/search/
  int matchingTrackId;
  for (int i=0; i < tracks.size(); i++)
  {
    if (tracks[i].title.contains(inputText))
    {
      matchingTrackId = i;
    }
    playlist.selectRow(matchingTrackId);
  }
}

void PlaylistComponent::saveTracks()
{
  // save tracks filepaths to tracks_file.csv
  std::ofstream savedTracksFile;
  savedTracksFile.open("./tracks_file.csv");
  for (AudioTrack& track : tracks)
  {
      savedTracksFile << track.trackFile.getFullPathName() << "\n";
      std::cout << "Saving " << track.trackFile.getFullPathName() << std::endl;
  }
  savedTracksFile.close();
}

void PlaylistComponent::loadTracks()
{
  // create input stream from saved library
  std::ifstream savedTracksFile;
  std::string filePath;
  std::string length;

  savedTracksFile.open("./tracks_file.csv");
  // Parse data
  while (getline(savedTracksFile, filePath)) {

    std::cout << "Getting " << filePath << std::endl;
    // load track
    juce::File file {filePath};
    AudioTrack track {file};

    // update length attribute
    player->loadURL(track.sourceURL);
    double lengthInSeconds = player->getLengthInSeconds();
    track.length = secondsToMinutes(lengthInSeconds);

    // push to tracks playlist
    tracks.push_back(track);
    std::cout << "LOADED " << filePath << std::endl;
  }
  
   savedTracksFile.close();

  remove ("tracks_file.csv");
  playlist.updateContent();
}