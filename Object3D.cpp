#include "Object3D.h"

#pragma region 静的メンバ変数の実体定義
const float Object3D::radius = 5.0f; //底面の半径
const float Object3D::prizmHeight = 8.0f; //柱の高さ

//デバイス
ID3D12Device* Object3D::device = nullptr;
//デスクリプタサイズ
UINT Object3D::descriptorHandleIncrementSize = 0;
//コマンドリスト
ID3D12GraphicsCommandList* Object3D::cmdList = nullptr;
//ルートシグネチャ
ComPtr<ID3D12RootSignature> Object3D::rootsignature;
//パイプラインステートオブジェクト
ComPtr<ID3D12PipelineState> Object3D::pipelinestate;
//デスクリプタヒープ
ComPtr<ID3D12DescriptorHeap> Object3D::descHeap;
//頂点バッファ
ComPtr<ID3D12Resource> Object3D::vertBuff;
//インデックスバッファ
ComPtr<ID3D12Resource> Object3D::indexBuff;
//テクスチャバッファ
ComPtr<ID3D12Resource> Object3D::texBuff;
//シェーダーリソースビューのハンドル(CPU)
CD3DX12_CPU_DESCRIPTOR_HANDLE Object3D::cpuDescHandleSRV;
//シェーダーリソースビューのハンドル(GPU)
CD3DX12_GPU_DESCRIPTOR_HANDLE Object3D::gpuDescHandleSRV;
//ビュー行列
XMMATRIX Object3D::matView{};
//射影行列
XMMATRIX Object3D::matProjection{};
//視点行列
XMFLOAT3 Object3D::eye = { 0,0,-50.0f };
//注視点行列
XMFLOAT3 Object3D::target = { 0,0,0 };
//上方向ベクトル
XMFLOAT3 Object3D::up = { 0,1,0 };
//頂点バッファビュー
D3D12_VERTEX_BUFFER_VIEW Object3D::vbView{};
//インデックスバッファビュー
D3D12_INDEX_BUFFER_VIEW Object3D::ibView{};
//頂点データ配列
Object3D::VertexPosNormalUv Object3D::vertices[vertexCount];
//頂点インデックス配列
unsigned short Object3D::indices[planeCount * 3];

#pragma endregion

//静的初期化
bool Object3D::StaticInit(ID3D12Device* dev, int window_width, int window_height)
{
	//nullptrチェック
	assert(dev);
	Object3D::device = dev;

	//デスクリプタヒープ初期化
	InitDescHeap();

	//カメラ初期化
	InitCamera(window_width, window_height);

	//パイプライン初期化
	InitGraphicsPipeline();

	////テクスチャ読み込み
	//LoadTexture();

	//モデル生成
	CreateModel();

	return true;
}

//描画前処理
void Object3D::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	//PreDrawとPostDrawが両方呼ばれていないとエラー
	assert(Object3D::cmdList == nullptr);

	//コマンドリストをセット
	Object3D::cmdList = cmdList;

	//パイプラインステートの設定
	cmdList->SetPipelineState(pipelinestate.Get());
	//ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	//プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

//描画後処理
void Object3D::PostDraw()
{
	//コマンドリストを解除
	Object3D::cmdList = nullptr;
}

