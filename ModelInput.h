#pragma once
#include <d3dx12.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <wrl.h>

using namespace std;
using namespace DirectX;

class ModelInput
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;	//Microsoft::WRL::を省略
public:		//サブクラス

	//頂点データ構造体
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;  // uv座標
	};

	//マテリアル
	struct Material {
		string name;		//マテリアル名
		XMFLOAT3 ambient;	//アンビエント影響度
		XMFLOAT3 diffuse;	//ディフューズ影響度
		XMFLOAT3 specular;	//スペキュラー影響度
		float alpha;		//アルファ
		string textureFilename;	//テクスチャファイル名

		// コンストラクタ
		Material() {
			ambient = { 0.3f, 0.3f, 0.3f };
			diffuse = { 0.0f, 0.0f, 0.0f };
			specular = { 0.0f, 0.0f, 0.0f };
			alpha = 1.0f;
		}
	};

public:		//静的メンバ関数
	static bool StaticInitialize(ID3D12Device *dev);


public:		//メンバ関数
	bool InitializeDiscriptorHeap();
	bool CreateFromObj(string modelName);		//モデル生成
	bool LoadMaterial(const string &directoryPath, const string &filename);		//マテリアル読み込み
	bool LoadTexture(const string &directoryPath, const string &filename);		//テクスチャ読み込み


	void Draw(ID3D12GraphicsCommandList *cmdList, ComPtr<ID3D12Resource> constBuffB0, ComPtr<ID3D12Resource> constBuffB1);								//描画

	Material GetMaterial() { return material; }

private:		//静的メンバ変数
	static ID3D12Device *dev;								//デバイス
	/*static*/ CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;	//シェーダリソースビューハンドル(GPU)
	/*static*/ CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;	//シェーダリソースビューハンドル(CPU)
	static UINT descHandleIncrementSize;					//デスクリプタサイズ

private:		//メンバ変数
	ComPtr<ID3D12DescriptorHeap> descHeap;			//テクスチャデスクリプタ
	ComPtr<ID3D12Resource> vertBuff;			//頂点バッファ
	D3D12_VERTEX_BUFFER_VIEW vbView;			//頂点バッファビュー
	ComPtr<ID3D12Resource> indexBuff;			//インデックスバッファ
	D3D12_INDEX_BUFFER_VIEW ibView;				//インデックスバッファビュー
	std::vector<VertexPosNormalUv> vertices;	//頂点データ配列
	std::vector<unsigned short> indices;		//インデックス配列
	Material material;		//マテリアル

	ComPtr<ID3D12Resource> texBuff;	//テクスチャバッファ

};

