#include "RigImporter.h"
#include "Application.h"
#include "ResourceRig.h"
#include "ModuleFileSystem.h"
#include "Configuration.h"

#include "MathGeo/Math/Quat.h"
#include "Glew\include\glew.h"
#include "Assimp\include\cimport.h" 
#include "Assimp\include\scene.h" 
#include "Assimp\include\postprocess.h" 
#include "Assimp\include\cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

RigImporter::RigImporter()
{
}

RigImporter::~RigImporter()
{
}

void RigImporter::Load(const char * inputFile, ResourceRig * rig)
{
	Configuration load(inputFile);

	if (load.IsValueValid() == true)
	{
		for (int i = 0; i < load.GetArraySize("Bones"); i++)
		{
			Configuration confBone = load.GetArray("Bones", i);
			Bone tmpBone;
			tmpBone.name = confBone.GetString("Name");

			float3 pos;
			float3 sca;
			Quat rot;
			pos.x = confBone.GetFloat("Position", 0);
			pos.y = confBone.GetFloat("Position", 1);
			pos.z = confBone.GetFloat("Position", 2);
			sca.x = confBone.GetFloat("Scale", 0);
			sca.y = confBone.GetFloat("Scale", 1);
			sca.z = confBone.GetFloat("Scale", 2);
			rot.x = confBone.GetFloat("Rotation", 0);
			rot.y = confBone.GetFloat("Rotation", 1);
			rot.z = confBone.GetFloat("Rotation", 2);
			rot.w = confBone.GetFloat("Rotation", 3);
			tmpBone.offsetMatrix = float4x4::FromQuat(rot);
			tmpBone.offsetMatrix = float4x4::Scale(sca, float3(0, 0, 0)) * tmpBone.offsetMatrix;
			tmpBone.offsetMatrix.float4x4::SetTranslatePart(pos.x, pos.y, pos.z);

			for (int i = 0; i < confBone.GetArraySize("Vertex Weights"); i++)
			{
				Configuration confWeight = confBone.GetArray("Vertex Weights", i);

				VertexWeight tmpWeight;
				tmpWeight.vertexID = confWeight.GetInt("VertexID");
				tmpWeight.weight = confWeight.GetFloat("Weight");

				tmpBone.weights.push_back(tmpWeight);
			}
			
			rig->bones.push_back(tmpBone);
		}
	}
}

bool RigImporter::Save(const aiMesh * mesh, const char * outputFile)
{
	Configuration save;
	save.AddArray("Bones");
	for (int i = 0; i < mesh->mNumBones; i++)
	{
		Configuration boneConf;
		boneConf.SetString("Name", mesh->mBones[i]->mName.C_Str());

		aiVector3D translation;
		aiVector3D scale;
		aiQuaternion rotation;

		mesh->mBones[i]->mOffsetMatrix.Decompose(scale, rotation, translation);

		float3 pos(translation.x, translation.y, translation.z);
		float3 sca(scale.x, scale.y, scale.z);
		Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);

		boneConf.AddArrayFloat("Position", &pos.x, 3);
		boneConf.AddArrayFloat("Rotation", &rot.x, 4);
		boneConf.AddArrayFloat("Scale", &sca.x, 3);

		boneConf.AddArray("Vertex Weights");
		for (int j = 0; j < mesh->mBones[i]->mNumWeights; j++)
		{
			Configuration weightConf;
			weightConf.SetInt("VertexID", mesh->mBones[i]->mWeights[j].mVertexId);
			weightConf.SetFloat("Weight", mesh->mBones[i]->mWeights[j].mWeight);
			boneConf.AddArrayEntry(weightConf);
		}

		save.AddArrayEntry(boneConf);

		/*Bone tmpBone;
		tmpBone.name = mesh->mBones[i]->mName.C_Str();

		aiVector3D translation;
		aiVector3D scale;
		aiQuaternion rotation;

		mesh->mBones[i]->mOffsetMatrix.Decompose(scale, rotation, translation);

		float3 pos(translation.x, translation.y, translation.z);
		float3 sca(scale.x, scale.y, scale.z);
		Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);

		tmpBone.offsetMatrix = float4x4::FromQuat(rot);
		tmpBone.offsetMatrix = float4x4::Scale(sca, float3(0, 0, 0)) * tmpBone.offsetMatrix;
		tmpBone.offsetMatrix.float4x4::SetTranslatePart(pos.x, pos.y, pos.z);

		for (int j = 0; i < mesh->mBones[i]->mNumWeights; j++)
		{
			VertexWeight tmpWeight;
			tmpWeight.weight = mesh->mBones[i]->mWeights[j].mWeight;
			tmpWeight.vertexID = mesh->mBones[i]->mWeights[j].mVertexId;
			tmpBone.weights.push_back(tmpWeight);
		}
		newRig->bones.push_back(tmpBone);*/
	}

	char* buffer = nullptr;
	uint fileSize = save.SaveFile(&buffer, "Rig Save");
	App->fileSystem->SaveFile(outputFile, buffer, fileSize, FileType::fileRig);

	return true;
}
