#pragma once
#include<Windows.h>
#include<string>

#include"Sprite.h"

class DebugText
{
public:
	//定数の宣言
	//デバッグテキスト用のテクスチャ番号を指定
	static const int maxCharCount = 256; //1フレームでの最大表示文字数
	static const int fontWidth = 9; //1文字分の横幅
	static const int fontHeight = 18; //1文字分の縦幅
	static const int fontLineCount = 14; //画像内1行分の文字数

private:
	//メンバ変数
	//スプライトの配列
	Sprite* textSprites[maxCharCount];
	//配列の添え字
	int textIndex = 0;


public:
	//メンバ関数
	//初期化
	void Init(UINT texnumber);
	//文字列の追加
	void PrintDebugText(const std::string& text, float x, float y, float scale = 1.0f);
	//文字列の表示
	void DrawAll(ID3D12GraphicsCommandList* cmdList);

};

