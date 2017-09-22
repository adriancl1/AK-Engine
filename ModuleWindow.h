#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);

	SDL_Window* GetWindow() const;
	void ResizeWindow(int width, int height);
	void SetFullscreen(bool fullscreen);
	void SetBrightness(float value);
	void SetBorderless(bool borderless);
	void SetFullDesktop(bool fullDesktop);

public:
	//The surface contained by the window
	SDL_Surface* screen_surface;
private:
	//The window we'll be rendering to
	SDL_Window* window;
};

#endif // __ModuleWindow_H__