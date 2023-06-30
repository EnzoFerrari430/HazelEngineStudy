#pragma once

#include "HazelAudio.h"

#include "Hazel/Core/Log.h"

#include <memory>
#include <unordered_map>

class TetrisAudio {
public:

    static void Init() {

        m_BackgroundMusic = std::make_unique<Hazel::AudioSource>(Hazel::AudioSource::LoadFromFile("assets/audio/BackgroundMusic.mp3"));
        m_BackgroundMusic->SetLoop(true);
        Hazel::Audio::Play(*m_BackgroundMusic);
    }



private:
    TetrisAudio() = delete;
    ~TetrisAudio() = delete;

    static std::unique_ptr<Hazel::AudioSource> m_BackgroundMusic;
};
