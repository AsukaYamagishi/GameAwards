#include "DebugText.h"

void DebugText::Init(UINT texnumber)
{
	//全てのデバッグテキストデータに対して
	for (int i = 0; i < _countof(textSprites); i++)
	{
		//デバッグテキスト用文字画像を生成する
		textSprites[i] = Sprite::CreateSprite(texnumber, { 0,0 });
	}
}

void DebugText::PrintDebugText(const std::string & text, float x, float y, float scale)
{
	//全ての文字について
	for (int i = 0; i < text.size(); i++)
	{
		//最大文字数超過
		if (textIndex >= maxCharCount) {
			break;
		}

		//1文字取り出す(ASCIIコード)
		const unsigned char& character = text[i];

		//ASCIIコードを2段分飛ばした番号を計算
		int fontIndex = character - 32;
		if (character >= 0x7f) {
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		// 座標計算
		textSprites[textIndex]->SetPosition({ x + fontWidth * scale * i, y });
		textSprites[textIndex]->SetTextureRect({ (float)fontIndexX * fontWidth, (float)fontIndexY * fontHeight }, { (float)fontWidth, (float)fontHeight });
		textSprites[textIndex]->SetSize({ fontWidth * scale, fontHeight * scale });

		//文字を一つ進める
		textIndex += 1;
	}
}

void DebugText::DrawAll(ID3D12GraphicsCommandList * cmdList)
{
	//全ての文字のスプライトに対して
	for (int i = 0; i < textIndex; i++)
	{
		// スプライト描画
		textSprites[i]->Draw();
	}

	//配列初期化
	textIndex = 0;
}
