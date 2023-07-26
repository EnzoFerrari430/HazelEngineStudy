#include "TetrisAudio.h"

std::unordered_map<SoundEffect, std::unique_ptr<Hazel::AudioSource>> TetrisAudio::m_SoundEffects;
std::unique_ptr<Hazel::AudioSource> TetrisAudio::m_BackgroundMusic;
