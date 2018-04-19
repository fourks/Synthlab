/*
  ==============================================================================

    SawtoothModule
    Created: 4 Apr 2018 3:48:23pm
    Author:  Matthias Pueski

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ADSRModule.h"
#include "Connection.h"
#include "AudioEngine/Sawtooth.h"

//==============================================================================
ADSRModule::ADSRModule(double sampleRate, int buffersize)
{
    this->sampleRate = sampleRate;
    this->buffersize = buffersize;

    setSize(120,140);
    nameLabel->setJustificationType (Justification::left);
    nameLabel->setTopLeftPosition(18,2);
    
    setName("ADSR");

    // Envelopes for polyphonic mode
    for (int i = 0; i < 128;i++){
        this->envelopes[i] = new ADSR();
    }
    
    // Single envelope for monophonic mode and modulation purposes
    this->envelope = new ADSR();
    
    editable = false;
    prefab = true;
    
    createProperties();
}

ADSRModule::~ADSRModule()
{
    delete isMonoListener;
    delete isMonoValue;
    for (int i = 0; i < 128;i++){
        delete this->envelopes[i];
    }
    delete envelope;
}

void ADSRModule::createProperties() {
    isMonoValue = new Value();
    isMonoListener = new IsMonoListener(*isMonoValue, this);
}


void ADSRModule::configurePins() {
    
    Pin* p1 = new Pin(Pin::Type::EVENT);
    p1->direction = Pin::Direction::IN;
    p1->listeners.push_back(this);
    p1->setName("G");
    
    Pin* p2 = new Pin(Pin::Type::VALUE);
    p2->direction = Pin::Direction::IN;
    p2->listeners.push_back(this);
    p2->setName("A");
    
    Pin* p3 = new Pin(Pin::Type::VALUE);
    p3->direction = Pin::Direction::IN;
    p3->listeners.push_back(this);
    p3->setName("D");
    
    Pin* p4 = new Pin(Pin::Type::VALUE);
    p4->direction = Pin::Direction::IN;
    p4->listeners.push_back(this);
    p4->setName("S");
    
    Pin* p5 = new Pin(Pin::Type::VALUE);
    p5->direction = Pin::Direction::IN;
    p5->listeners.push_back(this);
    p5->setName("R");
    
    Pin* p6= new Pin(Pin::Type::VALUE);
    p6->direction = Pin::Direction::OUT;
    p6->listeners.push_back(this);
    p6->setName("Out");
    
    
    addPin(Pin::Direction::IN,p1);
    addPin(Pin::Direction::IN,p2);
    addPin(Pin::Direction::IN,p3);
    addPin(Pin::Direction::IN,p4);
    addPin(Pin::Direction::IN,p5);
    addPin(Pin::Direction::OUT,p6);
  
}

void ADSRModule::paint(juce::Graphics &g) {
    Module::paint(g);
    g.drawImageAt(ImageCache::getFromMemory(BinaryData::adsr_png, BinaryData::adsr_pngSize),25,40);
}

void ADSRModule::setAttack(float attack) {
    if (attack != this->attack) {
        this->attack = attack;
        this->envelope->setAttackRate(sampleRate * attack);
        for (int i = 0; i < 128;i++){
            this->envelopes[i]->setAttackRate(sampleRate * attack );
        }
    }
}

void ADSRModule::setDecay(float decay) {
    if (decay != this->decay) {
        this->decay = decay;
        this->envelope->setDecayRate(sampleRate * decay);
        for (int i = 0; i < 128;i++){
            this->envelopes[i]->setDecayRate(sampleRate * decay );
        }
    }
}

void ADSRModule::setSustain(float sustain) {
    if (sustain != this->sustain) {
        this->sustain = sustain;
        this->envelope->setSustainLevel(sustain);
        for (int i = 0; i < 128;i++){
            this->envelopes[i]->setSustainLevel(sustain);
        }
    }
}

void ADSRModule::setRelease(float release) {
    if (release != this->release) {
        this->release = release;
        this->envelope->setReleaseRate(sampleRate * release);
        for (int i = 0; i < 128;i++){
            this->envelopes[i]->setReleaseRate(sampleRate * release );
        }
    }
}

float ADSRModule::getAttack() {
    return attack;
}

float ADSRModule::getDecay() {
    return decay;
}

float ADSRModule::getSustain() {
    return sustain;
}

float ADSRModule::getRelease() {
    return release;
}

void ADSRModule::process() {
    
    if (pins.at(1)->connections.size() ==  1) {
        this->setAttack(abs(pins.at(1)->connections.at(0)->getValue()));
    }
    if (pins.at(2)->connections.size() ==  1) {
        this->setDecay(abs(pins.at(2)->connections.at(0)->getValue()));
    }
    
    if (pins.at(3)->connections.size() ==  1) {
        this->setSustain(abs(pins.at(3)->connections.at(0)->getValue()));
    }
    
    if (pins.at(4)->connections.size() ==  1) {
        this->setRelease(abs(pins.at(4)->connections.at(0)->getValue()));
    }
    
    if (mono) {
        for (int i = 0; i < buffersize;i++){
            pins.at(5)->setValue(this->envelope->process());
        }
    }
    else {
        for (int i = 0; i < buffersize;i++){
            for (int j = 0; j < 128;j++) {
                pins.at(5)->data[j] = this->envelopes[j]->process();
            }
        }
    }

}

void ADSRModule::setMono(bool value) {
    this->mono = value;
}

bool ADSRModule::isMono() {
    return mono;
}

void ADSRModule::eventReceived(Event *e) {
    
    if (e->getType() == Event::Type::GATE) {
        if (e->getValue() > 0) {
            if (mono) {
                this->envelope->gate(e->getValue());
            }
            else {
                this->gate[e->getNote()] = true;
                pins.at(5)->dataEnabled[e->getNote()] = true;
                voices++;
                this->envelopes[e->getNote()]->gate(e->getValue());

            }
        }
        else {
            if (mono) {
                this->envelope->gate(0);
            }
            else {
                this->gate[e->getNote()]= false;
                this->envelopes[e->getNote()]->gate(0);
                if (voices > 0)
                    voices--;
            }

        }

    }
    
}
juce::Array<PropertyComponent*>& ADSRModule::getProperties() {
    
    properties = juce::Array<PropertyComponent*>();
    isMonoProp = new BooleanPropertyComponent(*isMonoValue,"isMono","Mono");
    
    
    properties.add(isMonoProp);
    
    return properties;
}

