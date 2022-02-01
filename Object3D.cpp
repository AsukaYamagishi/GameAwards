#include "Object3D.h"

#pragma region �ÓI�����o�ϐ��̎��̒�`
const float Object3D::radius = 5.0f; //��ʂ̔��a
const float Object3D::prizmHeight = 8.0f; //���̍���

//�f�o�C�X
ID3D12Device* Object3D::device = nullptr;
//�f�X�N���v�^�T�C�Y
UINT Object3D::descriptorHandleIncrementSize = 0;
//�R�}���h���X�g
ID3D12GraphicsCommandList* Object3D::cmdList = nullptr;
//���[�g�V�O�l�`��
ComPtr<ID3D12RootSignature> Object3D::rootsignature;
//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
ComPtr<ID3D12PipelineState> Object3D::pipelinestate;
//�f�X�N���v�^�q�[�v
ComPtr<ID3D12DescriptorHeap> Object3D::descHeap;
//���_�o�b�t�@
ComPtr<ID3D12Resource> Object3D::vertBuff;
//�C���f�b�N�X�o�b�t�@
ComPtr<ID3D12Resource> Object3D::indexBuff;
//�e�N�X�`���o�b�t�@
ComPtr<ID3D12Resource> Object3D::texBuff;
//�V�F�[�_�[���\�[�X�r���[�̃n���h��(CPU)
CD3DX12_CPU_DESCRIPTOR_HANDLE Object3D::cpuDescHandleSRV;
//�V�F�[�_�[���\�[�X�r���[�̃n���h��(GPU)
CD3DX12_GPU_DESCRIPTOR_HANDLE Object3D::gpuDescHandleSRV;
//�r���[�s��
XMMATRIX Object3D::matView{};
//�ˉe�s��
XMMATRIX Object3D::matProjection{};
//���_�s��
XMFLOAT3 Object3D::eye = { 0,0,-50.0f };
//�����_�s��
XMFLOAT3 Object3D::target = { 0,0,0 };
//������x�N�g��
XMFLOAT3 Object3D::up = { 0,1,0 };
//���_�o�b�t�@�r���[
D3D12_VERTEX_BUFFER_VIEW Object3D::vbView{};
//�C���f�b�N�X�o�b�t�@�r���[
D3D12_INDEX_BUFFER_VIEW Object3D::ibView{};
//���_�f�[�^�z��
Object3D::VertexPosNormalUv Object3D::vertices[vertexCount];
//���_�C���f�b�N�X�z��
unsigned short Object3D::indices[planeCount * 3];

#pragma endregion

//�ÓI������
bool Object3D::StaticInit(ID3D12Device* dev, int window_width, int window_height)
{
	//nullptr�`�F�b�N
	assert(dev);
	Object3D::device = dev;

	//�f�X�N���v�^�q�[�v������
	InitDescHeap();

	//�J����������
	InitCamera(window_width, window_height);

	//�p�C�v���C��������
	InitGraphicsPipeline();

	////�e�N�X�`���ǂݍ���
	//LoadTexture();

	//���f������
	CreateModel();

	return true;
}

//�`��O����
void Object3D::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	//PreDraw��PostDraw�������Ă΂�Ă��Ȃ��ƃG���[
	assert(Object3D::cmdList == nullptr);

	//�R�}���h���X�g���Z�b�g
	Object3D::cmdList = cmdList;

	//�p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(pipelinestate.Get());
	//���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	//�v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

//�`��㏈��
void Object3D::PostDraw()
{
	//�R�}���h���X�g������
	Object3D::cmdList = nullptr;
}

