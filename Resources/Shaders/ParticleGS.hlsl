#include "Particle.hlsli"

//�l�p�`�̒��_��
static const uint vnum = 4;

//�Z���^�[����̃I�t�Z�b�g
static const float4 offset_array[vnum] =
{
    float4(-0.5f, -0.5f, 0, 0), //����
    float4(-0.5f, +0.5f, 0, 0), //����
    float4(+0.5f, -0.5f, 0, 0), //�E��
    float4(+0.5f, +0.5f, 0, 0) //����
};

//���オ0,0    �E����1,1
static const float2 uv_array[vnum] =
{
    float2(0, 1), //����
    float2(0, 0), //����
    float2(1, 1), //�E��
    float2(1, 0), //�E��
};

//�_�̓��͂���A�_���o��
[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : SV_POSITION,
	inout TriangleStream<GSOutput> output
)
{
    GSOutput element;
    //���_���
    //4�_����
    for (int i = 0; i < vnum; i++)
    {
        //���S����̃I�t�Z�b�g���X�P�[�����O
        float4 offset = offset_array[i] * input[0].scale;
        //���S����̃I�t�Z�b�g���r���{�[�h��]�i���f�����W�j
        offset = mul(matBillboard, offset);
        //�I�t�Z�b�g�����炷�i���[���h���W�j
        element.svpos = input[0].pos + offset;
        //�r���[�A�ˉe�ϊ�
        element.svpos = mul(mat, element.svpos);
        element.uv = uv_array[i];
        output.Append(element);
    }
}