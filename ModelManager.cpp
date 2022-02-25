#include "ModelManager.h"

map<ModelManager::ModelName, ModelInput *> ModelManager::model;

ModelManager *ModelManager::GetIns()
{
	static ModelManager instans;
	return &instans;
}

void ModelManager::Initialize()
{
	LoadModel(Player, "player");
	//LoadModel(TestDunut, "donut");
}

void ModelManager::LoadModel(const ModelName modelName, std::string fileName)
{
	model[modelName] = new ModelInput();
	model[modelName]->InitializeDiscriptorHeap();
	model[modelName]->CreateFromObj(fileName);

}
