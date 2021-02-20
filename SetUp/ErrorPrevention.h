#pragma once

#include"..\Base\Pch.h"

namespace Beginner
{

	bool CreateFence();//フェンス初期化
	void WaitGPUCommandEnd();//GPU処理が終わるまで待つ

	void RenderTargetBarrier(const Microsoft::WRL::ComPtr<ID3D12Resource>&);//レンダーターゲットバリア
	void PresentBarrier(const Microsoft::WRL::ComPtr<ID3D12Resource>&);//フリップバリア
	void RegionBarrier(const Microsoft::WRL::ComPtr<ID3D12Resource>&);//Region時のバリア

	//デバッグ用のレイヤーを有効化
	bool EnableDebugLayer();
}