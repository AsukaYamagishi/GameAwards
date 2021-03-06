#include "FbxMeshCollider.h"
#include "Collision.h"

using namespace DirectX;

void FbxMeshCollider::ConstrucTriangles(FbxModel* model)
{
	//三角形リストをクリア
	triangles.clear();
	//モデルの持つメッシュリストを取得
	//const std::vector<Mesh*>& meshes = model->GetMeshes();
	//現在のメッシュの開始三角形番号を入れておく変数（0で初期化）
	int start = 0;
	//全メッシュについて順に処理する
	//std::vector<Mesh*>::const_iterator it = meshes.cbegin();
	//for (; it != meshes.cend(); ++it)
	{
		//Mesh* mesh = *it;
		const std::vector<FbxModel::VertexPosNormalUvSkin>& vertices = model->GetColVertices();
		const std::vector<unsigned short>& indices = model->GetColIndces();
		//インデックスは、三角形の数×3個であるので、
		//そこからメッシュ内の三角形の数を逆算する
		size_t triangleNum = indices.size() / 3;
		//現在のメッシュの三角形の数だけ、三角形リストにスペースを追加する
		triangles.resize(triangles.size() + triangleNum);
		//全三角形について順に処理する
		for (int i = 0; i < triangleNum; i++)
		{
			//今から計算する三角形の参照
			Triangle& tri = triangles[start + i];
			int idx0 = indices[i * 3 + 0];
			int idx1 = indices[i * 3 + 1];
			int idx2 = indices[i * 3 + 2];
			//三角形の3頂点の座標を代入
			tri.p0 = { vertices[idx0].pos.x,vertices[idx0].pos.y,vertices[idx0].pos.z,1 };
			tri.p1 = { vertices[idx1].pos.x,vertices[idx1].pos.y,vertices[idx1].pos.z,1 };
			tri.p2 = { vertices[idx2].pos.x,vertices[idx2].pos.y,vertices[idx2].pos.z,1 };
			//3頂点から法線を計算
			tri.ComputeNormal();
		}
		//次のメッシュは、今までの三角形番号の次から使う
		start += (int)triangleNum;
	}
}

void FbxMeshCollider::Update()
{
	//ワールド行列の逆行列を計算
	invMatWorld = XMMatrixInverse(nullptr, GetObject3d()->GetMatWorld());
}

bool FbxMeshCollider::CheckCollisionSphere(const Sphere& sphere, DirectX::XMVECTOR* inter)
{
	//オブジェクtのローカル座標系での球を得る
	Sphere localSphere;
	localSphere.center = XMVector3Transform(sphere.center, invMatWorld);
	localSphere.radius = sphere.radius;
	localSphere.radius *= XMVector3Length(invMatWorld.r[0]).m128_f32[0];

	//ローカル座標系での交差をチェック
	std::vector<Triangle>::const_iterator it = triangles.cbegin();

	for (; it != triangles.cend(); ++it)
	{
		const Triangle& triangle = *it;
		//球と三角形の当たり判定
		if (Collision::IsSqhereToTriangle(localSphere, triangle, inter)) {
			if (inter) {
				const XMMATRIX& matWorld = GetObject3d()->GetMatWorld();
				//ワールド座標系での交差を得る
				*inter = XMVector3Transform(*inter, matWorld);
			}
			return true;
		}
	}

	return false;
}

bool FbxMeshCollider::CheckCollisionRay(const Ray& ray, float* distance, DirectX::XMVECTOR* inter)
{
	return false;
}