//�e�N�X�`���ǂݍ���
bool Object3D::LoadTexture(UINT texnumber, const wchar_t* filename)
{
	HRESULT result = S_FALSE;

	//�e�N�X�`���̃��[�h
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

	const Image* img = scratchImg.GetImage(0, 0, 0); //���f�[�^���o

	//���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			metadata.format,
			metadata.width,
			(UINT)metadata.height,
			(UINT16)metadata.arraySize,
			(UINT16)metadata.mipLevels);

	// �e�N�X�`���o�b�t�@����
	result = device->CreateCommittedResource(	//GPU���\�[�X�̐���
											 &CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
											 D3D12_HEAP_FLAG_NONE,
											 &texresDesc,
											 D3D12_RESOURCE_STATE_GENERIC_READ,	//�e�N�X�`���p�w��
											 nullptr,
											 IID_PPV_ARGS(&texBuff));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// �e�N�X�`���o�b�t�@�ւ̃f�[�^�]��
	result = texBuff->WriteToSubresource(
		0,
		nullptr,	//�S�̈�փR�s�[
		img->pixels,	//���f�[�^�A�h���X
		(UINT)img->rowPitch,	//1���C���T�C�Y
		(UINT)img->slicePitch	//�S�T�C�Y
	);
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	//�V�F�[�_���\�[�X�r���[�ݒ�
	cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);
	gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);
	
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	//�ݒ�\����
	D3D12_RESOURCE_DESC resDesc = texBuff->GetDesc();
	srvDesc.Format = metadata.format;	//RGBA
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2D�e�N�X�`��
	srvDesc.Texture1D.MipLevels = 1;

	//�V�F�[�_�[���\�[�X�r���[�쐬
	device->CreateShaderResourceView(
		texBuff.Get(),	//�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc,	//�e�N�X�`���ݒ���
		cpuDescHandleSRV);

	return true;
}

//3D�I�u�W�F�N�g����
Object3D* Object3D::CreateObject()
{
	//3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Object3D* object = new Object3D();
	if (object == nullptr) {
		return nullptr;
	}

	//������
	if (!object->Init()) {
		delete object;
		assert(0);
		return nullptr;
	}

	return object;
}

//���_���W�̎擾
void Object3D::SetEye(XMFLOAT3 eye)
{
	Object3D::eye = eye;
	UpdateViewMatrix();
}

//�����_���W�̎擾
void Object3D::SetTarget(XMFLOAT3 target)
{
	Object3D::target = target;
	UpdateViewMatrix();
}

//�x�N�g���ɂ��ړ�
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

//�f�X�N���v�^�q�[�v������
bool Object3D::InitDescHeap()
{
	HRESULT result = S_FALSE;

	//�f�X�N���v�^�q�[�v�̐���
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; //�V�F�[�_�[���猩����悤��
	descHeapDesc.NumDescriptors = 1; //�V�F�[�_�[���\�[�X�r���[1��
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap)); //����
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	//�f�X�N���v�^�T�C�Y���擾
	descriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);


	return true;
}

//�J����������
void Object3D::InitCamera(int window_width, int window_height)
{
	//�r���[�s��̐���
	matView = XMMatrixLookAtLH(
		XMLoadFloat3(&eye),
		XMLoadFloat3(&target),
		XMLoadFloat3(&up));

	//�������e�ɂ��ˉe�s��̐���
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)window_width / window_height,
		0.1f, 1000.0f);
}

//�O���t�B�b�N�p�C�v���C������
bool Object3D::InitGraphicsPipeline()
{
	HRESULT result;
	//���_�V�F�[�_�t�@�C���̓ǂݍ��݂ƃR���p�C��
	ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/Shaders/BasicVS.hlsl",  // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);
	//���_�V�F�[�_�̃G���[��\��
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
					errorBlob->GetBufferSize(),
					errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	//�s�N�Z���V�F�[�_�t�@�C���̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/Shaders/BasicPS.hlsl",   // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);
	//�s�N�Z���V�F�[�_�̃G���[��\��
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
					errorBlob->GetBufferSize(),
					errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}


	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
			{ //xyz���W
				"POSITION", //�Z�}���e�B�b�N��
				0, //�����Z�}���e�B�b�N�����������鎞�Ɏg���C���f�b�N�X(0�ł悢)
				DXGI_FORMAT_R32G32B32_FLOAT, //�v�f���ƃr�b�g��
				0, //���̓X���b�g�C���f�b�N�X(0�ł悢)
				D3D12_APPEND_ALIGNED_ELEMENT, //�f�[�^�̃I�t�Z�b�g�l
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, //���̓f�[�^���
				0}, //xyz
			{ //�@���x�N�g��
				"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
			}, //�@���x�N�g��
			{ //uv���W
				"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
			}, //uv

	};


	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get()); //���_�V�F�[�_
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get()); //�s�N�Z���V�F�[�_

	//�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�i�W���ݒ�(�w�ʃJ�����O/�h��Ԃ�/�[�x�N���b�s���O�L��)�j
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; //�W���ݒ�
	//���ʐݒ�
	blenddesc.BlendEnable = true; //�u�����h��L���ɂ���
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD; //���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE; //�\�[�X(�V�����F)�̒l��100%�g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO; //�f�X�g(������F)�̒l��0%�g��
	//�����������i�f�t�H���g�j
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD; //���Z
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA; //�\�[�X�̃A���t�@�l
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA; //1.0f - �\�[�X�̃A���t�@�l
#pragma region ���̑��̐ݒ�i���Z�����Ȃǁj
	////���Z�����i�����ɕʕ��j
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD; //���Z
	//blenddesc.SrcBlend = D3D12_BLEND_ONE; //�\�[�X�̒l100%
	//blenddesc.DestBlend = D3D12_BLEND_ONE; //�f�X�g�̒l100%

	////���Z����
	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT; //�f�X�g����\�[�X�����Z
	//blenddesc.SrcBlend = D3D12_BLEND_ONE; //�\�[�X�̒l100%
	//blenddesc.DestBlend = D3D12_BLEND_ONE; //�f�X�g�̒l100%

	////�F���]
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD; //���Z
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR; //1.0f - �f�X�g�J���[�̒l
	//blenddesc.DestBlend = D3D12_BLEND_ZERO; //�g��Ȃ�
