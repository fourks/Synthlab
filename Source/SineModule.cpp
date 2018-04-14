/*
  ==============================================================================

    SawtoothModule
    Created: 4 Apr 2018 3:48:23pm
    Author:  Matthias Pueski

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "SineModule.h"
#include "Connection.h"
#include "AudioEngine/Sine.h"

//==============================================================================
SineModule::SineModule(double sampleRate, int buffersize)
{
    this->sampleRate = sampleRate;
    this->buffersize = buffersize;
    
    oscillator = new Sine(sampleRate);
    
    oscillator->setFrequency(440);
    oscillator->setVolume(0.5f);
    
    setSize(120,140);
    nameLabel->setJustificationType (Justification::left);
    nameLabel->setTopLeftPosition(18,2);
    
    setName("Sine");

    editable = false;
    prefab = true;
}

SineModule::~SineModule()
{
    delete oscillator;
}

void SineModule::configurePins() {
    Pin* p1 = new Pin(Pin::Type::VALUE);
    p1->direction = Pin::Direction::IN;
    p1->listeners.push_back(this);
    p1->setName("P");
    
    Pin* p2 = new Pin(Pin::Type::VALUE);
    p2->direction = Pin::Direction::IN;
    p2->listeners.push_back(this);
    p2->setName("F");
    
    Pin* p3 = new Pin(Pin::Type::VALUE);
    p3->direction = Pin::Direction::IN;
    p3->listeners.push_back(this);
    p3->setName("A");
    
    Pin* p4 = new Pin(Pin::Type::AUDIO);
    p4->direction = Pin::Direction::OUT;
    p4->listeners.push_back(this);
    p4->setName("Out");
    
    Pin* p5 = new Pin(Pin::Type::VALUE);
    p5->direction = Pin::Direction::OUT;
    p5->listeners.push_back(this);
    p5->setName("V");
    
    addPin(Pin::Direction::IN,p1);
    addPin(Pin::Direction::IN,p2);
    addPin(Pin::Direction::IN,p3);
    addPin(Pin::Direction::OUT,p4);
    addPin(Pin::Direction::OUT,p5);
}

void SineModule::paint(juce::Graphics &g) {
    Module::paint(g);
    g.drawImageAt(ImageCache::getFromMemory(BinaryData::sine_png, BinaryData::sine_pngSize),25,40);
}


void SineModule::setPitch(int pitch) {
    this->pitch = pitch;
    this->oscillator->setPitch(pitch);
}

void SineModule::setFine(float fine) {
    this->fine = fine;
    this->oscillator->setFine(fine);
}

void SineModule::setAmplitude(float amplitude) {
    this->amplitude = amplitude;
    this->oscillator->setVolume(amplitude);
}

void SineModule::process() {
    
    if (pins.at(0)->connections.size() ==  1) {
        this->oscillator->setFrequency(pins.at(0)->connections.at(0)->getValue());
    }
    if (pins.at(1)->connections.size() ==  1) {
        this->setFine(pins.at(1)->connections.at(0)->getValue());
    }
    if (pins.at(2)->connections.size() ==  1) {
        this->setAmplitude(abs(pins.at(2)->connections.at(0)->getValue()));
    }
    for (int i = 0; i < buffersize; i++) {
        float value = oscillator->process();
        
        if (pins.at(3)->getAudioBuffer() != nullptr && pins.at(3)->getAudioBuffer()->getNumChannels() > 0)
            pins.at(3)->getAudioBuffer()->setSample(0,i ,value);

        pins.at(4)->setValue(abs(value + 1));
    }
    
}