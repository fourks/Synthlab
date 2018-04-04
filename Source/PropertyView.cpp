/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.2.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "SynthEditor.h"

//[/Headers]

#include "PropertyView.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
PropertyView::PropertyView ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (tabbedComponent = new MainTabbedComponent (TabbedButtonBar::TabsAtTop));
    tabbedComponent->setTabBarDepth (30);
    tabbedComponent->setCurrentTabIndex (-1);

    tabbedComponent->setBounds (0, 0, 288, 632);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    setSize(getParentWidth(), getParentHeight());

    tabbedComponent->addTab("Properties",juce::Colours::grey,propertyPanel = new PropertyPanel(), true);

    properties = juce::Array<PropertyComponent*>();
    nameValue = new Value();
    numInputsValue = new Value();
    
    nameProp = new TextPropertyComponent(*nameValue,"name",16, true,true);
    inputsProp = new SliderPropertyComponent(*numInputsValue,"numInputs",0,16,1,1.0,true);
    
    nameListener = new NameListener(*nameValue, this);
    
    properties.add(nameProp);
    properties.add(inputsProp);
    
    propertyPanel->addProperties(properties);

    
    //[/Constructor]
}

PropertyView::~PropertyView()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    tabbedComponent = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    
    delete nameListener;
    delete nameValue;
    delete numInputsValue;

    //[/Destructor]
}

//==============================================================================
void PropertyView::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::grey);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void PropertyView::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void PropertyView::changeListenerCallback(juce::ChangeBroadcaster *source) {

    SynthEditor* editor = dynamic_cast<SynthEditor*>(source);

    if (editor != nullptr) {

        if (editor->getSelectedModule() != nullptr) {
            nameValue->setValue(editor->getSelectedModule()->getName());
            currentEditor = editor;
        }


    }


}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PropertyView" componentName=""
                 parentClasses="public Component, public ChangeListener" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff808080"/>
  <TABBEDCOMPONENT name="new tabbed component" id="5a5a0866ea539d50" memberName="tabbedComponent"
                   virtualName="MainTabbedComponent" explicitFocusOrder="0" pos="0 0 288 632"
                   orientation="top" tabBarDepth="30" initialTab="-1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
