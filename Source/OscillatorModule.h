/*
 ==============================================================================
 
 MidiGate.h
 Created: 4 Apr 2018 3:48:23pm
 Author:  Matthias Pueski
 
 ==============================================================================
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Module.h"
#include "AudioEngine/Oszillator.h"
#include "Monophonic.h"
//==============================================================================
/*
 */

template <typename T>
class OscillatorModule : public Module, public Monophonic
{
public:
    OscillatorModule(double sampleRate, int buffersize, Image* image);
    ~OscillatorModule();
    
    void paint (Graphics& g) override;
    
    void setPitch(int pitch);
    void setAmplitude(float amplitude);
    void setFine(float fine);
    void process() override;
    virtual void configurePins() override;
    
    virtual String getCategory() override {
        return "Sound sources";
    }
    virtual String getDescription() override {
        return BinaryData::oscillator_txt;
    }
    
    bool isMono() override;
    void setMono(bool value) override;
    
    virtual juce::Array<PropertyComponent*>& getProperties() override;
    virtual void createProperties() override;
    
    
private:
    
    struct IsMonoListener : Value::Listener
    {
        IsMonoListener (Value& v, OscillatorModule* mod) : mod(mod),  value (v) { value.addListener (this); }
        ~IsMonoListener()  {}  // no need to remove the listener
        
        void valueChanged (Value& value) override {
            mod->setMono(value.toString().getIntValue() > 0);
        }
        OscillatorModule* mod;
        Value value;
    };
    
    Value* isMonoValue;
    PropertyComponent* isMonoProp;
    IsMonoListener* isMonoListener;
    
    AudioSampleBuffer* out = nullptr;
    float frequencies[128] = {0};
    int pitch = 0;
    float amplitude;
    float fine = 0;
    Image* image = nullptr;
    T* monoOscillator;
    T* oscillator[128];
    int currentSample = 0;
    bool mono;
    
    Pin* gatePin = nullptr;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscillatorModule)
};


/*
 ==============================================================================
 
 OscillatorModule
 Created: 4 Apr 2018 3:48:23pm
 Author:  Matthias Pueski
 
 ==============================================================================
 */

#include "../JuceLibraryCode/JuceHeader.h"
#include "OscillatorModule.h"
#include "Connection.h"

//==============================================================================
template<typename T> OscillatorModule<T>::OscillatorModule(double sampleRate, int buffersize, Image* image) : image(image)
{
    this->sampleRate = sampleRate;
    this->buffersize = buffersize;
    
    for (int i = 0; i < 128;i++){
        oscillator[i] = nullptr;
    }
    
    monoOscillator = new T(sampleRate, buffersize);
    
    setSize(120,140);
    nameLabel->setJustificationType (Justification::left);
    nameLabel->setTopLeftPosition(18,2);

    editable = false;
    prefab = true;
    
}

template<typename T> OscillatorModule<T>::~OscillatorModule()
{
    
    for (int i = 0; i < 128;i++){
        if (oscillator[i] != nullptr)
            delete oscillator[i];
    }
    
    delete monoOscillator;
    if (image != nullptr) {
        delete image;
    }
    delete isMonoListener;
    delete isMonoValue;
    
}

template<typename T> void OscillatorModule<T>::configurePins() {
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
    
    createProperties();
    out = pins.at(3)->getAudioBuffer();
    
}

template<typename T> void OscillatorModule<T>::paint(juce::Graphics &g) {
    Module::paint(g);
    g.drawImageAt(*image,25,40);
}


template<typename T> void OscillatorModule<T>::setPitch(int pitch) {
    this->pitch = pitch;
}

template<typename T> void OscillatorModule<T>::setFine(float fine) {
    if (this->fine != fine) {
        this->fine = fine;
        for (int i = 0; i < 128;i++){
            if (oscillator[i] != nullptr)
                this->oscillator[i]->setFine(fine);
        }
    }
}

template<typename T> void OscillatorModule<T>::setAmplitude(float amplitude) {
    this->amplitude = amplitude;
}

template<typename T> void OscillatorModule<T>::setMono(bool value) {
    this->mono = value;
    this->isMonoValue->setValue(value);
}

template<typename T> bool OscillatorModule<T>::isMono() {
    return mono;
}

template<typename T> juce::Array<PropertyComponent*>& OscillatorModule<T>::getProperties() {
    
    properties = juce::Array<PropertyComponent*>();
    isMonoProp = new BooleanPropertyComponent(*isMonoValue,"isMono","Mono");
    
    
    properties.add(isMonoProp);
    
    return properties;
}


template<typename T> void OscillatorModule<T>::createProperties() {
    isMonoValue = new Value();
    isMonoListener = new IsMonoListener(*isMonoValue, this);
    isMonoValue->setValue(mono);
}

template<typename T> void OscillatorModule<T>::process() {
    bool volumegate = false;
    
    
    
    if (pins.at(0)->connections.size() ==  1) {
        this->setPitch(pins.at(0)->connections.at(0)->getValue());
    }
    if (pins.at(1)->connections.size() ==  1) {
        this->setFine(pins.at(1)->connections.at(0)->getValue());
    }
    if (pins.at(2)->connections.size() ==  1) {
        volumegate = true;
        gatePin = pins.at(2)->connections.at(0);
    }
    if (pins.at(2)->connections.size() == 1) {
        for (int i = 0; i < buffersize; i++) {
            float value = 0;
            
            if (mono) {
                this->monoOscillator->setFrequency((440 * pow(2.0,((pitch)-69.0)/12.0)));
                this->monoOscillator->setVolume(pins.at(2)->connections.at(0)->getValue());
                value = monoOscillator->process();
                pins.at(4)->setValue(abs(value + 1));
            }
            else {
                for (int j = 0; j < 128;j++){
                    
                    if(gatePin->dataEnabled[j]) {
                        if (oscillator[j] ==  nullptr) {
                            oscillator[j] = new T(sampleRate, buffersize);
                            
                            oscillator[j]->setFrequency((440 * pow(2.0,((j+1+pitch)-69.0)/12.0)));
                        }
                        float volume = gatePin->data[j];
                        
                        this->oscillator[j]->setVolume(volume);
                    }
                    else {
                        if (oscillator[j] != nullptr) {
                            
                            delete this->oscillator[j];
                            this->oscillator[j] = nullptr;
                        }
                    }
                    if (oscillator[j] != nullptr)
                        value += oscillator[j]->process();
                }
            }
            if (out != nullptr && out->getNumChannels() > 0)
                out->setSample(0,i ,value);
            
            
            
        }
    }
    else {
        if (out != nullptr && out->getNumChannels() > 0) {
            out->clear();
        }
    }
    
    
}


