#include "ModelDraw.h"
#include "ModelInput.h"

ModelDraw::ObjectCommon ModelDraw::objectCommon;
//std::vector<ModelDraw::VertexPosNormalUv> ModelDraw::vertices;
//std::vector<unsigned short> ModelDraw::indices;

bool ModelDraw::StaticInitialize(ID3D12Device *dev, int window_width, int window_height)
{

	objectCommon.dev = dev;

	//�J����������
	objectCommon.eye = { 0.0f,30.0f,-200.0f };
	objectCommon.target = { 0.0f,0.0f,0.0f };
	objectCommon.up = { 0.0f,1.0f,0.0f };
	InitializeCamera(window_width, window_height);

	InitializeGraphicsPipeline();		//�p�C�v���C��������
	

	return true;
}

void ModelDraw::PreDraw(ID3D12GraphicsCommandList *cmdList)
{
	assert(objectCommon.cmdList == nullptr);

	objectCommon.cmdList = cmdList;

	//�p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(objectCommon.pipelineSet.pipelinestate.Get());
	//���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(objectCommon.pipelineSet.rootsignature.Get());
	//�g�|���W�[
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void ModelDraw::PostDraw()
{
	objectCommon.cmdList = nullptr;
}

void ModelDraw::InitializeCamera(int window_width, int window_height)
{
	//�r���[�s��̐���
	UpdateViewMatrix();

	//�������e�ϊ�
	objectCommon.matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)window_width / window_height,
		0.1f, 1000.0f
	);
}


bool ModelDraw::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/Shaders/ObjVS.hlsl",  // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/Shaders/ObjPS.hlsl",   // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);

	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ���_���C�A�E�g
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

	//�O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
	//�T���v���}�X�N
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	//���X�^���C�U�X�e�[�g
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//�W���I�Ȑݒ�i�[�x�e�X�g���s���A�������݂����A�[�x����������΍��i�j
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);	//�[�x�e�X�g���s��

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	// �u�����h�ݒ�
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

	//�u�����h�X�e�[�g�ݒ�
	gpipeline.BlendState.RenderTarget[0] = blenddesc;
	//�[�x�l�t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	//���_���C�A�E�g�ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	//�g�|���W�[�ݒ�
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1; // �`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O


	//�f�X�N���v�^�e�[�u���̐ݒ�
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	//���[�g�p�����[�^�̐ݒ�
	CD3DX12_ROOT_PARAMETER rootparams[3];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	// �e�N�X�`���T���v���[�̐ݒ�
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	//�o�[�W������������ł̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(
		&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	//���[�g�V�O�l�`���̐���
	result = objectCommon.dev->CreateRootSignature(
		0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&objectCommon.pipelineSet.rootsignature)
	);
	if (FAILED(result)) {
		return result;
	}
	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	gpipeline.pRootSignature = objectCommon.pipelineSet.rootsignature.Get();

	//�O���t�B�b�N�X�p�C�v���C���X�e�[�g�̐���
	result = objectCommon.dev->CreateGraphicsPipelineState(
		&gpipeline, IID_PPV_ARGS(&objectCommon.pipelineSet.pipelinestate)
	);

	if (FAILED(result)) {
		return result;
	}

	return true;
}


void ModelDraw::UpdateViewMatrix()
{
	objectCommon.matView = XMMatrixLookAtLH(
		XMLoadFloat3(&objectCommon.eye),
		XMLoadFloat3(&objectCommon.target),
		XMLoadFloat3(&objectCommon.up));
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
	//�萔�o�b�t�@�̐���
	result = objectCommon.dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff), // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB0)
	);

	result = objectCommon.dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff), // ���\�[�X�ݒ�
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

	//�X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	//���[���h�s��̍���
	matWorld = XMMatrixIdentity();

	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;

	//�e�I�u�W�F�N�g�������
	if (parent != nullptr) {
		//�e�I�u�W�F�N�g�̃��[���h�s���������
		matWorld *= parent->matWorld;
	}

	//�萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataB0 *constMap0 = nullptr;
	if (SUCCEEDED(constBuffB0->Map(0, nullptr, (void **)&constMap0))) {
		constMap0->mat = matWorld * objectCommon.matView * objectCommon.matProjection;
		
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


