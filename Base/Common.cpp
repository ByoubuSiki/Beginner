#include"Common.h"

namespace Beginner
{
	std::list<Base*> registBase;//描画対象オブジェクト
	Eye* mainEye;//視点
	Microsoft::WRL::ComPtr<ID3D12Device> device;//描画デバイス
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;//命令リスト
}