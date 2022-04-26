﻿#include "FbxInput.h"
#include <cassert>

using namespace DirectX;

//静的メンバ変数の実体
const std::string FbxInput::baseDirectory = "Resources/";
const std::string FbxInput::defaultTexFileName = "white1x1.png";


FbxInput* FbxInput::GetInstance()
{
    static FbxInput instance;
    return &instance;
}

void FbxInput::Init(ID3D12Device* dev)
{
    //再初期化チェック
    assert(fbxManager == nullptr);
    //引数からメンバ変数に代入
    this->dev = dev;
    //FBXマネージャの生成
    fbxManager = FbxManager::Create();
    //FBXマネージャの入出力設定
    FbxIOSettings* ios = FbxIOSettings::Create(fbxManager, IOSROOT);
    fbxManager->SetIOSettings(ios);
    //FBXインポータの生成
    fbxImporter = FbxImporter::Create(fbxManager, "");
}

void FbxInput::Fin()
{
    //各種FBXインスタンスの破棄
    fbxImporter->Destroy();
    fbxManager->Destroy();
}

std::unique_ptr<FbxModel> FbxInput::LoadFbxFromFile(const string& modelName)
{
    //モデルと同じ名前のフォルダから読み込む
    const string directoryPath = baseDirectory + modelName + "/";
    //拡張子.fbxを付加
    const string fileName = modelName + ".fbx";
    //連結してフルパスを得る
    const string fullpath = directoryPath + fileName;

    //ファイル名を指定してFBXファイルを読み込む
    if (!fbxImporter->Initialize(fullpath.c_str(), -1, fbxManager->GetIOSettings())) {
        assert(0);
    }

    //シーン生成
    FbxScene* fbxScene = FbxScene::Create(fbxManager, "fbxScene");

    //ファイルからロードしたFBXの情報をシーンにインポート
    fbxImporter->Import(fbxScene);

    FbxGeometryConverter fbx_converter(fbxManager);
    fbx_converter.Triangulate(fbxScene, true);
    fbx_converter.RemoveBadPolygonsFromMeshes(fbxScene);

    //モデル生成
    FbxModel* model = new FbxModel();
    model->name = modelName;
    //FBXノードの数を取得
    int nodeCount = fbxScene->GetNodeCount();
    //あらかじめ必要数分のメモリを確保することで、アドレスがずれるのを予防
    model->nodes.reserve(nodeCount);

    //ルートノードから順に解析してモデルに流し込む
    ParseNodeRecursive(model, fbxScene->GetRootNode());
    model->fbxScene = fbxScene;

    //バッファ生成
    model->CreateBuffers(dev);

    return std::unique_ptr<FbxModel>(model);
}