//テクスチャ読み込み
bool Object3D::LoadTexture(UINT texnumber, const wchar_t* filename)
{
	HRESULT result = S_FALSE;

	//テクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	result = LoadFromWICFile(
		filename,
		WIC_FLAGS_NONE,
		&metadata, scratchImg);
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	const Image* img = scratchImg.GetImage(0, 0, 0); //生データ抽出

	//リソース設定
	CD3DX12_RESOURCE_DESC texresDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			metadata.format,
			metadata.width,
			(UINT)metadata.height,
			(UINT16)metadata.arraySize,
			(UINT16)metadata.mipLevels);

	// テクスチャバッファ生成
	result = device->CreateCommittedResource(	//GPUリソースの生成
											 &CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
											 D3D12_HEAP_FLAG_NONE,
											 &texresDesc,
											 D3D12_RESOURCE_STATE_GENERIC_READ,	//テクスチャ用指定
											 nullptr,
											 IID_PPV_ARGS(&texBuff));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// テクスチャバッファへのデータ転送
	result = texBuff->WriteToSubresource(
		0,
		nullptr,	//全領域へコピー
		img->pixels,	//元データアドレス
		(UINT)img->rowPitch,	//1ラインサイズ
		(UINT)img->slicePitch	//全サイズ
	);
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	//シェーダリソースビュー設定
	cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);
	gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);
	
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	//設定構造体
	D3D12_RESOURCE_DESC resDesc = texBuff->GetDesc();
	srvDesc.Format = metadata.format;	//RGBA
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2Dテクスチャ
	srvDesc.Texture1D.MipLevels = 1;

	//シェーダーリソースビュー作成
	device->CreateShaderResourceView(
		texBuff.Get(),	//ビューと関連付けるバッファ
		&srvDesc,	//テクスチャ設定情報
		cpuDescHandleSRV);

	return true;
}

//3Dオブジェクト生成
Object3D* Object3D::CreateObject()
{
	//3Dオブジェクトのインスタンスを生成
	Object3D* object = new Object3D();
	if (object == nullptr) {
		return nullptr;
	}

	//初期化
	if (!object->Init()) {
		delete object;
		assert(0);
		return nullptr;
	}

	return object;
}

//視点座標の取得
void Object3D::SetEye(XMFLOAT3 eye)
{
	Object3D::eye = eye;
	UpdateViewMatrix();
}

//注視点座標の取得
void Object3D::SetTarget(XMFLOAT3 target)
{
	Object3D::target = target;
	UpdateViewMatrix();
}

//ベクトルによる移動
void Object3D::CameraMoveVector(XMFLOAT3 move)
{
	XMFLOAT3 eye_moved = GetEye();
	XMFLOAT3 target_moved = GetTarget();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	target_moved.x += move.x;
	target_moved.y += move.y;
	target_moved.z += move.z;

	SetEye(eye_moved);
	SetTarget(target_moved);
}

//デスクリプタヒープ初期化
bool Object3D::InitDescHeap()
{
	HRESULT result = S_FALSE;

	//デスクリプタヒープの生成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; //シェーダーから見えるように
	descHeapDesc.NumDescriptors = 1; //シェーダーリソースビュー1つ
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap)); //生成
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	//デスクリプタサイズを取得
	descriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);


	return true;
}

//カメラ初期化
void Object3D::InitCamera(int window_width, int window_height)
{
	//ビュー行列の生成
	matView = XMMatrixLookAtLH(
		XMLoadFloat3(&eye),
		XMLoadFloat3(&target),
		XMLoadFloat3(&up));

	//透視投影による射影行列の生成
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)window_width / window_height,
		0.1f, 1000.0f);
}

//グラフィックパイプライン生成
bool Object3D::InitGraphicsPipeline()
{
	HRESULT result;
	//頂点シェーダファイルの読み込みとコンパイル
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob; // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/BasicVS.hlsl",  // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	//頂点シェーダのエラーを表示
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
					errorBlob->GetBufferSize(),
					errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	//ピクセルシェーダファイルの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/BasicPS.hlsl",   // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	//ピクセルシェーダのエラーを表示
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
					errorBlob->GetBufferSize(),
					errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}


	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
			{ //xyz座標
				"POSITION", //セマンティック名
				0, //同じセマンティック名が複数ある時に使うインデックス(0でよい)
				DXGI_FORMAT_R32G32B32_FLOAT, //要素数とビット数
				0, //入力スロットインデックス(0でよい)
				D3D12_APPEND_ALIGNED_ELEMENT, //データのオフセット値
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, //入力データ種別
				0}, //xyz
			{ //法線ベクトル
				"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
			}, //法線ベクトル
			{ //uv座標
				"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
			}, //uv

	};


	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get()); //頂点シェーダ
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get()); //ピクセルシェーダ

	//サンプルマスクとラスタライザステート（標準設定(背面カリング/塗りつぶし/深度クリッピング有効)）
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	//デプスステンシルステートの設定
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	//レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; //標準設定
	//共通設定
	blenddesc.BlendEnable = true; //ブレンドを有効にする
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD; //加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE; //ソース(新しい色)の値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO; //デスト(元ある色)の値を0%使う
	//半透明合成（デフォルト）
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD; //加算
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA; //ソースのアルファ値
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA; //1.0f - ソースのアルファ値
#pragma region その他の設定（加算合成など）
	////加算合成（微妙に別物）
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD; //加算
	//blenddesc.SrcBlend = D3D12_BLEND_ONE; //ソースの値100%
	//blenddesc.DestBlend = D3D12_BLEND_ONE; //デストの値100%

	////減算合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT; //デストからソースを減算
	//blenddesc.SrcBlend = D3D12_BLEND_ONE; //ソースの値100%
	//blenddesc.DestBlend = D3D12_BLEND_ONE; //デストの値100%

	////色反転
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD; //加算
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR; //1.0f - デストカラーの値
	//blenddesc.DestBlend = D3D12_BLEND_ZERO; //使わない
