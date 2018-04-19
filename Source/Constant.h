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

//==============================================================================
/*
*/
class Constant    : public Module
{
public:
    Constant();
    ~Constant();

    void paint (Graphics& g) override;

    void setValue(float value);
    float getValue();
    virtual void configurePins() override;

    virtual void setName(String name)  {
        //Module::setName(name);
        Component::setName(name);
        this->name = name;
        this->nameLabel->setText(name +" = "+String(value),juce::NotificationType::dontSendNotification);
    }
    
    virtual void setEditing(bool editing) override;
    
    virtual String getName()  {
        return name;
    }
    
    virtual String getCategory() override {
        return "Math";
    }
    
private:
    
    float value = 0;
    String name;
    bool editing = false;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Constant)
};
