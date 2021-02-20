#include"Sprite.h"

namespace Beginner
{
	std::list<Sprite> spriteList;//画像のメモリ

	//テクスチャをコピー
	void Sprite::CopyTexture()
	{
		const DirectX::Image* image = scratch.GetImage(0, 0, 0);

		D3D12_TEXTURE_COPY_LOCATION copyLocationUpload = {};
		{
			copyLocationUpload.pResource = uploadBuffer.Get();
			copyLocationUpload.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
			copyLocationUpload.PlacedFootprint.Offset = 0;
			copyLocationUpload.PlacedFootprint.Footprint.Width = (UINT)metaData.width;
			copyLocationUpload.PlacedFootprint.Footprint.Height = (UINT)metaData.height;
			copyLocationUpload.PlacedFootprint.Footprint.Depth = (UINT)metaData.depth;
			copyLocationUpload.PlacedFootprint.Footprint.RowPitch =
				(UINT)AlignmentedSize(image->rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);
			copyLocationUpload.PlacedFootprint.Footprint.Format = image->format;
		}

		D3D12_TEXTURE_COPY_LOCATION copyLocationTexture = {};
		{
			copyLocationTexture.pResource = textureBuffer.Get();
			copyLocationTexture.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
			copyLocationTexture.SubresourceIndex = 0;
		}

		//画像データ転送
		commandList->CopyTextureRegion(&copyLocationTexture, 0, 0, 0, &copyLocationUpload, nullptr);

		//バリア
		D3D12_RESOURCE_BARRIER barrier = {};
		{
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.pResource = textureBuffer.Get();
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
		}

		commandList->ResourceBarrier(1, &barrier);
	}

	//テクスチャのコピーが完了するまで停止
	bool Sprite::WaitCopyTexture()
	{
		//命令の受付を終了
		if (FAILED(commandList->Close()))
		{
			DebugLogOnConsole("命令のクローズ処理が失敗\n");
			return false;
		}

		//命令を実行 処理完了まで停止
		ID3D12CommandList* commandLists[] = { commandList.Get() };
		cmdQueue->ExecuteCommandLists(1, commandLists);
		WaitGPUCommandEnd();

		//命令を空にする
		if (FAILED(allocator->Reset()))
		{
			DebugLogOnConsole("命令アロケータのリセットが失敗\n");
			return false;
		}

		//コマンドリストをリセット
		if (FAILED(commandList->Reset(allocator.Get(), nullptr)))
		{
			DebugLogOnConsole("コマンドリストのリセットが失敗\n");
			return false;
		}

		return true;
	}
	//GPUの命令処理が終わるまで停止
	void Sprite::WaitGPUCommandEnd()
	{
		cmdQueue->Signal(fence.Get(), ++fenceValue);

		//GPUの処理が終わるまで待つ
		if (fence->GetCompletedValue() != fenceValue)
		{
			//イベントの作成
			HANDLE eventHandle = CreateEvent(nullptr, false, false, nullptr);

			fence->SetEventOnCompletion(fenceValue, eventHandle);

			if (eventHandle == 0)
			{
				return;
			}

			//イベント発生まで停止
			WaitForSingleObject(eventHandle, INFINITE);

			CloseHandle(eventHandle);//イベント終了
		}
	}

	//頂点座標を決定
	void Sprite::SetVertex(float imageWidth, float imageHeight)
	{
		imageWidth /= 2.0F;
		imageHeight /= 2.0F;

		//左上の頂点から右回りに頂点を代入 中心を原点にあわせる
		vertexUV.push_back({ { -imageWidth,imageHeight,0.0F }, { 0.0F,0.0F } });
		vertexUV.push_back({ { imageWidth,imageHeight,0.0F }, { 1.0F,0.0F } });
		vertexUV.push_back({ { imageWidth,-imageHeight,0.0F }, { 1.0F,1.0F } });
		vertexUV.push_back({ { -imageWidth,-imageHeight,0.0F }, { 0.0F,1.0F } });
	}

	//頂点をマップ
	bool Sprite::CreateSpriteBuffer()
	{
		std::vector<unsigned short> index;//頂点バッファの構成順番

		VertexIndexOrder(index, GetWindowSize());//頂点の構成順番

		//頂点バッファとインデックスバッファを作成
		if (!CreateBuffer(vertexBuffer, sizeof(vertexUV[0]) * vertexUV.size()) ||
			!CreateBuffer(indexBuffer, sizeof(index[0]) * index.size()))
		{
			return false;
		}

		//データのマップをする
		if (BufferMap(vertexBuffer, vertexUV.begin(), vertexUV.end(), true) == nullptr ||
			BufferMap(indexBuffer, index.begin(), index.end(), true) == nullptr)
		{
			return false;
		}

		//ビューを設定
		SetVertexView(sizeof(vertexUV[0]) * vertexUV.size(), sizeof(vertexUV[0]), vertexBuffer->GetGPUVirtualAddress());
		SetIndexView((UINT)(index.size() * sizeof(index[0])), indexBuffer->GetGPUVirtualAddress());

		return true;
	}

