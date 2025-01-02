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

class EnvelopeFollower {
public:
private:
}

//a single band of the vocoder, containing an analysis band, an envelope follower
//and a filtering band
class VocoderBand {
public:
    float Process(float modulatorInput, float carrierInput);
private:
    juce::IIRCoefficients coef;
    juce::IIRFilterBase<juce::SpinLock> analyzer;
    juce::IIRFilterBase<juce::SpinLock> outFilter;
    
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
