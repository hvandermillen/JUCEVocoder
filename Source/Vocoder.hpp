//
//  Vocoder.hpp
//  BasicVocoder - VST3
//
//  Created by Henry Vandermillen on 1/1/25.
//

#pragma once

#ifndef Vocoder_hpp
#define Vocoder_hpp

#include <stdio.h>
#include <JuceHeader.h>

#endif /* Vocoder_hpp */

#define ENV_ATTACK 500
#define ENV_RELEASE 500
#define Q 2


class EnvelopeFollower {
public:
    void Init(int attack, int release);
    float Process(float input);
    float getLevel();
private:
    int attack; //in samples
    int release; //in samples
    float level;
    int count = 0;
    bool state = true; //true = attack, false = release
};

//a single band of the vocoder, containing an analysis band, an envelope follower
//and a filtering band
class VocoderBand {
public:
    void Init(float sampleRate, float frequency);
    float Process(float modulatorInput, float carrierInput);
private:
    juce::IIRCoefficients coef;
    juce::IIRFilterBase<juce::SpinLock> analyzer;
    juce::IIRFilterBase<juce::SpinLock> outFilter;
    EnvelopeFollower env;
    float frequency;
};

class Vocoder {
public:
private:
    
};

/*
 needed:
 - int n = number of bands
 - array containing n analysis bands (filters)
 - array containing n filtering bands (filters)
 - array containing n envelope followers
 */
