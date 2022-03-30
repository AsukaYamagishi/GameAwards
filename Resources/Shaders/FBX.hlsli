cbuffer cbuff0 : register(b0)
{
	matrix viewproj; //�r���[�v���W�F�N�V�����s��
	matrix world; //���[���h�s��
	float3 cameraPos; //�J�������W
};

//�{�[���̍ő吔
static const int MAX_BONES = 32;
cbuffer skinning:register(b3) //�{�[���̃X�L�j���O�s�񂪓���
{
	matrix matSkinning[MAX_BONES];
};

//VertexBuff�̓���
struct VSInput
{
	float4 pos : POSITION; //�ʒu
	float3 normal : NORMAL; //���_�@��
	float2 uv : TEXCOORD; //�e�N�X�`�����W
	uint4 boneIndices : BONEINDICES; //�{�[���̔ԍ�
	float4 boneWeights : BONEWEIGHTS; //�{�[���̃X�L���E�F�C�g
};

//VS����PS�ւ̂����p�\����
struct VSOutput
{
	float4 svpos : SV_POSITION; //�V�X�e���p���_���W
	float3 normal :NORMAL; //�@��
	float2 uv :TEXCOORD; //uv�l
};

//�X�L�j���O��̒��_�E�@��������
struct SkinOutput
{
	float4 pos;
	float3 normal;
};