#ifndef _AUDIO_MANAGER_INCLUDE
#define _AUDIO_MANAGER_INCLUDE

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

struct ma_engine;

class AudioManager {
public:
   using SoundId = std::uint64_t;

    AudioManager();
    ~AudioManager();

    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    bool initialize();
    void shutdown();
    SoundId playSound(const std::string& path, bool loop = false);
    bool stopSound(SoundId id);
    bool isSoundPlaying(SoundId id) const;
    bool isSoundFinished(SoundId id) const;
    void update();
    void setMasterVolume(float volume);
    bool isInitialized() const;

private:
    struct Playback;
    ma_engine* engine = nullptr;
    SoundId nextSoundId = 1;
    std::vector<std::unique_ptr<Playback>> playbacks;
};

#endif // _AUDIO_MANAGER_INCLUDE
