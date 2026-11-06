#define MINIAUDIO_IMPLEMENTATION
#include "AudioManager.h"
#include "libs/miniaudio/miniaudio.h"

struct AudioManager::Playback {
    ma_sound sound;
    SoundId id;
    bool loop;
    bool initialized = false;

    Playback(SoundId id, bool loop) : id(id), loop(loop) {}
    ~Playback() {
        if (initialized)
            ma_sound_uninit(&sound);
    }
};

AudioManager::AudioManager() = default;

AudioManager::~AudioManager() {
    shutdown();
}

bool AudioManager::initialize() {
    if (engine != nullptr)
        return true;

    ma_engine* newEngine = new (std::nothrow) ma_engine();
    if (newEngine == nullptr)
        return false;

    if (ma_engine_init(nullptr, newEngine) != MA_SUCCESS) {
        delete newEngine;
        return false;
    }

    engine = newEngine;
    return true;
}

void AudioManager::shutdown() {
    if (engine == nullptr)
        return;

   playbacks.clear();
    ma_engine_uninit(engine);
    delete engine;
    engine = nullptr;
}

AudioManager::SoundId AudioManager::playSound(const std::string& name, bool loop) {
	const std::string path = "audio/" + name + ".mp3";

    if (engine == nullptr || path.empty())
        return 0;

    auto playback = std::make_unique<Playback>(nextSoundId++, loop);
    if (ma_sound_init_from_file(engine, path.c_str(), MA_SOUND_FLAG_STREAM, nullptr, nullptr, &playback->sound) != MA_SUCCESS)
        return 0;
    playback->initialized = true;

    ma_sound_set_looping(&playback->sound, loop ? MA_TRUE : MA_FALSE);
    if (ma_sound_start(&playback->sound) != MA_SUCCESS)
        return 0;

    const SoundId id = playback->id;
    playbacks.push_back(std::move(playback));
    return id;
}

bool AudioManager::stopSound(SoundId id) {
    for (auto it = playbacks.begin(); it != playbacks.end(); ++it) {
        if ((*it)->id == id) {
            ma_sound_stop(&(*it)->sound);
            playbacks.erase(it);
            return true;
        }
    }
    return false;
}

bool AudioManager::isSoundPlaying(SoundId id) const {
    for (const auto& playback : playbacks) {
        if (playback->id == id)
            return ma_sound_is_playing(&playback->sound) == MA_TRUE;
    }
    return false;
}

bool AudioManager::isSoundFinished(SoundId id) const {
    for (const auto& playback : playbacks) {
        if (playback->id == id)
            return !playback->loop && ma_sound_at_end(&playback->sound) == MA_TRUE;
    }
    return true;
}

void AudioManager::update() {
    playbacks.erase(std::remove_if(playbacks.begin(), playbacks.end(), [](const std::unique_ptr<Playback>& playback) {
        return !playback->loop && ma_sound_at_end(&playback->sound) == MA_TRUE;
    }), playbacks.end());
}

void AudioManager::setMasterVolume(float volume) {
    if (engine == nullptr)
        return;
    if (volume < 0.f)
        volume = 0.f;
    else if (volume > 1.f)
        volume = 1.f;
    ma_engine_set_volume(engine, volume);
}

bool AudioManager::isInitialized() const {
    return engine != nullptr;
}
