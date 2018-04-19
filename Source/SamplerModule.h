/*
  ==============================================================================

    SamplerModule.h
    Created: 17 Apr 2018 3:34:36pm
    Author:  Matthias Pueski

  ==============================================================================
*/

#pragma once


#include "../JuceLibraryCode/JuceHeader.h"
#include "Module.h"

#include "AudioEngine/Sampler.h"

//==============================================================================
/*
 */
class SamplerModule : public Module, public Timer
{
public:
    SamplerModule(double sampleRate, int buffersize, AudioFormatManager* manager);
    ~SamplerModule();
    
    void paint (Graphics& g) override;
    
    void setAmplitude(float amplitude);
    void process() override;
    
    virtual void configurePins() override;
    virtual String getCategory() override {
        return "Sound sources";
    }
    
    virtual void timerCallback() override;
    virtual void eventReceived(Event *e) override;
    void loadSample(InputStream* is);
    void setSamplePath(String sample);
    String getSamplePath();
    void setPitch(float pitch);
    float getPitch();
    void updatePush2Display();
    
    AudioSampleBuffer* getBuffer();
    
    void selectSample(int i);
    
private:
    float samplePosX = 20;
    float pushSamplePosX = 0;
    AudioFormatManager* manager;
    AudioThumbnailCache* cache = nullptr;
    AudioThumbnail* thumbnail = nullptr;
    Sampler* sampler[128] = { nullptr };
    int currentSample = 0;
    int currentEnvelope = -1;
    bool gate = false;
    String samplePath;
    
    float* bufferLeft = nullptr;
    float* bufferRight = nullptr;
    
    float pitch = 1.0;
    
    virtual void setSampleRate(float rate) override;
    virtual void setBuffersize(int buffersize) override;
    
    CatmullRomInterpolator* interpolatorLeft;
    CatmullRomInterpolator* interpolatorRight;
    
    int currentSampler = 0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SamplerModule)
};