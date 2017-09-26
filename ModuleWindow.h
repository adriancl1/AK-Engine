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

	bool Init(JSON_Object* data = nullptr);
	bool CleanUp(JSON_Object* data = nullptr);

	const char* GetTitle()const;
	void SetTitle(const char* title);

	SDL_Window* GetWindow() const;
	void ResizeWindow(int width, int height);
	void GetWindowSize(int& width, int& height);
	void SetFullscreen(bool fullscreen);
	bool GetFullscreen();
	void SetBrightness(float value);
	float GetBrightness();
	void SetBorderless(bool borderless);
	bool GetBorderless();
	void SetFullDesktop(bool fullDesktop);
	bool GetFullDesktop();

public:
	//The surface contained by the window
	SDL_Surface* screen_surface;
private:
	//The window we'll be rendering to
	SDL_Window* window;

	std::string title;
	int width;
	int height;
	float brightness;
	bool fullscreen;
	bool fullDesktop;
	bool borderless;
};

#endif // __ModuleWindow_H__