#ifndef __ModuleAudio_H__
#define __ModuleAudio_H__

#include "Module.h"
#include "SDL_mixer/include/SDL_mixer.h"

#include <list>

#define DEFAULT_MUSIC_FADE_TIME 2.0f

class ModuleAudio : public Module
{
public:
	ModuleAudio(Application* app, bool start_enabled = true);
	~ModuleAudio();

	bool Init(Configuration data);
	bool CleanUp(Configuration data);

	void OnConfiguration()override;

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

	void PauseMusic();
	void ResumeMusic();

private:
	int volume;

	Mix_Music* music;

	bool pausedMusic;

	std::list<Mix_Chunk*> fx;
};

#endif // __ModuleAudio_H__