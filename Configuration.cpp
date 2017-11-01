#include "Configuration.h"
#include "Color.h"

Configuration::Configuration()
{
	valueRoot = json_value_init_object();
	objectRoot = json_value_get_object(valueRoot);
}

Configuration::Configuration(const char* string)
{
	valueRoot = json_parse_file(string);
	objectRoot = json_value_get_object(valueRoot);
}

Configuration::Configuration(JSON_Object* section)
{
	objectRoot = section;
}

Configuration::~Configuration()
{
	if (valueRoot != nullptr)
	{
		json_value_free(valueRoot);
	}
}

Configuration Configuration::GetSection(const char * name) const
{
	return Configuration(json_object_dotget_object(objectRoot, name));
}

Configuration Configuration::AddSection(const char * name)
{
	json_object_set_value(objectRoot, name, json_value_init_object());
	return GetSection(name);
}

JSON_Value * Configuration::GetValue(const char * field, int count) const
{
	if (count == -1)
		return json_object_get_value(objectRoot, field);

	JSON_Array* array = json_object_get_array(objectRoot, field);
	if (array != nullptr)
	{
		return json_array_get_value(array, count);
	}

	return nullptr;
}

bool Configuration::IsValueValid() const
{
	if (valueRoot != nullptr && valueRoot != NULL && objectRoot != nullptr)
	{
		return true;
	}
	return false;
}

bool Configuration::GetBool(const char * fieldName) const
{
	if (objectRoot != nullptr)
	{
		return json_object_dotget_boolean(objectRoot, fieldName);
	}
	return false;
}

int Configuration::GetInt(const char * fieldName) const
{
	if (objectRoot != nullptr)
	{
		return (int)json_object_dotget_number(objectRoot, fieldName);
	}
	return 0;
}

float Configuration::GetFloat(const char * fieldName, int count) const
{
	JSON_Value* value = GetValue(fieldName, count);

	if (value && json_value_get_type(value) == JSONNumber)
	{
		return (float)json_value_get_number(value);
	}

	return 0.0f;
}

const char * Configuration::GetString(const char * fieldName) const
{
	if (objectRoot != nullptr)
	{
		return json_object_dotget_string(objectRoot, fieldName);
	}
	return nullptr;
}

int Configuration::GetArraySize(const char * fieldName) const
{
	JSON_Array* array = json_object_get_array(objectRoot, fieldName);
	if (array != nullptr)
	{
		return json_array_get_count(array);
	}
	
	return -1;
}

Configuration Configuration::GetArray(const char * fieldName, int count) const
{
	JSON_Array* tmpArray = json_object_get_array(objectRoot, fieldName);
	if (tmpArray != nullptr)
	{
		return Configuration(json_array_get_object(tmpArray, count));
	}
	else
	{
		return Configuration((JSON_Object*) nullptr);
	}	
}

bool Configuration::SetBool(const char * fieldName, bool value)
{
	if (json_object_dotset_boolean(objectRoot, fieldName, value) == JSONSuccess)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Configuration::SetInt(const char * fieldName, int value)
{
	if (json_object_dotset_number(objectRoot, fieldName, value) == JSONSuccess)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Configuration::SetFloat(const char * fieldName, float value)
{
	if (json_object_dotset_number(objectRoot, fieldName, value) == JSONSuccess)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Configuration::SetString(const char * fieldName, const char * value)
{
	if (json_object_dotset_string(objectRoot, fieldName, value) == JSONSuccess)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Configuration::SetColor(const char * fieldName, Color value)
{
	if (AddArrayFloat("fieldName", &value, 4))
	{
		return true;
	}
	return false;
}

bool Configuration::AddArrayFloat(const char * fieldName, const float * value, int size)
{
	if (value != nullptr && size > 0)
	{
		JSON_Value* tmp = json_value_init_array();
		arrayConfig = json_value_get_array(tmp);
		json_object_set_value(objectRoot, fieldName, tmp);

		for (int i = 0; i < size; ++i)
		{
			if (json_array_append_number(arrayConfig, value[i]) != JSONSuccess)
			{
				return false;
			}
		}
	}
	return false;
}

bool Configuration::AddArray(const char * name)
{
	bool ret = false;

	JSON_Value* tmpArray = json_value_init_array();
	arrayConfig = json_value_get_array(tmpArray);

	if (json_object_set_value(objectRoot, name, tmpArray) == JSONSuccess)
	{
		ret = true;
	}
	
	return ret;
}

bool Configuration::AddArrayEntry(const Configuration & toAdd)
{
	bool ret = false;

	if (arrayConfig != nullptr)
	{
		if (json_array_append_value(arrayConfig, json_value_deep_copy(toAdd.valueRoot)) == JSONSuccess)
		{
			ret = true;
		}
	}

	return ret;
}

bool Configuration::SerializeToFile(const char * fileName)
{
	bool ret = false;
	if (valueRoot != nullptr)
	{
		if (json_serialize_to_file(valueRoot, fileName) == JSONSuccess)
		{
			ret = true;
		}
	}
	return ret;
}

size_t Configuration::SaveFile(char ** buffer, const char * comment) const
{
	size_t written = json_serialization_size(valueRoot);
	*buffer = new char[written];
	json_serialize_to_buffer(valueRoot, *buffer, written);
	return written;
}
