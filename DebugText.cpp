#include "DebugText.h"

void DebugText::Init(UINT texnumber)
{
	//�S�Ẵf�o�b�O�e�L�X�g�f�[�^�ɑ΂���
	for (int i = 0; i < _countof(textSprites); i++)
	{
		//�f�o�b�O�e�L�X�g�p�����摜�𐶐�����
		textSprites[i] = Object2D::CreateSprite(texnumber, { 0,0 });
	}
}

void DebugText::PrintDebugText(const std::string & text, float x, float y, float scale)
{
	//�S�Ă̕����ɂ���
	for (int i = 0; i < text.size(); i++)
	{
		//�ő啶��������
		if (textIndex >= maxCharCount) {
			break;
		}

		//1�������o��(ASCII�R�[�h)
		const unsigned char& character = text[i];

		//ASCII�R�[�h��2�i����΂����ԍ����v�Z
		int fontIndex = character - 32;
		if (character >= 0x7f) {
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		// ���W�v�Z
		textSprites[textIndex]->SetPosition({ x + fontWidth * scale * i, y });
		textSprites[textIndex]->SetTextureRect({ (float)fontIndexX * fontWidth, (float)fontIndexY * fontHeight }, { (float)fontWidth, (float)fontHeight });
		textSprites[textIndex]->SetSize({ fontWidth * scale, fontHeight * scale });

		//��������i�߂�
		textIndex += 1;
	}
}

void DebugText::DrawAll(ID3D12GraphicsCommandList * cmdList)
{
	//�S�Ă̕����̃X�v���C�g�ɑ΂���
	for (int i = 0; i < textIndex; i++)
	{
		// �X�v���C�g�`��
		textSprites[i]->Draw();
	}

	//�z�񏉊���
	textIndex = 0;
}
