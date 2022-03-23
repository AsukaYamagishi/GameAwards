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
	};

public:
	static ModelManager *GetIns();		//取得用

	void Initialize();
	void LoadModel(const ModelName modelName, std::string fileName);
	ModelInput *GetModel(ModelName modelName) { return model[modelName]; }

private:
	static map<ModelName, ModelInput *> model;	//モデル格納マップ
};

