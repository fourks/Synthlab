//
//  PrefabFactory.cpp
//  Synthlab - App
//
//  Created by Matthias Pueski on 10.04.18.
//

#include "Project.h"
#include "PrefabFactory.h"
#include "MidiGate.h"
#include "MidiOut.h"
#include "MidiNote.h"
#include "SawtoothModule.h"
#include "NoiseModule.h"
#include "AudioOut.h"
#include "Constant.h"
#include "AdderModule.h"
#include "LPFilterModule.h"
#include "HPFilterModule.h"
#include "Knob.h"
#include "ADSRModule.h"
#include "SineModule.h"
#include "RandomModule.h"
#include "DelayModule.h"
#include "AddValueModule.h"
#include "DisplayValueModule.h"
#include "MultiplyModule.h"
#include "OscilloscopeModule.h"
#include "InverterModule.h"
#include "PitchToFrequencyModule.h"
#include "LabelModule.h"
#include "SamplerModule.h"

PrefabFactory* PrefabFactory::instance = NULL;

Module* PrefabFactory::getPrefab(int id, float sampleRate, int bufferSize) {

    Module* m = nullptr;
    
    if (prefabs[id].getName() == "Add signal") {
        m = new AdderModule(bufferSize);
    }
    else if (prefabs[id].getName() == "Pitch to freq") {
        m = new PitchToFrequencyModule();
    }
    else if (prefabs[id].getName() == "Add value") {
        m = new AddValueModule();
    }
    else if (prefabs[id].getName() == "Multiply value") {
        m = new MultiplyModule();
    }
    else if (prefabs[id].getName() == "Invert value") {
        m = new InverterModule();
    }
    else if (prefabs[id].getName() == "Midi gate") {
        m = new MidiGate();
        m->setName("Gate");
    }
    else if (prefabs[id].getName() == "Midi out") {
        m = new MidiOut();
        m->setName("Midi Out");
    }
    else if (prefabs[id].getName() == "Midi note") {
        m = new MidiNote();
        m->setName("Note");
    }
    else if (prefabs[id].getName() == "Sawtooth") {
        m = new SawtoothModule(sampleRate, bufferSize);
    }
    else if (prefabs[id].getName() == "Sine") {
        m = new SineModule(sampleRate, bufferSize);
    }
    else if (prefabs[id].getName() == "Random") {
        m = new RandomModule(sampleRate, bufferSize);
    }
    else if (prefabs[id].getName() == "White noise") {
        m = new NoiseModule(sampleRate, bufferSize);
    }
    else if (prefabs[id].getName() == "Audio out") {
        m = new AudioOut();
    }
    else if (prefabs[id].getName() == "Constant") {
        m = new Constant();
    }
    else if (prefabs[id].getName() == "Knob") {
        m = new Knob();
    }
    else if (prefabs[id].getName() == "Label") {
        m = new LabelModule("Label");
    }
    else if (prefabs[id].getName() == "LP filter") {
        m = new LPFilterModule(sampleRate,bufferSize);
    }
    else if (prefabs[id].getName() == "HP filter") {
        m = new HPFilterModule(sampleRate,bufferSize);
    }
    else if (prefabs[id].getName() == "ADSR") {
        m = new ADSRModule(sampleRate,bufferSize);
    }
    else if (prefabs[id].getName() == "Stereo delay") {
        m = new DelayModule(sampleRate,bufferSize);
    }
    else if (prefabs[id].getName() == "Display value") {
        m = new DisplayValueModule();
    }
    else if (prefabs[id].getName() == "Scope") {
        m = new OscilloscopeModule(sampleRate,bufferSize);
    }
    else if (prefabs[id].getName() == "Sampler") {
        m = new SamplerModule(sampleRate,bufferSize,Project::getInstance()->getFormatManager());
    }
    
    m->setIndex(id + 100 * prefabs[id].getNumInstances());
    m->setId(id);
    prefabs[id].addInstance();
    m->configurePins();
    
    return m;
}

void PrefabFactory::reset() {
    for (std::map<int,Prefab>::iterator it = prefabs.begin();it != prefabs.end();it++) {
        (*it).second.reset();
    }
}
