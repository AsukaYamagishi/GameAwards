#include "Sprite.hlsli"

VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
	VSOutput output; //�s�N�Z���V�F�[�_�ɓn���l
	output.svpos = mul(mat, pos); //���W�ɍs�����Z
	output.uv = uv;
	return output;
}