#pragma endregion

	//ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	//深度バッファのフォーマット
	//標準設定（深度テストを行う/書き込み許可/深度が小さければ合格）
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT; 

	//頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	//図形の形状を三角形に設定
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//その他の設定
	gpipeline.NumRenderTargets = 1; // 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング


	//デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); //t0レジスタ
	//ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0); //定数バッファビューとして初期化(b0レジスタ)
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV);

	//テクスチャサンプラー（スタティックサンプラー）
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	//ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	//バージョン自動判定でのシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	//ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	if (FAILED(result)) {
		assert(0);
		return false;
	}
	// パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = rootsignature.Get();

	//グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	return true;
}

//モデル作成
void Object3D::CreateModel()
{
	HRESULT result = S_FALSE;

	std::vector<VertexPosNormalUv> realVertices;
	//頂点座標の計算（重複あり）
	{
		realVertices.resize((division + 1) * 2);
		int index = 0;
		float zValue;
	
		//底面
		zValue = prizmHeight / 2.0f;
		for (int i = 0; i < division; i++)
		{
			XMFLOAT3 vertex;
			vertex.x = radius * sinf(2 * XM_PI / division * i);
			vertex.y = radius * cosf(2 * XM_PI / division * i);
			vertex.z = zValue;
			realVertices[index++].pos = vertex;
		}
		realVertices[index++].pos = XMFLOAT3(0, 0, zValue); //底面中心点
	
		//天面
		zValue = -prizmHeight / 2.0f;
		for (int i = 0; i < division; i++)
		{
			XMFLOAT3 vertex;
			vertex.x = radius * sinf(2 * XM_PI / division * i);
			vertex.y = radius * cosf(2 * XM_PI / division * i);
			vertex.z = zValue;
			realVertices[index++].pos = vertex;
		}
		realVertices[index++].pos = XMFLOAT3(0, 0, zValue); //天面中心点
	}

	//頂点座標の計算（重複なし）
	{
		int index = 0;
		//底面
		for (int i = 0; i < division; i++)
		{
			unsigned short idx0 = i + 1;
			unsigned short idx1 = i;
			unsigned short idx2 = division;

			vertices[index++] = realVertices[idx0];
			vertices[index++] = realVertices[idx1];
			vertices[index++] = realVertices[idx2]; //底面中心点
		}
		//底面の最後の三角形の1番目のインデックスを0に書き換え
		vertices[index - 3] = realVertices[0];

		int topStart = division + 1;
		//天面
		for (int i = 0; i < division; i++)
		{
			unsigned short idx0 = topStart + i;
			unsigned short idx1 = topStart + i + 1;
			unsigned short idx2 = topStart + division;

			vertices[index++] = realVertices[idx0];
			vertices[index++] = realVertices[idx1];
			vertices[index++] = realVertices[idx2]; //天面中心点
		}
		//天面の最後の三角形の1番目のインデックスを0に書き換え
		vertices[index - 2] = realVertices[topStart];

		//側面
		for (int i = 0; i < division; i++)
		{
			unsigned short idx0 = i + 1;
			unsigned short idx1 = topStart + i + 1;
			unsigned short idx2 = i;
			unsigned short idx3 = topStart + i;

			if (i == division - 1)
			{
				idx0 = 0;
				idx1 = topStart;
			}

			vertices[index++] = realVertices[idx0];
			vertices[index++] = realVertices[idx1];
			vertices[index++] = realVertices[idx2];

			vertices[index++] = realVertices[idx2];
			vertices[index++] = realVertices[idx1];
			vertices[index++] = realVertices[idx3];
		}
	}

	//頂点インデックス
	{
		for (int i = 0; i < _countof(indices); i++)
		{
			indices[i] = i;
		}
	}

	//法線ベクトルの計算
	for (int i = 0; i < _countof(indices) / 3; i++)
	{//三角形1つごとに計算していく
		//三角形のインデックスを取り出す
		unsigned short idx0 = indices[i * 3 + 0];
		unsigned short idx1 = indices[i * 3 + 1];
		unsigned short idx2 = indices[i * 3 + 2];

		//三角形を構成する頂点座標をベクトルに代入
		XMVECTOR p0 = XMLoadFloat3(&vertices[idx0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[idx1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[idx2].pos);

		//p0->p1ベクトル,p0->p2ベクトルを計算（ベクトルの減算）
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);
		//外積は両方から垂直なベクトル
		XMVECTOR normal = XMVector3Cross(v1, v2);
		//正規化
		normal = XMVector3Normalize(normal);

		//求めた法線を頂点データに代入
		XMStoreFloat3(&vertices[idx0].normal, normal);
		XMStoreFloat3(&vertices[idx1].normal, normal);
		XMStoreFloat3(&vertices[idx2].normal, normal);
	}


	//頂点バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	//インデックスバッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(indices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}


	//頂点バッファへのデータ転送
	// GPU上のバッファに対応した仮想メモリを取得
	VertexPosNormalUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		//全頂点に対して
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr); // マップを解除
	}
	
	//インデックスバッファへのデータ転送
	//GPU上のバッファに対応した仮想メモリを取得
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {
		//全インデックスに対して
		for (int i = 0; i < _countof(indices); i++)
		{
			indexMap[i] = indices[i];   //インデックスをコピー
		}
		indexBuff->Unmap(0, nullptr);	//マップを解除
	}

	//頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);

	//インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeof(indices);
}

