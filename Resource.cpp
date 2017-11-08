#include "Resource.h"

Resource::Resource(int UID, ResourceType type) : UID(UID), type(type)
{
}

Resource::~Resource()
{
}

ResourceType Resource::GetType() const
{
	return type;
}

int Resource::GetUID() const
{
	return UID;
}

const char * Resource::GetFile() const
{
	return file.c_str();
}

const char * Resource::GetExportedFile() const
{
	return exportedFile.c_str();
}

bool Resource::IsLoadedToMemory() const
{
	return references > 0;
}

void Resource::LoadToComponent()
{
	if (references == 0)
	{
		if (LoadInMemory() == false)
		{
			return;
		}
	}
	references++;
}

void Resource::UnloadFromComponent()
{
	references--;
	if (references == 0)
	{
		UnloadFromMemory();
	}
}

uint Resource::GetReferenceCount() const
{
	return references;
}

void Resource::Save(Configuration & config) const
{
}

void Resource::Load(Configuration & config)
{
}
