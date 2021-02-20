#include"Common.h"

namespace Beginner
{
	HWND beginHwnd;//対象のウィンドウハンドル
	std::list<Base*> outputObject;//描画対象オブジェクト
	Eye* mainEye;//視点

	//描画ポインタ
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	Microsoft::WRL::ComPtr<IDXGIFactory6> dxgiFactory;
	Microsoft::WRL::ComPtr<IDXGIAdapter> useAdapter;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;

	//命令ポインタ
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> allocator;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> cmdQueue;

	//命令エラー防止ポインタ
	Microsoft::WRL::ComPtr<ID3D12Fence> fence;
	UINT64 fenceValue = 0;
}