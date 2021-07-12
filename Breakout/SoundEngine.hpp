//
//  SoundEngine.hpp
//  Breakout
//
//  Created by Apple on 11/07/21.
//

#ifndef SoundEngine_hpp
#define SoundEngine_hpp

#include <stdio.h>

#include <iostream>
#include <irrkLang/irrKlang.h>

using namespace irrklang;

// This class manages the audio sounds in the games
class SoundEngine{
public:
    // Functions
    SoundEngine();
    ~SoundEngine();
    void play2D(std::string fileName, bool loop);
    void drop();
private:
    irrklang::ISoundEngine *soundEngine;
};
#endif /* SoundEngine_hpp */
