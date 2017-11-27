#include "Globals.h"
#include "Application.h"

enum GuiType 
{ 
	BUTTON,
	TEXT, 
	TEXT_BOX, 
	MOUSE, 
	IMAGE, 
	DIALOGUE,
	MENU 
};

enum UI_STATE
{
	MAIN_MENU,
	HUD,

};

class ModuleUI : Module {

public:

	ModuleUI(Application* app, bool start_enabled);
	virtual ~ModuleUI();

	//bool Awake(); uncomment if necessary
	bool Start();
	bool PreUpdate();
	void ReceiveInput();
	bool PostUpdate();
	bool CleanUp();

	void Erase(); //fill later
	const SDL_Texture* GetAtlas();
	void SaveUI();


};
