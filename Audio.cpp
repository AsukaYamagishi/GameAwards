#include "Audio.h"
#include<fstream>
#include<cassert>


//初期化
void Audio::Init()
{
	HRESULT result;

	//XAudioエンジンのインスタンスを生成
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	//マスターボイスを生成
	result = xAudio2->CreateMasteringVoice(&masterVoice);
}

//wavファイル読み込み
Audio::SoundData Audio::SoundLoadWave(const char * filename)
{
	{
		HRESULT result;

		////ファイルオープン
		//ファイル入力ストリームのインスタンス
		std::ifstream file;
		//wavファイルをバイナリモードで開く
		file.open(filename, std::ios_base::binary);
		//ファイルオープン失敗を検出する
		assert(file.is_open());


		////wavデータ読み込み
		//RIFFヘッダーの読み込み
		RiffHeader riff;
		file.read((char*)&riff, sizeof(riff));
		//ファイルがRIFFかチェック
		if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
			assert(0);
		}
		//タイプがWAVEかチェック
		if (strncmp(riff.type, "WAVE", 4) != 0) {
			assert(0);
		}
		//Formatチャンクの読み込み
		FormatChunk format = {};
		//チャンクヘッダーの確認
		file.read((char*)&format, sizeof(ChunkHeader));
		if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
			assert(0);
		}

		//チャンク本体の読み込み
		assert(format.chunk.size <= sizeof(format.fmt));
		file.read((char*)&format.fmt, format.chunk.size);
		//Dataチャンクの読み込み
		ChunkHeader data;
		file.read((char*)&data, sizeof(data));
		//JUNKチャンクを検出した場合
		if (strncmp(data.id, "JUNK", 4) == 0) {
			//読み取り位置をJUNKチャンクの終わりまで進める
			file.seekg(data.size, std::ios_base::cur);
			//再読み込み
			file.read((char*)&data, sizeof(data));
		}
		if (strncmp(data.id, "data", 4) != 0) {
			assert(0);
		}

		//Dataチャンクのデータ部(波形データ)の読み込み
		char* pBuffer = new char[data.size];
		file.read(pBuffer, data.size);

		//Waveファイルを閉じる
		file.close();


		//読み込んだ音声データをreturnする
		SoundData soundData = {};

		soundData.wfex = format.fmt;
		soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
		soundData.bufferSize = data.size;

		return soundData;
	}
}

//音楽再生
void Audio::SoundPlayWave(IXAudio2 * xAudio2, const SoundData & soundData, IsLoop isLoop)
{
	HRESULT result;

	//波形フォーマットを元にSourceVoiceの生成
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	//再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	switch (isLoop)
	{
		case loop:
			buf.LoopCount = 255;
			break;

		default:
			buf.LoopCount = 0;
			break;
	}
	

	//波形データの再生
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	if (FAILED(result)) {
		delete[] soundData.pBuffer;
		assert(0);
		return;
	}
	result = pSourceVoice->Start();
	if (FAILED(result)) {
		delete[] soundData.pBuffer;
		assert(0);
		return;
	}
}

//再生終了
void Audio::SoundStop(IXAudio2* xAudio2, const SoundData& soundData)
{
	HRESULT result;

	//波形フォーマットを元にSourceVoiceの生成
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex, 0, 2.0f, &voiceCallback);
	assert(SUCCEEDED(result));

	//再生停止
	result = pSourceVoice->ExitLoop();
}

//データ解放
void Audio::SoundUnLoad(SoundData * soundData)
{
	//xAudio2のインスタンスを解放
	xAudio2.Reset();

	//バッファのメモリを解放
	delete[] soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}
