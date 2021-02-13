#include"DirectGraphics.h"

namespace Beginner
{

	DirectGraphics::DirectGraphics()
		:device(nullptr),
		dxgiFactory(nullptr),
		useAdapter(nullptr),
		swapChain(nullptr)
	{

	}

	DirectGraphics::~DirectGraphics()
	{

	}

	//DirectXデバイスの初期化
	bool DirectGraphics::CreateDevice()
	{
		//使えるバージョンを繰り返し調べる
		std::vector<D3D_FEATURE_LEVEL> featureList =
		{
			D3D_FEATURE_LEVEL_12_1,
			D3D_FEATURE_LEVEL_12_0,
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1,
			D3D_FEATURE_LEVEL_1_0_CORE
		};

		for (unsigned short i = 0; i < featureList.size(); i++)
		{
			HRESULT result = D3D12CreateDevice(//デバイスの作成
				nullptr, featureList[i], IID_PPV_ARGS(device.ReleaseAndGetAddressOf())
			);

			//デバイスの作成が成功
			if (SUCCEEDED(result))
			{
				return true;
			}
		}

		DebugLogOnConsole("デバイスの作成が失敗\n");
		return false;
	}

	//DXGIの初期化
	bool DirectGraphics::CreateDXGIFactory()
	{
#ifdef _DEBUG
		HRESULT result = CreateDXGIFactory2(//Errorを表示する
			DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(dxgiFactory.ReleaseAndGetAddressOf())
		);
#else
		//Errorを表示させない
		HRESULT result = CreateDXGIFactory1(IID_PPV_ARGS(dxgiFactory.ReleaseAndGetAddressOf()));
#endif // _DEBUG

		//DXGIの作成が成功
		if (SUCCEEDED(result))
		{
			return true;
		}

		DebugLogOnConsole("DXGIFactoryの作成が失敗\n");
		return false;
	}

	//使用するアダプタの取得
	bool DirectGraphics::GetUseAdapter()
	{
		HRESULT result = dxgiFactory->EnumAdapters(0, useAdapter.ReleaseAndGetAddressOf());

		//使用できるアダプタが存在しない
		if (FAILED(result))
		{
			DebugLogOnConsole("アダプターの取得が失敗\n");
			return false;
		}

		return true;
	}

	//1段階目の初期化
	bool DirectGraphics::CreateUntilAdapter()
	{
		//アダプタの作成までが成功
		if (CreateDevice() && CreateDXGIFactory() && GetUseAdapter())
		{
			return true;
		}

		return false;
	}

	//スワップチェインの作成
	bool DirectGraphics::CreateSwapChain(
		const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& cmdQueue, const HWND hwnd)
	{
		DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};//スワップチェインの設定
		{
			swapchainDesc.Width = (UINT)GetWindowSize(hwnd).x;
			swapchainDesc.Height = (UINT)GetWindowSize(hwnd).y;
			swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapchainDesc.Stereo = false;
			swapchainDesc.SampleDesc.Count = 1;
			swapchainDesc.SampleDesc.Quality = LOW_QUALITY;
			swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
			swapchainDesc.BufferCount = 2;
			swapchainDesc.Scaling = DXGI_SCALING_STRETCH;//拡縮自在
			swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//フリップ後はすぐに破棄
			swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
			swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;//ウィンドウとフルスクリーンの切り替え可能
		}

		HRESULT result = dxgiFactory->CreateSwapChainForHwnd(//DXGIオブジェクトの作成
			cmdQueue.Get(), hwnd, &swapchainDesc, nullptr, nullptr,
			(IDXGISwapChain1**)swapChain.ReleaseAndGetAddressOf()
		);

		//スワップチェインの作成が失敗
		if (FAILED(result))
		{
			DebugLogOnConsole("スワップチェインの作成が失敗\n");
			return false;
		}

		return true;
	}

	//バックバッファ数を返す
	UINT DirectGraphics::GetBackBufferSize()
	{
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

		HRESULT result = swapChain->GetDesc(&swapChainDesc);//スワップチェインの設定を取得

		if (FAILED(result))
		{
			DebugLogOnConsole("スワップチェインの設定が取得できませんでし\n");
			return 0;
		}

		return swapChainDesc.BufferCount;
	}

	//RTVのバッファ数だけ関連付け
	bool DirectGraphics::CreateBackBuffer(
		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& backBuffer,
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle)
	{
		//バッファサイズ変更
		const UINT backBufferSize = GetBackBufferSize();
		backBuffer.resize((size_t)backBufferSize);

		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		{
			rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		}

		for (unsigned int index = 0; index < backBuffer.size(); index++)
		{
			//関連させるバッファを取得
			HRESULT result = swapChain->GetBuffer(
				index, IID_PPV_ARGS(backBuffer[index].ReleaseAndGetAddressOf())
			);

			if (FAILED(result))
			{
				DebugLogOnConsole("スワップチェインのバッファ取得が失敗\n");
				return false;
			}

			//関連付けして次のアドレスへ移動
			device->CreateRenderTargetView(backBuffer[index].Get(), &rtvDesc, cpuHandle);
			cpuHandle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		}

		return true;
	}

	//残った初期化を行う
	bool DirectGraphics::CreateUntilEnd(
		const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& cmdQueue,
		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& backBuffer,
		const D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, const HWND hwnd
	)
	{
		//スワップチェインの作成とバッファとの関連付け
		if (CreateSwapChain(cmdQueue, hwnd) && CreateBackBuffer(backBuffer, cpuHandle))
		{
			return true;
		}

		return false;
	}

	Microsoft::WRL::ComPtr<ID3D12Device> DirectGraphics::GetDevice()
	{
		return device;
	}

	Microsoft::WRL::ComPtr<IDXGIFactory6> DirectGraphics::GetDxgiFactory()
	{
		return dxgiFactory;
	}

	Microsoft::WRL::ComPtr<IDXGIAdapter> DirectGraphics::GetAdapter()
	{
		return useAdapter;
	}

	Microsoft::WRL::ComPtr<IDXGISwapChain4> DirectGraphics::GetSwapChain()
	{
		return swapChain;
	}
}