	//頂点の接続順番を構成
	void Sprite::VertexIndexOrder(std::vector<unsigned short>& index, const Size winSize)
	{
		for (unsigned int i = 0; i < vertexUV.size(); i++)
		{
			vertexUV[i].vertex = FigureVertex(vertexUV[i].vertex, winSize);//クリップ座標で表現

			//三角形を構成する順につないでゆく
			if (i < 3)
			{
				index.push_back(i);
			}
			else if (i >= 3)
			{
				index.push_back(index[0]);
				index.push_back(index[index.size() - 2]);
				index.push_back(i);
			}
		}
	}

	//頂点ビューの設定
	void Sprite::SetVertexView(const size_t byte, const UINT typeSize, const D3D12_GPU_VIRTUAL_ADDRESS gpuAddress)
	{
		vertexView.BufferLocation = gpuAddress;
		vertexView.SizeInBytes = (UINT)byte;
		vertexView.StrideInBytes = typeSize;
	}

	//インデックスビューの設定
	void Sprite::SetIndexView(const UINT indexSize, const D3D12_GPU_VIRTUAL_ADDRESS gpuAddress)
	{
		indexView.BufferLocation = gpuAddress;
		indexView.Format = DXGI_FORMAT_R16_UINT;
		indexView.SizeInBytes = indexSize;
	}

	//Regist時に呼び出し
	bool Sprite::SetUpSprite()
	{
		const DirectX::Image* image = scratch.GetImage(0, 0, 0);
		const size_t imageSize = AlignmentedSize(image->rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);

		SetVertex((float)image->width, (float)image->height);//矩形の頂点座標を設定

		if (CreateSpriteBuffer() &&//三角形の構成順序
			cbv_srvHeap.CreateCBV_SRV_UAV(2) &&//CBVとSRVを隣同士で作成
			pipeline.CreateGraphicsPipeline(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, SPRITE_TYPE, vertexShader->GetShaderBlob(), pixelShader->GetShaderBlob()) &&//パイプライン作成
			CreateBuffer(textureBuffer, metaData) && //texture作製
			CreateBuffer(uploadBuffer, imageSize * image->height) &&//upload作成
			BufferMap(uploadBuffer, *image, true) != nullptr)//データを転送
		{
			return true;
		}

		return false;
	}

	//描画時に呼び出し
	void Sprite::DrawCall()
	{
		ApplyTransform();

		//パイプラインとルートシグネチャを設定
		commandList->SetPipelineState(pipeline.GetPipelineState().Get());
		commandList->SetGraphicsRootSignature(pipeline.GetRootSignature().Get());

		//メッシュの形と頂点バッファを設定
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		commandList->IASetVertexBuffers(0, 1, &vertexView);
		commandList->IASetIndexBuffer(&indexView);

		//ビューの設定
		const D3D12_GPU_DESCRIPTOR_HANDLE handle = cbv_srvHeap.GetHeap()->GetGPUDescriptorHandleForHeapStart();
		commandList->SetDescriptorHeaps(1, cbv_srvHeap.GetHeap().GetAddressOf());
		commandList->SetGraphicsRootDescriptorTable(0, handle);

		commandList->DrawIndexedInstanced(indexView.SizeInBytes / sizeof(unsigned short), 1, 0, 0, 0);
	}

	Sprite* Sprite::CreateSprite(const char* fileName)
	{
		spriteList.push_back(Sprite());
		auto spriteItr = --spriteList.end();

		const std::wstring wstr = TransformToWideChar(fileName);

		HRESULT result = DirectX::LoadFromWICFile(//画像の読込
			wstr.c_str(), DirectX::WIC_FLAGS_NONE, &spriteItr->metaData, spriteItr->scratch
		);

		if (FAILED(result))
		{
			DebugLogOnConsole("画像の読込が失敗\n");
			spriteList.erase(spriteItr);
			return nullptr;
		}

		if (!spriteItr->SetUpObject() || !spriteItr->SetUpSprite())
		{
			DebugLogOnConsole("SetUp動作が失敗\n");
			spriteList.erase(spriteItr);
			return nullptr;
		}

		auto heap = spriteItr->cbv_srvHeap.GetHeap();
		CreateConstantView(spriteItr->constBuffer, heap);//CBV作成
		CreateShaderResourceView(spriteItr->textureBuffer, heap, spriteItr->metaData.format);//SRV作成
		spriteItr->CopyTexture();//テクスチャの転送

		//CommandActionを入れる
		if (!spriteItr->WaitCopyTexture())
		{
			spriteList.erase(spriteItr);
			return nullptr;
		}

		outputObject.push_back(&(*spriteItr));

		return &(*spriteItr);
	}
}