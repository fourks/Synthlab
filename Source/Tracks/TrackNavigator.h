/*
  ==============================================================================

    WaveNavigator.h
    Created: 24 Nov 2016 8:04:54am
    Author:  Matthias Pueski

  ==============================================================================
*/

#ifndef WAVENAVIGATOR_H_INCLUDED
#define WAVENAVIGATOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveSelector.h"
#include "PositionMarker.h"
#include "Track.h"
#include "DragConstrainer.h"
#include "MultiComponentDragger.h"
#include "../Project/TrackConfig.h"
#include <vector>
#include <map>
//==============================================================================
/*
*/
class TrackNavigator    : public Component,
                          public ChangeListener,
                          public ChangeBroadcaster,
public KeyListener
{
public:
    TrackNavigator(PositionMarker* marker, AudioDeviceManager* manager);
    ~TrackNavigator();

    void paint (Graphics&) override;
    void resized() override;

    WaveSelector* getSelector();
    
    void clearTracks();
    
    Track* addTrack(TrackConfig* tc);
    void addTrack(Track* t);
    Track* addTrack(Track::Type type,double sampleRate);
	void removeSelectedTrack();
    Track* getCurrentTrack();
    std::vector<Track*> getTracks();
    
    bool isPlaying();
    void setPlaying(bool playing);
    
    bool isRecording();
    void setRecording(bool recording);
    
    double getMaxLength();
    void setPosition(double position);
    double getPosition();
    
	float getZoom();
    void setZoom(float zoom);
    
	void updateTrackLayout(ChangeBroadcaster * source);
	void adjustHeight();

    inline void addChangeListener (ChangeListener* listener) {
        ChangeBroadcaster::addChangeListener(listener);
    }
    
private:
    
    // ComponentDragger dragger;
    // DragConstrainer constrainer;
    
    MultiComponentDragger* dragger = NULL;
    AudioDeviceManager* deviceManager;
    
    Track* currentTrack = NULL;
    PositionMarker* marker;
    
    std::vector<Track*> tracks;
    ScopedPointer<WaveSelector> selector;

    bool playing = false;
    bool recording = false;
    double position;
    float zoom;
    // length of the track in seconds
    long tracklength;
    int mouseDeltaX = 0;
    
    long recordStart = 0;
    long recordStop = 0;
    
    MidiMessage message;
    
    virtual void changeListenerCallback(ChangeBroadcaster * source) override;
    virtual void mouseDown (const MouseEvent& event) override;
    virtual void mouseUp (const MouseEvent& event) override;
    virtual void mouseMove(const juce::MouseEvent &event) override;
    virtual bool keyPressed(const KeyPress& key, Component* originatingComponent) override;
    virtual void mouseDrag(const MouseEvent& event) override;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrackNavigator)
};


#endif  // WAVENAVIGATOR_H_INCLUDED
