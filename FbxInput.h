#pragma once

#include "fbxsdk.h"
#include "FbxModel.h"

#include <d3d12.h>
#include <d3dx12.h>
#include <string>

class FbxInput
{
private: //エイリアス
	using string = std::string;

public:
	// シングルトンインスタンスの取得
	static FbxInput* GetInstance();
	//FBXの行列をXMMATRIXに変換
	static void ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src);

	//初期化
	void Init(ID3D12Device* dev);
	//後始末
	void Fin();
	//ファイルからFBXモデル読み込み
	std::unique_ptr<FbxModel> LoadFbxFromFile(const string& modelName);


private:
	// privateなコンストラクタ（シングルトンパターン）
	FbxInput() = default;
	// privateなデストラクタ（シングルトンパターン）
	~FbxInput() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	FbxInput(const FbxInput & obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const FbxInput & obj) = delete;

	//再帰的にノード構成を解析
	void ParseNodeRecursive(FbxModel* model, FbxNode* fbxNode, Node* parent = nullptr);
	//メッシュ読み取り
	void ParseMesh(FbxModel* model, FbxNode* fbxNode);
#pragma region メッシュ解析用サブ関数
	//頂点座標読み取り
	void ParseMeshVertices(FbxModel* model, FbxMesh* fbxMesh);
	//面情報読み取り
	void ParseMeshFaces(FbxModel* model, FbxMesh* fbxMesh);
	//マテリアル読み取り
	void ParseMaterial(FbxModel* model, FbxNode* fbxNode);
	//テクスチャ読み込み
	void LoadTexture(FbxModel* model, const std::string& fullpath);
	//スキニング情報の読み取り
	void ParseSkin(FbxModel* model, FbxMesh* fbxMesh);
#pragma endregion
	//ディレクトリを含んだファイルパスからファイル名を抽出する
	std::string ExtractFileName(const std::string& path);



public: //定数
	//モデル格納ルートパス
	static const string baseDirectory;
	//テクスチャが無い場合の標準テクスチャファイル名
	static const string defaultTexFileName;

private: //変数
	//D3D12デバイス
	ID3D12Device* dev = nullptr;
	//FBXマネージャ
	FbxManager* fbxManager = nullptr;
	//FBXインポータ
	FbxImporter* fbxImporter = nullptr;
	// コントロールポイントのデータ
	std::vector<std::vector<int>> controlPointsData;
};