//ビュー行列更新
void Object3D::UpdateViewMatrix()
{
	//ビュー行列の更新
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

//初期化
bool Object3D::Init()
{
	//nullptrチェック
	assert(device);

	HRESULT result;
	//定数バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));

	return true;
}

//毎フレーム更新
void Object3D::Update()
{
	HRESULT result;
	XMMATRIX matScale, matRot, matTrans;

	//スケール、回転、平行移動行列の計算
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z); //スケーリング
	matRot = XMMatrixIdentity(); //回転行列
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z)); //Z軸周りに回転
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x)); //X軸周りに回転
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y)); //Y軸周りに回転
	matTrans = XMMatrixTranslation(position.x, position.y, position.z); //平行移動

	//ワールド行列の合成
	matWorld = XMMatrixIdentity(); //単位行列
	matWorld *= matScale; //ワールド座標にスケーリングを反映
	matWorld *= matRot; //ワールド行列に回転を反映
	matWorld *= matTrans; //ワールド行列に平行移動を反映

	//親オブジェクトがあれば
	if (parent != nullptr)
	{
		//親オブジェクトのワールド行列を掛ける
		matWorld *= parent->matWorld;
	}

	//定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;
	if (SUCCEEDED(constBuff->Map(0, nullptr, (void**)&constMap)))
	{
		constMap->color = color;
		constMap->mat = matWorld * matView * matProjection;
		constBuff->Unmap(0, nullptr);
	}
}

//描画
void Object3D::Draw()
{
	//nullptrチェック
	assert(device);
	assert(Object3D::cmdList);

	//頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	//インデックスバッファの設定
	cmdList->IASetIndexBuffer(&ibView);

	//デスクリプタヒープの配列
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	//シェーダーリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandleSRV);
	//描画コマンド
	cmdList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
}