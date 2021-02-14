#include"Common.h"

namespace Beginner
{
	HWND beginHwnd;//対象のウィンドウハンドル
	std::list<Base*> outputObject;//描画対象オブジェクト
	Eye* mainEye;//視点
	Microsoft::WRL::ComPtr<ID3D12Device> device;//描画デバイス
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;//命令リスト
}