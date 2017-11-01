#pragma once

#include "parson\parson.h"

struct Color;

class Configuration
{
public:
	Configuration();
	Configuration(const char* string);
	Configuration(JSON_Object* section);
	~Configuration();

	Configuration GetSection(const char* name) const;
	Configuration AddSection(const char* name);
	JSON_Value* GetValue(const char* field, int count) const;
	bool IsValueValid()const;

	bool GetBool(const char* fieldName)const;
	int GetInt(const char* fieldName)const;
	float GetFloat(const char* fieldName, int count = -1)const;
	const char* GetString(const char* fieldName)const;
	int GetArraySize(const char* fieldName)const;
	Configuration GetArray(const char* fieldName, int count)const;

	bool SetBool(const char* fieldName, bool value);
	bool SetInt(const char* fieldName, int value);
	bool SetFloat(const char* fieldName, float value);
	bool SetString(const char* fieldName, const char* value);
	bool SetColor(const char* fieldName, Color value);
	bool AddArrayFloat(const char* fieldName, const float* value, int size);
	bool AddArray(const char* name);
	bool AddArrayEntry(const Configuration& toAdd);

	bool SerializeToFile(const char* fileName);
	size_t SaveFile(char** buffer, const char* comment) const;
	
private:
	JSON_Value* valueRoot = nullptr;
	JSON_Object* objectRoot = nullptr;
	JSON_Array* arrayConfig = nullptr;
};