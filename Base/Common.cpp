#include"Common.h"

namespace Beginner
{
	HWND beginHwnd;//�Ώۂ̃E�B���h�E�n���h��
	std::list<Base*> outputObject;//�`��ΏۃI�u�W�F�N�g
	Eye* mainEye;//���_

	//�`��|�C���^
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	Microsoft::WRL::ComPtr<IDXGIFactory6> dxgiFactory;
	Microsoft::WRL::ComPtr<IDXGIAdapter> useAdapter;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;

	//���߃|�C���^
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> allocator;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> cmdQueue;

	//���߃G���[�h�~�|�C���^
	Microsoft::WRL::ComPtr<ID3D12Fence> fence;
	UINT64 fenceValue = 0;
}