#pragma once

#include"Resource.h"
#include"DirectCore.h"
#include"..\Sprite\Figure.h"
#include"..\Sprite\Sprite.h"
#include"..\3D\Model.h"
#include"..\Shader\Shader.h"

namespace Beginner
{

	//ユーザー側が使うDirectX操作
	class Begin
	{
	public:
		Begin() :rtBuffer(), depthBuffer(), prevention(), command(), graphics(), rtvHeap(), init(false) {}
		~Begin(){DebugLogOnConsole("Widelyクラスが破棄\n");}

		bool Init(const HWND);//DirectXを初期化
		void FlameStart();//フレーム開始処理
		bool FlameEnd();//フレーム終了処理


	private:
		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> rtBuffer;//RenderTargetのBuffer
		Microsoft::WRL::ComPtr<ID3D12Resource> depthBuffer;
		DescriptorHeap rtvHeap, dsvHeap;

		DirectCommand command;//DirectXの命令オブジェクト
		DirectGraphics graphics;//DirectXの描画オブジェクト
		Prevention prevention;//エラー防止オブジェクト

		bool init : 1;//初期化済みフラグ

		D3D12_CPU_DESCRIPTOR_HANDLE GetRTVAddress();//RTのアドレスを取得

		void Draw();//登録されたオブジェクトを描画
		bool CommandAction();//ユーザー側からの命令を処理
		bool DeviceCommandInits();//クラスの初期化をまとめる
		bool BufferInits();//BufferとViewの初期化
	};

}