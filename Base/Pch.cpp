#include"Pch.h"

namespace Beginner
{
	//ログ表示
	void DebugLogOnConsole(const char* format)
	{
#ifdef _DEBUG
		printf(format);
#endif // _DEBUG
		std::string str = format;
		DebugLogOnOutput(str);
	}

	void DebugLogOnOutput(const std::string& format)
	{
		std::wstring wideChar = TransformToWideChar(format);
		OutputDebugStringW(wideChar.c_str());
	}

	//stringをwstring変換
	std::wstring TransformToWideChar(const std::string& str)
	{
		//一回目の呼び出し
		const int charSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, nullptr, 0);

		std::wstring wstr;//stringのwchar_t番
		wstr.resize(charSize);//得られた文字サイズでリサイズ

		//呼び出し2回目
		MultiByteToWideChar(
			CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(),
			-1, &wstr[0], charSize
		);

		return wstr;
	}

	//値を任意の値に整列
	size_t AlignmentedSize(const size_t size, const size_t alignment)
	{
		size_t align = size + alignment - size % alignment;
		return align;
	}

	//ベクトルを0～１で正規化
	DirectX::XMFLOAT3 FigureVertex(const Vector3& vert, const Size winSize)
	{
		DirectX::XMFLOAT3 normal;

		normal.x = 2.0F * vert.x / winSize.x;
		normal.y = 2.0F * vert.y / winSize.y;
		normal.z = vert.z;

		return normal;
	}

	//ベクトルを0～１で正規化
	DirectX::XMFLOAT3 FigureVertex(const DirectX::XMFLOAT3& vert, const Size winSize)
	{
		return FigureVertex(Vector3(vert.x, vert.y, vert.z), winSize);
	}

	//バッファの作成
	bool CreateBuffer(const HWND hwnd, Microsoft::WRL::ComPtr<ID3D12Resource>& buffer, const size_t bufferSize)
	{
		D3D12_HEAP_PROPERTIES heapprop = {};//ヒープ設定
		{
			heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
			heapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			heapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			heapprop.CreationNodeMask = 0;
			heapprop.VisibleNodeMask = 0;
		}

		D3D12_RESOURCE_DESC resDesc = {};//バッファの設定
		{
			resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			resDesc.Width = bufferSize;
			resDesc.Height = 1;
			resDesc.DepthOrArraySize = 1;
			resDesc.MipLevels = 1;
			resDesc.Format = DXGI_FORMAT_UNKNOWN;
			resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
			resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

			resDesc.SampleDesc.Count = 1;
			resDesc.SampleDesc.Quality = LOW_QUALITY;
		}

		HRESULT result = device->CreateCommittedResource(//バッファの作成
			&heapprop, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
			IID_PPV_ARGS(buffer.ReleaseAndGetAddressOf())
		);

		if (FAILED(result))
		{
			DebugLogOnConsole("バッファの作成が失敗\n");
			return false;
		}

		return true;
	}

	bool CreateBuffer(const HWND hwnd, Microsoft::WRL::ComPtr<ID3D12Resource>& buffer, const DirectX::TexMetadata& metaData)
	{
		D3D12_HEAP_PROPERTIES textureHeap = {};
		{
			textureHeap.Type = D3D12_HEAP_TYPE_DEFAULT;
			textureHeap.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			textureHeap.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			textureHeap.CreationNodeMask = 0;
			textureHeap.VisibleNodeMask = 0;
		}

		D3D12_RESOURCE_DESC resourceDesc = {};
		{
			resourceDesc.Format = metaData.format;
			resourceDesc.Width = metaData.width;
			resourceDesc.Height = (UINT)metaData.height;
			resourceDesc.DepthOrArraySize = (UINT16)metaData.arraySize;
			resourceDesc.MipLevels = (UINT16)metaData.mipLevels;
			resourceDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metaData.dimension);
			resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
			resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

			resourceDesc.SampleDesc.Count = 1;
			resourceDesc.SampleDesc.Quality = LOW_QUALITY;
		}

		HRESULT result = device->CreateCommittedResource(//バッファ作成
			&textureHeap, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr, IID_PPV_ARGS(buffer.ReleaseAndGetAddressOf())
		);

		if (FAILED(result))
		{
			DebugLogOnConsole("テクスチャバッファの作成が失敗\n");
			return false;
		}

		return true;
	}

	bool CreateDepthBuffer(const HWND hwnd, Microsoft::WRL::ComPtr<ID3D12Resource>& buffer)
	{
		D3D12_RESOURCE_DESC depthDesc = {};
		{
			depthDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			depthDesc.Width = (UINT64)GetWindowSize(hwnd).x;
			depthDesc.Height = (UINT)GetWindowSize(hwnd).y;
			depthDesc.DepthOrArraySize = 1;
			depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
			depthDesc.SampleDesc.Count = 1;
			depthDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
			depthDesc.MipLevels = 1;
			depthDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
			depthDesc.Alignment = 0;
		}

		D3D12_HEAP_PROPERTIES depthPro = {};
		{
			depthPro.Type = D3D12_HEAP_TYPE_DEFAULT;
			depthPro.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			depthPro.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		}

		D3D12_CLEAR_VALUE depthClear = {};
		{
			depthClear.DepthStencil.Depth = 1.0F;
			depthClear.Format = DXGI_FORMAT_D32_FLOAT;
		}

		HRESULT result = device->CreateCommittedResource(//バッファ作成
			&depthPro, D3D12_HEAP_FLAG_NONE, &depthDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&depthClear, IID_PPV_ARGS(buffer.ReleaseAndGetAddressOf())
		);

		if (FAILED(result))
		{
			DebugLogOnConsole("深度バッファの作成が失敗\n");
			return false;
		}

		return true;
	}

	//Buffer Map
	VertexUV* BufferMap(Microsoft::WRL::ComPtr<ID3D12Resource>& buffer, std::vector<VertexUV>::iterator begin, std::vector<VertexUV>::iterator end, const bool isunmap)
	{
		VertexUV* mapTarget = nullptr;

		HRESULT result = buffer.Get()->Map(0, nullptr, (void**)&mapTarget);//全範囲マップ

		if (FAILED(result))
		{
			DebugLogOnConsole("頂点マップが失敗しました\n");
			return nullptr;
		}

		std::copy(begin, end, mapTarget);//データをマップ

		if (isunmap)
		{
			buffer->Unmap(0, nullptr);
		}

		return mapTarget;
	}

	unsigned short* BufferMap(Microsoft::WRL::ComPtr<ID3D12Resource>& buffer, std::vector<unsigned short>::iterator begin, std::vector<unsigned short>::iterator end, const bool isunmap)
	{
		unsigned short* mapTarget = nullptr;

		HRESULT result = buffer.Get()->Map(0, nullptr, (void**)&mapTarget);//全範囲マップ

		if (FAILED(result))
		{
			DebugLogOnConsole("頂点マップが失敗しました\n");
			return nullptr;
		}

		std::copy(begin, end, mapTarget);//データをマップ

		if (isunmap)
		{
			buffer->Unmap(0, nullptr);
		}

		return mapTarget;
	}

	unsigned* BufferMap(Microsoft::WRL::ComPtr<ID3D12Resource>& buffer, std::vector<unsigned>::iterator begin, std::vector<unsigned>::iterator end, const bool isunmap)
	{
		unsigned* mapTarget = nullptr;

		HRESULT result = buffer.Get()->Map(0, nullptr, (void**)&mapTarget);//全範囲マップ

		if (FAILED(result))
		{
			DebugLogOnConsole("頂点マップが失敗しました\n");
			return nullptr;
		}

		std::copy(begin, end, mapTarget);//データをマップ

		if (isunmap)
		{
			buffer->Unmap(0, nullptr);
		}

		return mapTarget;
	}

	Vector3* BufferMap(Microsoft::WRL::ComPtr<ID3D12Resource>& buffer, std::vector<Vector3>::iterator begin, std::vector<Vector3>::iterator end, const bool isunmap)
	{
		Vector3* mapTarget = nullptr;

		HRESULT result = buffer.Get()->Map(0, nullptr, (void**)&mapTarget);//全範囲マップ

		if (FAILED(result))
		{
			DebugLogOnConsole("頂点マップが失敗しました\n");
			return nullptr;
		}

		std::copy(begin, end, mapTarget);//データをマップ

		if (isunmap)
		{
			buffer->Unmap(0, nullptr);
		}

		return mapTarget;
	}

	Vertex* BufferMap(Microsoft::WRL::ComPtr<ID3D12Resource>& buffer, std::vector<Vertex>::iterator begin, std::vector<Vertex>::iterator end, const bool isunmap)
	{
		Vertex* mapTarget = nullptr;

		HRESULT result = buffer.Get()->Map(0, nullptr, (void**)&mapTarget);//全範囲マップ

		if (FAILED(result))
		{
			DebugLogOnConsole("頂点マップが失敗しました\n");
			return nullptr;
		}

		std::copy(begin, end, mapTarget);//データをマップ

		if (isunmap)
		{
			buffer->Unmap(0, nullptr);
		}

		return mapTarget;
	}

	uint8_t* BufferMap(Microsoft::WRL::ComPtr<ID3D12Resource>& buffer, const DirectX::Image& image, const bool isunmap)
	{
		uint8_t* mapTexture = nullptr;

		HRESULT result = buffer->Map(0, nullptr, (void**)&mapTexture);//全範囲マップ

		if (FAILED(result))
		{
			DebugLogOnConsole("テクスチャのマップが失敗\n");
			return nullptr;
		}

		uint8_t* srcAddress = image.pixels;
		size_t rowPitch = AlignmentedSize(image.rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);

		for (int y = 0; y < image.height; y++)//画像データをアライメントしてコピー
		{
			std::copy_n(srcAddress, rowPitch, mapTexture);
			srcAddress += image.rowPitch;
			mapTexture += rowPitch;
		}

		if (isunmap)
		{
			buffer->Unmap(0, nullptr);
		}

		return mapTexture;
	}

	//頂点数からポリゴン図形を取得
	D3D12_PRIMITIVE_TOPOLOGY GetPrimitiveTopology(const unsigned short vertexCount)
	{
		switch (vertexCount)
		{
		case 1://点
			return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;

		case 2://線
			return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;

		default://多角形
			return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		}
	}

	//グラフィックパイプライン用のプリミティブを取得
	D3D12_PRIMITIVE_TOPOLOGY_TYPE GetPrimitiveTopologyType(const D3D12_PRIMITIVE_TOPOLOGY topology)
	{
		switch (topology)
		{
		case D3D_PRIMITIVE_TOPOLOGY_POINTLIST:
			return D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

		case D3D_PRIMITIVE_TOPOLOGY_LINESTRIP:
			return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;

		default:
			return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		}
	}

	//ウィンドウサイズを取得
	Size GetWindowSize(const HWND hwnd)
	{
		RECT rect = {};

		if (FALSE == GetWindowRect(hwnd, &rect))
		{
			return Size(0, 0);
		}

		return { (float)(rect.right - rect.left),(float)(rect.bottom - rect.top) };

	}

	//XMFLOAT3とVector3間の変換
	Vector3 TransformToVector3(const DirectX::XMFLOAT3& mfloat)
	{
		return Vector3(mfloat.x, mfloat.y, mfloat.z);
	}

	DirectX::XMFLOAT3 TransformToXMFLOAT3(const Vector3& vector)
	{
		return DirectX::XMFLOAT3(vector.x, vector.y, vector.z);
	}

	//Shaderのエラーを表示
	void OutPutShaderError(HRESULT& result, Microsoft::WRL::ComPtr<ID3DBlob>& error)
	{
		if (result == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
		{
			DebugLogOnConsole("シェーダーファイルが見つかりません\n");
		}
		else
		{
			std::string message;//エラーメッセージ受け取り
			message.resize(error->GetBufferSize());

			std::copy_n((char*)error->GetBufferPointer(), error->GetBufferSize(), message.begin());
			message += "\n";

			DebugLogOnConsole(message.c_str());//エラー表示
		}
	}

	//図形の中心座標を取得
	Vector3 VertexToPivot(const unsigned int vertCount, const Vector3 vert[])
	{
		Vector3 pivot(0, 0, 0);//中心

		for (unsigned int i = 0; i < vertCount; i++)
		{
			pivot += vert[i];
		}

		return pivot / vertCount;
	}

	//ルートシグネチャのエラー表示
	void OutPutRootSignatureError(Microsoft::WRL::ComPtr<ID3DBlob>& error)
	{
		std::string message;//エラーメッセージ受け取り
		message.resize(error->GetBufferSize());

		std::copy_n((char*)error->GetBufferPointer(), error->GetBufferSize(), message.begin());
		message += "\n";

		DebugLogOnConsole(message.c_str());//エラー表示
	}

	//ビューの作製
	void CreateConstantView(const HWND hwnd, Microsoft::WRL::ComPtr<ID3D12Resource>& constBuffer, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& heap)
	{
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		{
			cbvDesc.BufferLocation = constBuffer->GetGPUVirtualAddress();
			cbvDesc.SizeInBytes = (UINT)constBuffer->GetDesc().Width;
		}

		//定数ビューを作成
		const auto handle = heap->GetCPUDescriptorHandleForHeapStart();
		device->CreateConstantBufferView(&cbvDesc, handle);
	}

	//SRVを作成
	void CreateShaderResourceView(const HWND hwnd, Microsoft::WRL::ComPtr<ID3D12Resource>& texture, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& heap, const DXGI_FORMAT format)
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		{
			srvDesc.Format = format;
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;
		}

		//srvをアドレス移動してから作成
		D3D12_CPU_DESCRIPTOR_HANDLE handle = heap->GetCPUDescriptorHandleForHeapStart();
		handle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		device->CreateShaderResourceView(texture.Get(), &srvDesc, handle);
	}

	void CreateDepthView(const HWND hwnd, Microsoft::WRL::ComPtr<ID3D12Resource>& buffer, const D3D12_CPU_DESCRIPTOR_HANDLE handle)
	{
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		{
			dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
			dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
			dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
		}

		device->CreateDepthStencilView(buffer.Get(), &dsvDesc, handle);
	}
}