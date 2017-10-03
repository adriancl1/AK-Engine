#ifndef __ModuleGeometryImporter_H__
#define __ModuleGeometryImporter_H__

#include "Globals.h"
#include "Module.h"

class ModuleGeometryImporter : public Module
{
public:
	ModuleGeometryImporter(Application* app, bool start_enabled = true);
	~ModuleGeometryImporter();

	bool Init(JSON_Object* data = nullptr);

	bool CleanUp(JSON_Object* data = nullptr);

	bool LoadMesh(const char* fullPath);
};

#endif //__ModuleGeometryImporter_H__
