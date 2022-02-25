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


class Sprite
{
#pragma region 構造体
public: //構造体
	//頂点データ構造体
	struct VertexPosUv
	{
		XMFLOAT3 pos; //xyz
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
private: //静的メンバ変数
	//テクスチャの最大枚数
	static const int spriteSRVCount = 512; //テクスチャの最大枚数
	//頂点数
	static const int vertNum = 4;
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
	//テクスチャバッファ
	static ComPtr<ID3D12Resource> texBuff[spriteSRVCount];
	//射影行列
	static XMMATRIX matProjection;

private: //メンバ変数
	//頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//テクスチャ番号
	UINT texNumber = 0;
	//Z軸の回転角
	float rotation = 0.0f;
	//ローカル座標
	XMFLOAT2 position = {};
	//スプライト幅、高さ
	XMFLOAT2 size = { 100.0f,100.0f };
	//アンカーポイント
	XMFLOAT2 anchorpoint = { 0,0 };
	//ワールド変換行列
	XMMATRIX matWorld{};
	//色
	XMFLOAT4 color = { 1,1,1,1 };
	//左右反転
	bool isFlipX = false;
	//上下反転
	bool isFlipY = false;
	//テクスチャ始点
	XMFLOAT2 texBase = { 0,0 };
	//テクスチャ幅、高さ
	XMFLOAT2 texSize = { 100.0f,100.0f };
	
#pragma endregion

#pragma region 関数
public: //静的メンバ関数
	//静的初期化
	static bool StaticInit(ID3D12Device* dev, int window_width, int window_height);
	//描画前処理
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);
	//描画後処理
	static void PostDraw();
	//スプライト生成
	static Sprite* CreateSprite(UINT texNumber, XMFLOAT2 position, XMFLOAT4 color = { 1,1,1,1 }, XMFLOAT2 anchorpoint = { 0.0f,0.0f }, bool isFlipX = false, bool isFlipY = false);
	//テクスチャ読み込み
	static bool LoadTexture(UINT texnumber, const wchar_t* filename);

public: //メンバ関数
	//コンストラクタ
	Sprite(UINT texNumber, XMFLOAT2 position, XMFLOAT2 size, XMFLOAT4 color, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlopY);
	//初期化
	bool Init();
	//描画
	void Draw();
	//角度の設定
	void SetRotation(float rotation);
	//座標の設定
	void SetPosition(XMFLOAT2 position);
	//サイズの設定
	void SetSize(XMFLOAT2 size);
	//アンカーポイントの設定
	void SetAnchorPoint(XMFLOAT2 anchorpoint);
	//左右反転の設定
	void SetIsFlipX(bool isFlipX);
	//上下反転の設定
	void SetIsFlipY(bool isFlipY);
	//テクスチャ範囲設定
	void SetTextureRect(XMFLOAT2 texBase, XMFLOAT2 texSize);

private: //メンバ関数
	//頂点データ転送
	void TransferVertices();
#pragma endregion

};

