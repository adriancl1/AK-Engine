#ifndef __ModuleAudio_H__
#define __ModuleAudio_H__

#include "Module.h"
#include "SDL_mixer\include\SDL_mixer.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

class ModuleAudio : public Module
{
public:
	ModuleAudio(Application* app, bool start_enabled = true);
	~ModuleAudio();

	bool Init(JSON_Object* data = nullptr);
	bool CleanUp(JSON_Object* data = nullptr);

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

	p2List<Mix_Chunk*>	fx;
};

#endif // __ModuleAudio_H__