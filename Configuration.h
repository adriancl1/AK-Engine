#pragma once

#include "parson\parson.h"

struct Color;

class Configuration
{
public:
	Configuration(const char* string);
	Configuration(JSON_Object* section);
	~Configuration();

	Configuration GetSection(const char* name) const;
	Configuration AddSection(const char* name);
	bool IsValueValid()const;

	bool GetBool(const char* fieldName)const;
	int GetInt(const char* fieldName)const;
	float GetFloat(const char* fieldName)const;
	const char* GetString(const char* fieldName)const;

	bool SetBool(const char* fieldName, bool value);
	bool SetInt(const char* fieldName, int value);
	bool SetFloat(const char* fieldName, float value);
	bool SetString(const char* fieldName, const char* value);
	bool SetColor(const char* fieldName, Color value);
	bool AddArrayFloat(const char* fieldName, const float* value, int size);

	bool SerializeToFile(const char* fileName);
	
private:
	JSON_Value* valueRoot = nullptr;
	JSON_Object* objectRoot = nullptr;
	JSON_Array* arrayConfig = nullptr;
};