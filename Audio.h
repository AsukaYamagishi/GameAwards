#pragma once
#include<Windows.h>
#include<xaudio2.h>
#include<wrl.h>

using namespace Microsoft::WRL;

//オーディオコールバック
class XAudio2VoiceCallback : public IXAudio2VoiceCallback
{
public:
	//ボイス処理パスの開始時
	void OnVoiceProcessingPassStart(UINT32 ByteRequired) {};
	//ボイス処理パスの終了時
	void OnVoiceProcessingPassEnd() {};
	//バッファストリームの再生が終了した時
	void OnStreamEnd() {};
	//バッファの使用開始時
	void OnBufferStart(void* pBufferContext) {};
	//バッファの末尾に達したとき
	void OnBufferEnd(void* pBufferContext) { delete[] pBufferContext; };
	//再生がループ位置に達したとき
	void OnLoopEnd(void* pBufferContext) {};
	//ボイスの実行エラー時
	void OnVoiceError(void* pBufferContext, HRESULT Error) {};
};

//オーディオ
class Audio
{
#pragma region 構造体

public: //構造体
	//チャンクヘッダ
	struct ChunkHeader
	{
		char id[4]; //チャンク毎のID
		int size; //チャンクサイズ
	};
	//RIFFヘッダチャンク
	struct RiffHeader
	{
		ChunkHeader chunk; //"RIFF"
		char type[4]; //"WAVE"
	};
	//FMTチャンク
	struct FormatChunk
	{
		ChunkHeader chunk; //"fmt"
		WAVEFORMATEX fmt; //波形フォーマット
	};
	//音声データ
	struct SoundData
	{
		//波形フォーマット
		WAVEFORMATEX wfex;
		//バッファの先頭アドレス
		BYTE* pBuffer;
		//バッファのサイズ
		unsigned int bufferSize;
	};

#pragma endregion

#pragma region 変数

public: //メンバ変数
	ComPtr<IXAudio2> xAudio2;
	enum IsLoop {
		loop,
		not
	};

private: //メンバ変数
	IXAudio2MasteringVoice* masterVoice;
	XAudio2VoiceCallback voiceCallback;

#pragma endregion

#pragma region 関数
public: //メンバ関数
	//音声関係初期化
	void Init();
	//音声データ読み込み
	SoundData SoundLoadWave(const char* filename);
	//音声再生(ループ対応)
	void SoundPlayWave(IXAudio2* xAudio2, const SoundData& soundData, IsLoop isLoop = not);
	//再生停止
	void SoundStop(IXAudio2* xAudio2, const SoundData& soundData);
	//音声データ解放
	void SoundUnLoad(SoundData* soundData);

#pragma endregion

};