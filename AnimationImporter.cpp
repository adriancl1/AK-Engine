#include "AnimationImporter.h"
#include "Configuration.h"
#include "Application.h"
#include "ResourceAnimation.h"

#include "Assimp/include/cimport.h" 
#include "Assimp/include/scene.h" 
#include "Assimp/include/postprocess.h" 
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

AnimationImporter::AnimationImporter()
{
}

AnimationImporter::~AnimationImporter()
{
}

void AnimationImporter::Load(const char * inputFile, ResourceAnimation * anim)
{
	Configuration load(inputFile);

	if (load.IsValueValid() == true)
	{
		anim->duration = load.GetFloat("Duration");
		anim->ticksPerSecond = load.GetInt("TicksPerSecond");
		for (int i = 0; i < load.GetArraySize("Bones"); i++)
		{
			Configuration boneConf = load.GetArray("Bones", i);
			AnimBone tmpBone;
			tmpBone.name = boneConf.GetString("Name");
			for (int j = 0; j < boneConf.GetArraySize("Position Keys"); j++)
			{
				Configuration confPosKey = boneConf.GetArray("Position Keys", j);
				PositionKey tmpPosKey;
				tmpPosKey.time = confPosKey.GetFloat("Time");
				tmpPosKey.value.x = confPosKey.GetFloat("Position", 0);
				tmpPosKey.value.y = confPosKey.GetFloat("Position", 1);
				tmpPosKey.value.z = confPosKey.GetFloat("Position", 2);
				tmpBone.posKeys.push_back(tmpPosKey);
			}
			for (int j = 0; j < boneConf.GetArraySize("Rotation Keys"); j++)
			{
				Configuration confRotKey = boneConf.GetArray("Rotation Keys", j);
				RotationKey tmpRotKey;
				tmpRotKey.time = confRotKey.GetFloat("Time");
				tmpRotKey.value.x = confRotKey.GetFloat("Rotation", 0);
				tmpRotKey.value.y = confRotKey.GetFloat("Rotation", 1);
				tmpRotKey.value.z = confRotKey.GetFloat("Rotation", 2);
				tmpRotKey.value.w = confRotKey.GetFloat("Rotation", 3);
				tmpBone.rotKeys.push_back(tmpRotKey);
			}
			anim->bones.push_back(tmpBone);
		}
	}
}

bool AnimationImporter::Save(const aiAnimation * anim, const char * outputFile)
{
	Configuration save;
	save.SetString("Name", anim->mName.C_Str());
	save.SetFloat("Duration", anim->mDuration);
	save.SetInt("TicksPerSecond", anim->mTicksPerSecond);
	save.AddArray("Bones");
	for (int i = 0; i < anim->mNumChannels; i++)
	{
		Configuration boneConf;
		aiNodeAnim* tmp = anim->mChannels[i];
		boneConf.SetString("Name", tmp->mNodeName.C_Str());
		boneConf.AddArray("Position Keys");
		for (int j = 0; j < tmp->mNumPositionKeys; j++)
		{
			Configuration posKey;
			posKey.SetFloat("Time", tmp->mPositionKeys[j].mTime);
			posKey.AddArrayFloat("Position", &tmp->mPositionKeys[j].mValue.x, 3);
			boneConf.AddArrayEntry(posKey);
		}
		boneConf.AddArray("Rotation Keys");
		for (int j = 0; j < tmp->mNumRotationKeys; j++)
		{
			Configuration rotKey;
			rotKey.SetFloat("Time", tmp->mRotationKeys[j].mTime);
			rotKey.AddArrayFloat("Rotation", &tmp->mRotationKeys[j].mValue.x, 4);
			boneConf.AddArrayEntry(rotKey);
		}
		save.AddArrayEntry(boneConf);
	}

	char* buffer = nullptr;
	uint fileSize = save.SaveFile(&buffer, "Animation Save");
	App->fileSystem->SaveFile(outputFile, buffer, fileSize, FileType::fileAnimation);

	return true;
}