void FbxInput::ParseNodeRecursive(FbxModel* model, FbxNode* fbxNode, Node* parent)
{
    //モデルにノードを追加
    model->nodes.emplace_back();
    Node& node = model->nodes.back();
    //ノード名を取得
    string name = fbxNode->GetName();

#pragma region FBXノードの情報を解析してノードに記録(ローカル情報)

    //FBXノードのローカル移動情報
    FbxDouble3 rotation = fbxNode->LclRotation.Get();
    FbxDouble3 scaling = fbxNode->LclScaling.Get();
    FbxDouble3 translation = fbxNode->LclTranslation.Get();

    //形式変換して代入
    node.rotation = { (float)rotation[0],(float)rotation[1] ,(float)rotation[2] ,0.0f };
    node.scaling = { (float)scaling[0],(float)scaling[1], (float)scaling[2], 0.0f };
    node.translation = { (float)translation[0],(float)translation[1], (float)translation[2], 1.0f };

    //回転角をDgree(度)からラジアンに変換
    node.rotation.m128_f32[0] = XMConvertToRadians(node.rotation.m128_f32[0]);
    node.rotation.m128_f32[1] = XMConvertToRadians(node.rotation.m128_f32[1]);
    node.rotation.m128_f32[2] = XMConvertToRadians(node.rotation.m128_f32[2]);

    //スケール、回転、平行移動行列の計算
    XMMATRIX matScaling, matRotation, matTranslation;
    matScaling = XMMatrixScalingFromVector(node.scaling);
    matRotation = XMMatrixRotationRollPitchYawFromVector(node.rotation);
    matTranslation = XMMatrixTranslationFromVector(node.translation);

    //ローカル変形行列の計算
    node.transform = XMMatrixIdentity();
    node.transform *= matScaling; //ワールド行列にスケーリングを反映
    node.transform *= matRotation; //回転
    node.transform *= matTranslation; //平行移動

#pragma endregion

    //グローバル変形行列の計算
    node.globalTransform = node.transform;
    if (parent)
    {
        node.parent = parent;
        //親の変形を乗算
        node.globalTransform *= parent->globalTransform;
    }

    //FBXノードのメッシュ情報を解析
    FbxNodeAttribute* fbxNodeAttribute = fbxNode->GetNodeAttribute();
    if (fbxNodeAttribute)
    {
        if (fbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
        {
            model->meshNode = &node;
            ParseMesh(model, fbxNode);
        }
    }


    //子ノードに対して再帰呼び出し
    for (int i = 0; i < fbxNode->GetChildCount(); i++)
    {
        ParseNodeRecursive(model, fbxNode->GetChild(i), &node);
    }
}

void FbxInput::ParseMesh(FbxModel* model, FbxNode* fbxNode)
{
    //ノードのメッシュを取得
    FbxMesh* fbxMesh = fbxNode->GetMesh();

    //頂点座標読み取り
    ParseMeshVertices(model, fbxMesh);
    //面を構成するデータの読み取り
    ParseMeshFaces(model, fbxMesh);
    //マテリアルの読み取り
    ParseMaterial(model, fbxNode);
    //スキニング情報の読み取り
    ParseSkin(model, fbxMesh);
}

#pragma region メッシュ解析用サブ関数
void FbxInput::ParseMeshVertices(FbxModel* model, FbxMesh* fbxMesh)
{
    //auto& vertices = model->vertices;

    ////頂点データの数
    //const int controlPointsCount = fbxMesh->GetControlPointsCount();
    ////必要数だけ頂点データ配列を確保
    //FbxModel::VertexPosNormalUvSkin vert{};
    //model->vertices.resize(controlPointsCount, vert);

    ////FBXメッシュの頂点座標配列を取得
    //FbxVector4* pCoord = fbxMesh->GetControlPoints();

    ////FBXメッシュの全頂点座標をモデル内の配列にコピーする
    //for (int i = 0; i < controlPointsCount; i++)
    //{
    //    FbxModel::VertexPosNormalUvSkin& vertex = vertices[i];
    //    //座標のコピー
    //    vertex.pos.x = (float)pCoord[i][0];
    //    vertex.pos.y = (float)pCoord[i][1];
    //    vertex.pos.z = (float)pCoord[i][2];
    //}
}

void FbxInput::ParseMeshFaces(FbxModel* model, FbxMesh* fbxMesh)
{
    auto& vertices = model->vertices;
    auto& indices = model->indices;

    //1ファイルに複数メッシュのモデルは非対応
    assert(indices.size() == 0);
    //面の数
    const int polygonCount = fbxMesh->GetPolygonCount();
    //UVデータの数
    const int texUVCount = fbxMesh->GetTextureUVCount();
    //UV名リスト
    FbxStringList uvNames;
    fbxMesh->GetUVSetNames(uvNames);

    //必要数だけ頂点データ配列を確保
    FbxModel::VertexPosNormalUvSkin vert{};
    model->vertices.resize(polygonCount * 3, vert);

    //FBXメッシュの頂点座標配列を取得
    FbxVector4* pCoord = fbxMesh->GetControlPoints();
    int indexCount = 0;
    controlPointsData.resize(fbxMesh->GetControlPointsCount());

    //面ごとの情報読み込み
    for (int i = 0; i < polygonCount; i++)
    {
        //面を構成する頂点の数を取得(3なら三角形ポリゴン)
        const int polygonSize = fbxMesh->GetPolygonSize(i);
        assert(polygonSize <= 3);

        //1頂点ずつ処理
        for (int j = 0; j < polygonSize; j++)
        {
            //FBX頂点配列のインデックス
            int controlPointIndex = fbxMesh->GetPolygonVertex(i, j);
            assert(controlPointIndex >= 0);

            controlPointsData[controlPointIndex].push_back(indexCount);

            
            FbxModel::VertexPosNormalUvSkin& vertex = vertices[indexCount];
            //座標のコピー
            vertex.pos.x = (float)pCoord[controlPointIndex][0];
            vertex.pos.y = (float)pCoord[controlPointIndex][1];
            vertex.pos.z = (float)pCoord[controlPointIndex][2];

            //頂点法線読み込み
            FbxVector4 normal;
            if (fbxMesh->GetPolygonVertexNormal(i, j, normal))
            {
                vertex.normal.x = (float)normal[0];
                vertex.normal.y = (float)normal[1];
                vertex.normal.z = (float)normal[2];
            }

            //テクスチャUV読み込み
            if (texUVCount > 0)
            {
                FbxVector2 uvs;
                bool lUnmappesUV;
                //0番決め打ちで読み込み
                if (fbxMesh->GetPolygonVertexUV(i, j, uvNames[0], uvs, lUnmappesUV))
                {
                    vertex.uv.x = (float)uvs[0];
                    vertex.uv.y = (float)uvs[1] * -1.0f;
                }
            }

            //インデックス配列に頂点インデックス追加
            //3頂点目までなら
            if (j < 3)
            {
                //1点追加し、他の2点と三角形を構築する
                indices.push_back(indexCount);
            }
            //4頂点目
            else 
            {
                //3点追加し、
                //四角形の0,1,2,3のうち、2,3,0で三角形を構築する
                int index2 = indices[indices.size() - 1];
                int index3 = controlPointIndex;
                int index0 = indices[indices.size() - 3];
                indices.push_back(index2);
                indices.push_back(index3);
                indices.push_back(index0);
            }

            indexCount++;
            
        }
    }

    FILE* outputfile;         // 出力ストリーム

    fopen_s(&outputfile, "vertices.txt", "w");  // ファイルを書き込み用にオープン(開く)
    if (outputfile == NULL) {          // オープンに失敗した場合
        printf("cannot open\n");         // エラーメッセージを出して
        exit(1);                         // 異常終了
    }
    fprintf(outputfile, "%d\n", model->vertices.size());
    for (auto& vert : model->vertices) {
        fprintf(outputfile, "%f,%f,%f\n", vert.pos.x, vert.pos.y, vert.pos.z);
    }
    fclose(outputfile);          // ファイルをクローズ(閉じる)
}

void FbxInput::ParseMaterial(FbxModel* model, FbxNode* fbxNode)
{
    const int materialCount = fbxNode->GetMaterialCount();
    if (materialCount > 0)
    {
        //先頭のマテリアルを取得
        FbxSurfaceMaterial* material = fbxNode->GetMaterial(0);
        //テクスチャを読み込んだかどうかを表すフラグ
        bool texLoaded = false;

        //テクスチャを読み込む
        if (material)
        {
            //FbxSurfaceLambertクラスかどうかを調べる
            if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
            {
                FbxSurfaceLambert* lambert = static_cast<FbxSurfaceLambert*>(material);

                //環境光係数
                FbxPropertyT<FbxDouble3> ambient = lambert->Ambient;
                model->ambient.x = (float)ambient.Get()[0];
                model->ambient.y = (float)ambient.Get()[1];
                model->ambient.z = (float)ambient.Get()[2];

                //拡散反射光係数
                FbxPropertyT<FbxDouble3> diffuse = lambert->Diffuse;
                model->diffuse.x = (float)diffuse.Get()[0];
                model->diffuse.y = (float)diffuse.Get()[1];
                model->diffuse.z = (float)diffuse.Get()[2];
            }

            //ディフューズテクスチャを取り出す
            const FbxProperty diffuseProperty = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
            if (diffuseProperty.IsValid())
            {
                const FbxFileTexture* tex = diffuseProperty.GetSrcObject<FbxFileTexture>();
                if (tex)
                {
                    const char* filepath = tex->GetFileName();

                    //ファイルパスからファイル名抽出
                    string path_str(filepath);
                    string name = ExtractFileName(path_str);
                    //テクスチャ読み込み
                    LoadTexture(model, baseDirectory + model->name + "/" + name);
                    texLoaded = true;
                }
            }

        }

        //テクスチャが無い場合は白テクスチャを貼る
        if (!texLoaded)
        {
            LoadTexture(model, baseDirectory + defaultTexFileName);
        }
    }
}

void FbxInput::LoadTexture(FbxModel* model, const std::string& fullpath)
{
    HRESULT result = S_FALSE;

    //WICテクスチャのロード
    TexMetadata& metadata = model->metadata;
    ScratchImage& scratchImg = model->scratchImg;

    //ユニコード文字列に変換
    wchar_t wfilepath[128];
    MultiByteToWideChar(CP_ACP, 0, fullpath.c_str(), -1, wfilepath, _countof(wfilepath));
    result = LoadFromWICFile(wfilepath, WIC_FLAGS_NONE, &metadata, scratchImg);
    if (FAILED(result)) {
        assert(0);
    }
}

void FbxInput::ParseSkin(FbxModel* model, FbxMesh* fbxMesh)
{
    //スキニング情報
    FbxSkin* fbxSkin = static_cast<FbxSkin*>(fbxMesh->GetDeformer(0, FbxDeformer::eSkin));
    //スキニング情報が無ければ終了
    if (fbxSkin == nullptr)
    {
        //各頂点について処理
        for (int i = 0; i < model->vertices.size(); i++)
        {
            //最初のボーン(単位行列)の影響を100％にする
            model->vertices[i].boneIndex[0] = 0;
            model->vertices[i].boneWeight[0] = 1.0f;
        }

        return; 
    }

    //ボーン配列の参照
    std::vector<FbxModel::Bone>& bones = model->bones;

    //ボーンの数
    int clusterCount = fbxSkin->GetClusterCount();
    bones.reserve(clusterCount);

    //全てのボーンについて
    for (int i = 0; i < clusterCount; i++)
    {
        //FBXボーン情報
        FbxCluster* fbxCluster = fbxSkin->GetCluster(i);

        //ボーン自体のノードの名前を取得
        const char* boneName = fbxCluster->GetLink()->GetName();

        //新しくボーンを追加し、追加したボーンの参照を得る
        bones.emplace_back(FbxModel::Bone(boneName));
        FbxModel::Bone& bone = bones.back();
        //自作ボーンとFBXのボーンを紐づける
        bone.fbxCluster = fbxCluster;

        //FBXから初期姿勢行列を取得する
        FbxAMatrix fbxMat;
        fbxCluster->GetTransformLinkMatrix(fbxMat);

        //XMMATRIX型に変換する
        XMMATRIX initialPose;
        ConvertMatrixFromFbx(&initialPose, fbxMat);

        //初期姿勢行列の逆行列を得る
        bone.invInitialPose = XMMatrixInverse(nullptr, initialPose);
    }

    //ボーン番号とスキンウェイトのペア
    struct WeightSet
    {
        UINT index;
        float weight;
    };

    //二次元配列(ジャグ配列)
    //list：頂点が影響を受けるボーンの全リスト
    //vector：それを全頂点分
    std::vector<std::list<WeightSet>> weightLists(controlPointsData.size());
    //頂点配列書き換え用参照
    auto& vertices = model->vertices;

    //全てのボーンについて
    for (int i = 0; i < clusterCount; i++)
    {
        //FBXボーン情報
        FbxCluster* fbxCluster = fbxSkin->GetCluster(i);
        //このボーンに影響を受ける頂点の数
        int controlPointIndicesCount = fbxCluster->GetControlPointIndicesCount();
        //このボーンに影響を受ける頂点の配列
        int* controlPointIndices = fbxCluster->GetControlPointIndices();
        double* controlPointWeights = fbxCluster->GetControlPointWeights();

        //影響を受ける全頂点について
        for (int j = 0; j < controlPointIndicesCount; j++)
        {
            //頂点番号
            int controlPointIndex = controlPointIndices[j];
            //スキンウェイト
            float weight = (float)controlPointWeights[j];
            //その頂点の影響を受けるボーンリストに、ボーンとウェイトのペアを追加
            weightLists[controlPointIndex].emplace_back(WeightSet{ (UINT)i,weight });
        }
    }

    //頂点配列書き換え用の参照
    //auto& vertices = model->vertices;
    //各頂点について処理
    for (int i = 0; i < controlPointsData.size(); i++)
    {
        //頂点のウェイトから最も大きい4つを選択
        auto& weightList = weightLists[i];
        //大小比較用のラムダ式を指定して降順にソート
        weightList.sort(
            [](auto const& lhs, auto const& rhs)
            {
                //左の要素のほうが大きければtrue，そうでなければfalseを返す
                return lhs.weight > rhs.weight;
            });

        int weightArrayIndex = 0;
        //降順ソート済ウェイトリストから
        for (int j = 0; j < controlPointsData[i].size(); j++)
        {
            for (auto& weightSet : weightList)
            {
                std::vector<int>& controlPoint = controlPointsData[i];
                int indexCount = controlPoint[j];

                vertices[indexCount].boneIndex[weightArrayIndex] = weightSet.index;
                vertices[indexCount].boneWeight[weightArrayIndex] = weightSet.weight;

                //4つに達したら終了
                if (++weightArrayIndex >= FbxModel::MAX_BONE_INDICES)
                {
                    float weight = 0.0f;
                    //2番目以降のウェイトを合計
                    for (int j = 1; j < FbxModel::MAX_BONE_INDICES; j++)
                    {
                        weight += vertices[indexCount].boneWeight[j];
                    }
                    //合計で1.0f(100%)になるように調整
                    vertices[indexCount].boneWeight[0] = 1.0f - weight;
                    break;
                }
            }
        }
    }
}
#pragma endregion


std::string FbxInput::ExtractFileName(const std::string& path)
{
    size_t pos1;

    //区切り文字"\\"が出てくる一番最後の文字の部分を検索
    pos1 = path.rfind('\\');
    if (pos1 != string::npos)
    {
        return path.substr(pos1 + 1, path.size() - pos1 - 1);
    }

    //区切り文字"/"が出てくる一番最後の文字の部分を検索
    pos1 = path.rfind('/');
    if (pos1 != string::npos)
    {
        return path.substr(pos1 + 1, path.size() - pos1 - 1);
    }

    return path;
}

void FbxInput::ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src)
{
    //行
    for (int i = 0; i < 4; i++)
    {
        //列
        for (int j = 0; j < 4; j++)
        {
            //1要素コピー
            dst->r[i].m128_f32[j] = (float)src.Get(i, j);
        }
    }
}
