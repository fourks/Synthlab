/*
  ==============================================================================

    Constant
    Created: 4 Apr 2018 3:48:23pm
    Author:  Matthias Pueski

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Constant.h"


//==============================================================================
Constant::Constant()
{
   
    setSize(120,30);
    nameLabel->setJustificationType (Justification::left);
    nameLabel->setTopLeftPosition(2,2);

    editable = false;
    prefab = true;
}

Constant::~Constant()
{

}

void Constant::configurePins(){
    Pin* p1 = new Pin(Pin::Type::VALUE);
    p1->direction = Pin::Direction::OUT;
    p1->setName("V");
    addPin(Pin::Direction::OUT,p1);

}

void Constant::paint(juce::Graphics &g) {
    Module::paint(g);
}

float Constant::getValue() {
    return value;
}

void Constant::setValue(float value) {
    this->value = value;
    this->pins.at(0)->setValue(value);
    this->nameLabel->setText(name +" = "+String(value),juce::NotificationType::dontSendNotification);
}

void Constant::setEditing(bool editing) {
    
    if (editing) {
        if (!this->editing) {
            this->editing = true;
            nameLabel->setVisible(false);
            nameEditor->setVisible(true);
            nameEditor->setText(getName());
            setInterceptsMouseClicks(false,true);
            nameEditor->setWantsKeyboardFocus(true);
            nameEditor->setSelectAllWhenFocused(true);
            nameEditor->grabKeyboardFocus();
        }
    }
    else {
        if (this->editing) {
            this->editing = false;
            setName(nameEditor->getText());
            this->nameLabel->setText(name +" = "+String(value),juce::NotificationType::dontSendNotification);
            nameLabel->setVisible(true);
            nameEditor->setVisible(false);
            setInterceptsMouseClicks(false,false);
            nameEditor->setWantsKeyboardFocus(false);
        }
    }

}

