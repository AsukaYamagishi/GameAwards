cbuffer cbuff0 : register(b0)
{
	float4 color; //色(RGBA)
	matrix mat; //3D変換行列
};

//頂点シェーダからピクセルシェーダへのやり取りに使用する構造体
struct VSOutput
{
	float4 svpos : SV_POSITION; //システム用頂点座標
	float3 normal : NORMAL; //法線ベクトル
	float2 uv : TEXCOORD; //uv値
};