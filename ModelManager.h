#pragma once
#include <map>
#include <string>
#include "ModelInput.h"
class ModelManager final
{
private:
	ModelManager() = default;
	~ModelManager() = default;
	ModelManager(const ModelManager &r) = default;
	ModelManager &operator= (const ModelManager &r) = default;

public:
	enum ModelName {
		Player,
		Stage,
		Skydome,
		Weapon,
		Face,
		Body,
		Right_arm,
		Left_arm,
		Rightleg,
		Leftleg,
		Plane,
		Bullet
	};

public:
	static ModelManager *GetIns();		//�擾�p

	void Initialize();
	void LoadModel(const ModelName modelName, std::string fileName);
	ModelInput *GetModel(ModelName modelName) { return model[modelName]; }

private:
	static map<ModelName, ModelInput *> model;	//���f���i�[�}�b�v
};

