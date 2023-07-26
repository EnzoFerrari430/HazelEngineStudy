#pragma once

#include "HazelAudio.h"
#include "SoundEffect.h"

#include "Hazel/Core/Log.h"

#include <memory>
#include <unordered_map>

class TetrisAudio {
public:

    static void Init() {

        m_SoundEffects[SoundEffect::Movement] = std::make_unique<Hazel::AudioSource>(Hazel::AudioSource::LoadFromFile("assets/audio/Movement.ogg"));

        m_SoundEffects[SoundEffect::Movement]->SetGain(0.1f);

        m_BackgroundMusic = std::make_unique<Hazel::AudioSource>(Hazel::AudioSource::LoadFromFile("assets/audio/BackgroundMusic.mp3"));
        m_BackgroundMusic->SetLoop(true);
        Hazel::Audio::Play(*m_BackgroundMusic);
    }

    static void PlaySound(SoundEffect sound) {
        HZ_ASSERT(m_SoundEffects[sound], "ERROR: SoundEffect has not been initialised");
        Hazel::Audio::Play(*m_SoundEffects[sound]);
    }

private:
    TetrisAudio() = delete;
    ~TetrisAudio() = delete;

    static std::unordered_map<SoundEffect, std::unique_ptr<Hazel::AudioSource>> m_SoundEffects;
    static std::unique_ptr<Hazel::AudioSource> m_BackgroundMusic;
};
