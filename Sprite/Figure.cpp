#include"Figure.h"

namespace Beginner
{

	std::list<Figure> figureList;

	//Vertex Order
	void Figure::VertexIndexOrder(std::vector<unsigned short>& vertexIndex, const Size winSize)
	{
		for (unsigned int i = 0; i < vertexUV.size(); i++)
		{
			vertexUV[i].vertex = FigureVertex(vertexUV[i].vertex, winSize);//クリップ座標で表現

			//三角形を構成する順につないでゆく
			if (i < 3)
			{
				vertexIndex.push_back(i);
			}
			else if (i >= 3)
			{
				vertexIndex.push_back(vertexIndex[0]);
				vertexIndex.push_back(vertexIndex[vertexIndex.size() - 2]);
				vertexIndex.push_back(i);
			}
		}
	}

	//Registの動作
	bool Figure::RegistFigure(const HWND hwnd)
	{
		if (CreateFigureBuffer(hwnd) &&
			cbvHeap.CreateCBV_SRV_UAV(device, 1) &&//CBVのみ作成
			pipeline.CreateGraphicsPipeline(hwnd, drawTopology, FIGURE_TYPE, vertexShader->GetShaderBlob(), pixelShader->GetShaderBlob()))
		{
			auto a = cbvHeap.GetHeap();
			CreateConstantView(hwnd, constBuffer, a);//CBVの中身を作成
			return true;
		}

		return false;
	}

	//頂点ビューの設定
	void Figure::SetVertexView(const size_t byte, const UINT typeSize, const D3D12_GPU_VIRTUAL_ADDRESS gpuAddress)
	{
		vertexView.BufferLocation = gpuAddress;
		vertexView.SizeInBytes = (UINT)byte;
		vertexView.StrideInBytes = typeSize;
	}

	//インデックスビューの設定
	void Figure::SetIndexView(const UINT indexSize, const D3D12_GPU_VIRTUAL_ADDRESS gpuAddress)
	{
		indexView.BufferLocation = gpuAddress;
		indexView.Format = DXGI_FORMAT_R16_UINT;
		indexView.SizeInBytes = indexSize;
	}

	//頂点をマップ
	bool Figure::CreateFigureBuffer(const HWND hwnd)
	{
		std::vector<unsigned short> index;//頂点バッファの構成順番

		VertexIndexOrder(index, GetWindowSize(hwnd));//頂点の構成順番

		//頂点バッファとインデックスバッファを作成
		if (!CreateBuffer(hwnd, vertexBuffer, sizeof(vertexUV[0]) * vertexUV.size()) ||
			!CreateBuffer(hwnd, indexBuffer, sizeof(index[0]) * index.size()))
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

	//Regist時に呼び出し
	bool Figure::RegistCall(const HWND hwnd)
	{
		if (RegistObject(hwnd) && RegistFigure(hwnd))
		{
			regist = true;//Registフラグを立てる
			return true;
		}

		return false;
	}

	//Widelyからの描画時に呼び出し
	void Figure::DrawCall(const HWND hwnd)
	{
		ApplyTransform(hwnd);//座標変換

		//パイプラインとルートシグネチャを設定
		commandList->SetPipelineState(pipeline.GetPipelineState().Get());
		commandList->SetGraphicsRootSignature(pipeline.GetRootSignature().Get());

		//メッシュの形と頂点バッファを設定
		commandList->IASetPrimitiveTopology(drawTopology);
		commandList->IASetIndexBuffer(&indexView);
		commandList->IASetVertexBuffers(0, 1, &vertexView);

		//CBVを適用
		const D3D12_GPU_DESCRIPTOR_HANDLE handle = cbvHeap.GetHeap()->GetGPUDescriptorHandleForHeapStart();
		commandList->SetDescriptorHeaps(1, cbvHeap.GetHeap().GetAddressOf());
		commandList->SetGraphicsRootDescriptorTable(0, handle);

		//頂点を描画
		commandList->DrawIndexedInstanced(indexView.SizeInBytes / sizeof(unsigned short), 1, 0, 0, 0);
	}

	//図形の作成
	Figure* Figure::CreateFigure(const unsigned short vertCount, Vector3 vert[])
	{
		//ListにFigureオブジェクトを追加
		figureList.push_back(Figure());
		auto figureItr = --figureList.end();

		Vector3 pivot(0, 0, 0);

		//頂点情報と中心座標を記録
		for (unsigned short i = 0; i < vertCount; i++)
		{
			pivot += vert[i];//座標の合計値
		}

		figureItr->position = pivot / vertCount;//中心座標
		figureItr->drawTopology = GetPrimitiveTopology(vertCount);//描画トポロジーを取得

		//中心と頂点の距離ベクトルを代入
		for (unsigned short i = 0; i < vertCount; i++)
		{
			const DirectX::XMFLOAT3 vertex = {
				vert[i].x - figureItr->position.x,
				vert[i].y - figureItr->position.y,
				vert[i].z - figureItr->position.z
			};

			figureItr->vertexUV.push_back({ vertex,{ 0.0F,0.0F } });
		}

		return &(*figureItr);//作成した図形のポインタ
	}
}