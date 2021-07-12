//
//  SoundEngine.cpp
//  Breakout
//
//  Created by Apple on 11/07/21.
//

#include "SoundEngine.hpp"

//pragma comment(lib, "irrKlang.lib")

SoundEngine::SoundEngine(){
    soundEngine = irrklang::createIrrKlangDevice();
}
SoundEngine::~SoundEngine(){
    
}
void SoundEngine::play2D(std::string fileName, bool loop){
    soundEngine->play2D(fileName.c_str(), loop);
}

void SoundEngine::drop(){
    soundEngine->drop();
}
