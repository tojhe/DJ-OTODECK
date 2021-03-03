#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }

    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);

    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    volSlider.setRange(0.0, 1.0);
    posSlider.setRange(0.0, 1.0);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
    

    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

/*
void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    auto* leftChan = bufferToFill.buffer->getWritePointer(0, 
                                                        bufferToFill.startSample);
    auto* rightChan = bufferToFill.buffer->getWritePointer(1, 
                                                        bufferToFill.startSample);                                                        
    
    for (auto i=0; i < bufferToFill.numSamples ; ++i)
    {
        // double sample = rand.nextDouble() * 0.25;
        // double sample = fmod(phase, 0.2);
        double sample = sin(phase) * 0.01;
        leftChan[i] = sample;
        rightChan[i] = sample;

        phase += dphase;
    }
    // bufferToFill.clearActiveBufferRegion();
}
*/

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)

    player1.getNextAudioBlock(bufferToFill);
}


void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    player1.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
    // g.setFont(20.0f);
    // g.drawText ("Hello from Singapore", getLocalBounds(),
    //            juce::Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

    double rowH = getHeight() / 6;

    playButton.setBounds(0, 0, getWidth(), rowH);
    stopButton.setBounds(0, rowH, getWidth(), rowH);

    volSlider.setBounds(0, rowH * 2, getWidth(), rowH);
    speedSlider.setBounds(0, rowH * 3, getWidth(), rowH);
    posSlider.setBounds(0, rowH * 4, getWidth(), rowH);
    
    loadButton.setBounds(0, rowH * 5, getWidth(), rowH);    
}

void MainComponent::buttonClicked(juce::Button* button)
{   
    // Use pointer and memory address match
    if (button == &playButton)
    {
        std::cout << "Play button was clicked " << std::endl;
        player1.start();
    }
    if (button == &stopButton)
    {
        std::cout << "Stop button was clicked " << std::endl;
        player1.stop();
    }
    if (button == &loadButton)
    {
        std::cout << "Load button was clicked " << std::endl;
        juce::FileChooser chooser{"Select a file..."};
        if (chooser.browseForFileToOpen())
        {
            player1.loadURL(juce::URL {chooser.getResult()});
        }
    }
}

void MainComponent::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volSlider)
    {
        std::cout << "vol slider moved "  << slider->getValue() << std::endl;
        player1.setGain(slider->getValue());
        // dphase = volSlider.getValue() * 0.001;
    }
    if (slider == &speedSlider)
    {
        std::cout << "speed slider moved "  << slider->getValue() << std::endl;
        // resampleSource.setResamplingRatio(slider->getValue());
        // dphase = volSlider.getValue() * 0.001;
        player1.setSpeed(slider->getValue());
    }

    if (slider == &posSlider)
    {
        std::cout << "speed slider moved "  << slider->getValue() << std::endl;
        player1.setPositionRelative(slider->getValue());
    }
}