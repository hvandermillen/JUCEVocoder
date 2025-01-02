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
    float inputFiltered = analyzer.processSingleSampleRaw(modulatorInput);
    
    //process envelope follower to get level
    float modulatorLevel = env.Process(modulatorInput);

    
    return carrierInput; //CHANGE
}