#pragma endregion

	//�u�����h�X�e�[�g�̐ݒ�
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	//�[�x�o�b�t�@�̃t�H�[�}�b�g
	//�W���ݒ�i�[�x�e�X�g���s��/�������݋���/�[�x����������΍��i�j
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT; 

	//���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	//�}�`�̌`����O�p�`�ɐݒ�
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//���̑��̐ݒ�
	gpipeline.NumRenderTargets = 1; // �`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O


	//�f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); //t0���W�X�^
	//���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0); //�萔�o�b�t�@�r���[�Ƃ��ď�����(b0���W�X�^)
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV);

	//�e�N�X�`���T���v���[�i�X�^�e�B�b�N�T���v���[�j
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	//���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	//�o�[�W������������ł̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	//���[�g�V�O�l�`���̐���
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	if (FAILED(result)) {
		assert(0);
		return false;
	}
	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	gpipeline.pRootSignature = rootsignature.Get();

	//�O���t�B�b�N�X�p�C�v���C���̐���
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	return true;
}

//���f���쐬
void Object3D::CreateModel()
{
	HRESULT result = S_FALSE;

	std::vector<VertexPosNormalUv> realVertices;
	//���_���W�̌v�Z�i�d������j
	{
		realVertices.resize((division + 1) * 2);
		int index = 0;
		float zValue;
	
		//���
		zValue = prizmHeight / 2.0f;
		for (int i = 0; i < division; i++)
		{
			XMFLOAT3 vertex;
			vertex.x = radius * sinf(2 * XM_PI / division * i);
			vertex.y = radius * cosf(2 * XM_PI / division * i);
			vertex.z = zValue;
			realVertices[index++].pos = vertex;
		}
		realVertices[index++].pos = XMFLOAT3(0, 0, zValue); //��ʒ��S�_
	
		//�V��
		zValue = -prizmHeight / 2.0f;
		for (int i = 0; i < division; i++)
		{
			XMFLOAT3 vertex;
			vertex.x = radius * sinf(2 * XM_PI / division * i);
			vertex.y = radius * cosf(2 * XM_PI / division * i);
			vertex.z = zValue;
			realVertices[index++].pos = vertex;
		}
		realVertices[index++].pos = XMFLOAT3(0, 0, zValue); //�V�ʒ��S�_
	}

	//���_���W�̌v�Z�i�d���Ȃ��j
	{
		int index = 0;
		//���
		for (int i = 0; i < division; i++)
		{
			unsigned short idx0 = i + 1;
			unsigned short idx1 = i;
			unsigned short idx2 = division;

			vertices[index++] = realVertices[idx0];
			vertices[index++] = realVertices[idx1];
			vertices[index++] = realVertices[idx2]; //��ʒ��S�_
		}
		//��ʂ̍Ō�̎O�p�`��1�Ԗڂ̃C���f�b�N�X��0�ɏ�������
		vertices[index - 3] = realVertices[0];

		int topStart = division + 1;
		//�V��
		for (int i = 0; i < division; i++)
		{
			unsigned short idx0 = topStart + i;
			unsigned short idx1 = topStart + i + 1;
			unsigned short idx2 = topStart + division;

			vertices[index++] = realVertices[idx0];
			vertices[index++] = realVertices[idx1];
			vertices[index++] = realVertices[idx2]; //�V�ʒ��S�_
		}
		//�V�ʂ̍Ō�̎O�p�`��1�Ԗڂ̃C���f�b�N�X��0�ɏ�������
		vertices[index - 2] = realVertices[topStart];

		//����
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

	//���_�C���f�b�N�X
	{
		for (int i = 0; i < _countof(indices); i++)
		{
			indices[i] = i;
		}
	}

	//�@���x�N�g���̌v�Z
	for (int i = 0; i < _countof(indices) / 3; i++)
	{//�O�p�`1���ƂɌv�Z���Ă���
		//�O�p�`�̃C���f�b�N�X�����o��
		unsigned short idx0 = indices[i * 3 + 0];
		unsigned short idx1 = indices[i * 3 + 1];
		unsigned short idx2 = indices[i * 3 + 2];

		//�O�p�`���\�����钸�_���W���x�N�g���ɑ��
		XMVECTOR p0 = XMLoadFloat3(&vertices[idx0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[idx1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[idx2].pos);

		//p0->p1�x�N�g��,p0->p2�x�N�g�����v�Z�i�x�N�g���̌��Z�j
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);
		//�O�ς͗������琂���ȃx�N�g��
		XMVECTOR normal = XMVector3Cross(v1, v2);
		//���K��
		normal = XMVector3Normalize(normal);

		//���߂��@���𒸓_�f�[�^�ɑ��
		XMStoreFloat3(&vertices[idx0].normal, normal);
		XMStoreFloat3(&vertices[idx1].normal, normal);
		XMStoreFloat3(&vertices[idx2].normal, normal);
	}


	//���_�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	//�C���f�b�N�X�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(indices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}


	//���_�o�b�t�@�ւ̃f�[�^�]��
	// GPU��̃o�b�t�@�ɑΉ��������z���������擾
	VertexPosNormalUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		//�S���_�ɑ΂���
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr); // �}�b�v������
	}
	
	//�C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	//GPU��̃o�b�t�@�ɑΉ��������z���������擾
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {
		//�S�C���f�b�N�X�ɑ΂���
		for (int i = 0; i < _countof(indices); i++)
		{
			indexMap[i] = indices[i];   //�C���f�b�N�X���R�s�[
		}
		indexBuff->Unmap(0, nullptr); //�}�b�v������
	}

	//���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);

	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeof(indices);
}

