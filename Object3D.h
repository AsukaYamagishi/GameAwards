#pragma once
#include<DirectXTex.h>
#include<wrl.h>
#include<d3d12.h>
#include<d3dx12.h>
#include<DirectXMath.h>
#include <d3dcompiler.h>


using namespace Microsoft::WRL;
using namespace DirectX;
#pragma comment(lib, "d3dcompiler.lib")


class Object3D
{
#pragma region 構造体
public: //構造体
	//頂点データ構造体
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; //xyz
		XMFLOAT3 normal; //法線ベクトル
		XMFLOAT2 uv; //uv
	};
	//定数バッファデータ構造体（送りたいデータが増えたらここに書く）
	struct ConstBufferData
	{
		XMFLOAT4 color; //色(RGBA)
		XMMATRIX mat; //3D変換行列
	};
#pragma endregion

#pragma region 変数
private: //定数
	static const int division = 50; //分割数
	static const float radius; //底面の半径
	static const float prizmHeight; //柱の高さ
	static const int planeCount = division * 2 + division * 2; //面の数
	static const int vertexCount = planeCount * 3; //頂点数
	//static const int spriteSRVCount = 512; //テクスチャの最大枚数

private: //静的メンバ変数
	//デバイス
	static ID3D12Device* device;
	//デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;
	//コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	//ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	//パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;
	//デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	//頂点バッファ
	static ComPtr<ID3D12Resource> vertBuff;
	//インデックスバッファ
	static ComPtr<ID3D12Resource> indexBuff;
	//テクスチャバッファ
	static ComPtr<ID3D12Resource> texBuff;
	//シェーダーリソースビューのハンドル(CPU)
	static CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	//シェーダーリソースビューのハンドル(GPU)
	static CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	//ビュー行列
	static XMMATRIX matView;
	//射影行列
	static XMMATRIX matProjection;
	//視点行列
	static XMFLOAT3 eye;
	//注視点行列
	static XMFLOAT3 target;
	//上方向ベクトル
	static XMFLOAT3 up;
	//頂点バッファビュー
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	//インデックスバッファビュー
	static D3D12_INDEX_BUFFER_VIEW ibView;
	//頂点データ配列
	static VertexPosNormalUv vertices[vertexCount];
	//頂点インデックス配列
	static unsigned short indices[planeCount * 3];

private: //メンバ変数
	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	//色
	XMFLOAT4 color = { 1,1,1,1 };
	//ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	//XYZ軸のローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	//ローカル座標
	XMFLOAT3 position = { 0,0,0 };
	//ローカルワールド変換行列
	XMMATRIX matWorld;
	//親オブジェクト
	Object3D* parent = nullptr;
#pragma endregion

#pragma region 関数
public: //静的メンバ関数
	//静的初期化
	static bool StaticInit(ID3D12Device* dev, int window_width, int window_height);
	//描画前処理
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);
	//描画後処理
	static void PostDraw();
	//3Dオブジェクト生成
	static Object3D* CreateObject();
	//テクスチャ読み込み
	static bool LoadTexture(UINT texnumber, const wchar_t* filename);

	//視点座標の取得
	static const XMFLOAT3& GetEye() { return eye; }
	//視点座標の設定
	static void SetEye(XMFLOAT3 eye);
	//注視点座標の取得
	static const XMFLOAT3& GetTarget() { return target; }
	//注視点座標の設定
	static void SetTarget(XMFLOAT3 target);
	//ベクトルによる移動
	static void CameraMoveVector(XMFLOAT3 move);

private: //静的メンバ関数
	//デスクリプタヒープ初期化
	static bool InitDescHeap();
	//カメラ初期化
	static void InitCamera(int window_width, int window_height);
	//グラフィックパイプライン生成
	static bool InitGraphicsPipeline();
	//モデル作成
	static void CreateModel();
	//ビュー行列を更新
	static void UpdateViewMatrix();

public: //メンバ関数
	//初期化
	bool Init();
	//毎フレーム処理
	void Update();
	//描画
	void Draw();
	//座標の取得
	const XMFLOAT3& GetPosition() { return position; }
	//座標の設定
	void SetPosition(XMFLOAT3 position) { this->position = position; }
#pragma endregion

};

