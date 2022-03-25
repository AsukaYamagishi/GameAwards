#include "ModelManager.h"

map<ModelManager::ModelName, ModelInput *> ModelManager::model;

ModelManager *ModelManager::GetIns()
{
	static ModelManager instans;
	return &instans;
}

void ModelManager::Initialize()
{
<<<<<<< HEAD
	//LoadModel(Player, "player");
=======
	LoadModel(Player, "player_human");
	LoadModel(Stage, "stage");
	LoadModel(Skydome, "skydome");
	LoadModel(Weapon, "weapon");

	LoadModel(Face, "face");
	LoadModel(Body, "Upper_body");
	LoadModel(Right_arm, "Right_arm");
	LoadModel(Left_arm, "Left_arm");
	LoadModel(Leftleg, "Leftleg");
	LoadModel(Rightleg, "Rightleg");

	LoadModel(Plane, "plane");
>>>>>>> 33627d828452657bf52159acf9193d675f1e7812
	//LoadModel(TestDunut, "donut");
}

void ModelManager::LoadModel(const ModelName modelName, std::string fileName)
{
	model[modelName] = new ModelInput();
	model[modelName]->InitializeDiscriptorHeap();
	model[modelName]->CreateFromObj(fileName);

}
