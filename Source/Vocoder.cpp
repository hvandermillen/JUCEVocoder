//
//  Vocoder.cpp
//  BasicVocoder - VST3
//
//  Created by Henry Vandermillen on 1/1/25.
//

#include "Vocoder.hpp"

void EnvelopeFollower::Init(int attack, int release) {
    this->attack = attack;
    this->release = release;
    this->level = 0;
}

//this should be improved later
float EnvelopeFollower::Process(float input) {
    float absInput = std::abs(input);
    if (state) {
        if (absInput > level) {
            level += (absInput - level)/attack;
            count++;
        } else {
            state = false;
            count = 0;
        }
    } else if (!state) {
        if (absInput < level) {
            level -= (level-absInput)/release;
            count++;
        } else {
            state = true;
            count = 0;
        }
    }
    
    std::cout<<level;
    
    return level;
}

float EnvelopeFollower::getLevel() {
    return level;
}

void VocoderBand::Init(float sampleRate, float frequency) {
    //initialize analysis and filter IIRs to bandpasses at the correct frequency
    coef = juce::IIRCoefficients::makeBandPass(sampleRate, frequency, Q);
    analyzer.setCoefficients(coef);
    outFilter.setCoefficients(coef);
    env.Init(ENV_ATTACK,ENV_RELEASE);
}

float VocoderBand::Process(float modulatorInput, float carrierInput) {
    //filter input to isolate band
    float inputFiltered = analyzer.processSingleSampleRaw(modulatorInput);
    
    //process envelope follower to get level
    float modulatorLevel = env.Process(inputFiltered);
    
    //filter carrier and adjust level to match modulator
    float outputFiltered = outFilter.processSingleSampleRaw(carrierInput);
    float makeupGain = 10;
    outputFiltered *= modulatorLevel * makeupGain;
    
    return outputFiltered; //CHANGE
}

void Vocoder::Init(float sampleRate) {
    //inintialize all the bands
    for (int i = 0; i < numBands; i++) {
        bands[i].Init(sampleRate, frequencies[i]);
    }
}

float Vocoder::Process(float modulatorInput, float carrierInput) {
    float out = 0;
    
    for (int i = 0; i < numBands; i++) {
        out += bands[i].Process(modulatorInput, carrierInput) * 0.5 * log(i+1);
    }
    return out; //CHANGE
}

/*
 The higher I set the buffer size, the worse it sounds!
 Probably doing too much in that one loop
 */