//�r���[�s��X�V
void Object3D::UpdateViewMatrix()
{
	//�r���[�s��̍X�V
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

//������
bool Object3D::Init()
{
	//nullptr�`�F�b�N
	assert(device);

	HRESULT result;
	//�萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));

	return true;
}

//���t���[���X�V
void Object3D::Update()
{
	HRESULT result;
	XMMATRIX matScale, matRot, matTrans;

	//�X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z); //�X�P�[�����O
	matRot = XMMatrixIdentity(); //��]�s��
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z)); //Z������ɉ�]
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x)); //X������ɉ�]
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y)); //Y������ɉ�]
	matTrans = XMMatrixTranslation(position.x, position.y, position.z); //���s�ړ�

	//���[���h�s��̍���
	matWorld = XMMatrixIdentity(); //�P�ʍs��
	matWorld *= matScale; //���[���h���W�ɃX�P�[�����O�𔽉f
	matWorld *= matRot; //���[���h�s��ɉ�]�𔽉f
	matWorld *= matTrans; //���[���h�s��ɕ��s�ړ��𔽉f

	//�e�I�u�W�F�N�g�������
	if (parent != nullptr)
	{
		//�e�I�u�W�F�N�g�̃��[���h�s����|����
		matWorld *= parent->matWorld;
	}

	//�萔�o�b�t�@�փf�[�^�]��
	ConstBufferData* constMap = nullptr;
	if (SUCCEEDED(constBuff->Map(0, nullptr, (void**)&constMap)))
	{
		constMap->color = color;
		constMap->mat = matWorld * matView * matProjection;
		constBuff->Unmap(0, nullptr);
	}
}

//�`��
void Object3D::Draw()
{
	//nullptr�`�F�b�N
	assert(device);
	assert(Object3D::cmdList);

	//���_�o�b�t�@�̐ݒ�
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	cmdList->IASetIndexBuffer(&ibView);

	//�f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//�萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	//�V�F�[�_�[���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandleSRV);
	//�`��R�}���h
	cmdList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
}