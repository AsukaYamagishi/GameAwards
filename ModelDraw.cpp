#include "ModelDraw.h"
#include "ModelInput.h"

ModelDraw::ObjectCommon ModelDraw::objectCommon;
//std::vector<ModelDraw::VertexPosNormalUv> ModelDraw::vertices;
//std::vector<unsigned short> ModelDraw::indices;

bool ModelDraw::StaticInitialize(ID3D12Device *dev, int window_width, int window_height)
{

	objectCommon.dev = dev;

	//カメラ初期化
	/*objectCommon.eye = { 0.0f,30.0f,-200.0f };
	objectCommon.target = { 0.0f,0.0f,0.0f };
	objectCommon.up = { 0.0f,1.0f,0.0f };
	InitializeCamera(window_width, window_height);*/

	InitializeGraphicsPipeline();		//パイプライン初期化
	

	return true;
}

void ModelDraw::PreDraw(ID3D12GraphicsCommandList *cmdList)
{
	assert(objectCommon.cmdList == nullptr);

	objectCommon.cmdList = cmdList;

	//パイプラインステートの設定
	cmdList->SetPipelineState(objectCommon.pipelineSet.pipelinestate.Get());
	//ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(objectCommon.pipelineSet.rootsignature.Get());
	//トポロジー
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void ModelDraw::PostDraw()
{
	objectCommon.cmdList = nullptr;
}

bool ModelDraw::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob; // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/ObjVS.hlsl",  // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/ObjPS.hlsl",   // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);

	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		}
	};

	//グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
	//サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	//ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//標準的な設定（深度テストを行う、書き込みを許可、深度が小さければ合格）
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);	//深度テストを行う

	// レンダーターゲットのブレンド設定
	// ブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

#pragma region BLENDSET
	blenddesc.BlendEnable = true;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
#pragma endregion
	//#pragma region ADD
	//    blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	//    blenddesc.SrcBlend = D3D12_BLEND_ONE;
	//    blenddesc.DestBlend = D3D12_BLEND_ONE;
	//#pragma endregion
	//#pragma region SUB
	//    blenddesc.BlendOp = D3D12_BLEND_OP_SUBTRACT;
	//    blenddesc.SrcBlend = D3D12_BLEND_ONE;
	//    blenddesc.DestBlend = D3D12_BLEND_ONE;
	//#pragma endregion
	//#pragma region REVERSE
	//    blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	//    blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
	//    blenddesc.DestBlend = D3D12_BLEND_ZERO;
	//#pragma endregion
#pragma region INV
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
#pragma endregion

	//ブレンドステート設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;
	//深度値フォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	//頂点レイアウト設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	//トポロジー設定
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1; // 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング


	//デスクリプタテーブルの設定
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	//ルートパラメータの設定
	CD3DX12_ROOT_PARAMETER rootparams[3];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	// テクスチャサンプラーの設定
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	//バージョン自動判定でのシリアライズ
	result = D3DX12SerializeVersionedRootSignature(
		&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	//ルートシグネチャの生成
	result = objectCommon.dev->CreateRootSignature(
		0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&objectCommon.pipelineSet.rootsignature)
	);
	if (FAILED(result)) {
		return result;
	}
	// パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = objectCommon.pipelineSet.rootsignature.Get();

	//グラフィックスパイプラインステートの生成
	result = objectCommon.dev->CreateGraphicsPipelineState(
		&gpipeline, IID_PPV_ARGS(&objectCommon.pipelineSet.pipelinestate)
	);

	if (FAILED(result)) {
		return result;
	}

	return true;
}

void ModelDraw::BlendMode(BLENDMODE blendM)
{
}

ModelDraw *ModelDraw::Create(bool isBillboard)
{
	ModelDraw *model = new ModelDraw();
	model->isBillboard = isBillboard;
	if (model == nullptr) {
		return nullptr;
	}

	if (!model->Initialize()) {
		delete model;
		assert(0);
		return nullptr;
	}

	return model;
}


bool ModelDraw::Initialize()
{
	HRESULT result;
	//定数バッファの生成
	result = objectCommon.dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff), // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB0)
	);

	result = objectCommon.dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff), // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB1)
	);

	return true;
}

void ModelDraw::SetModel(ModelInput *model)
{
	objectModel = model;
}

void ModelDraw::Update()
{
	XMMATRIX matScale, matRot, matTrans;

	//スケール、回転、平行移動行列の計算
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	//ワールド行列の合成
	matWorld = XMMatrixIdentity();

	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;

	//親オブジェクトがあれば
	if (parent != nullptr) {
		//親オブジェクトのワールド行列をかける
		matWorld *= parent->matWorld;
	}

	//定数バッファへデータ転送
	ConstBufferDataB0 *constMap0 = nullptr;
	if (SUCCEEDED(constBuffB0->Map(0, nullptr, (void **)&constMap0))) {
		constMap0->mat = matWorld * Camera::GetCam()->GetMatView() * Camera::GetCam()->GetMatProj();
		
		constBuffB0->Unmap(0, nullptr);
	}

	ConstBufferDataB1 *constMap1 = nullptr;
	if (SUCCEEDED(constBuffB1->Map(0, nullptr, (void **)&constMap1))) {
		constMap1->ambient = objectModel->GetMaterial().ambient;
		constMap1->diffuse = objectModel->GetMaterial().diffuse;
		constMap1->specular = objectModel->GetMaterial().specular;
		constMap1->alpha = objectModel->GetMaterial().alpha;
		constBuffB1->Unmap(0, nullptr);
	}
}

void ModelDraw::Draw()
{
	objectModel->Draw(objectCommon.cmdList, constBuffB0, constBuffB1);
}

