#include"Figure.h"

namespace Beginner
{
	std::list<Figure> figureList;//作成した図形のメモリ

	//ポリゴン構成順を決定
	void Figure::VertexIndexOrder(std::vector<unsigned short>& vertexIndex, const Size winSize)
	{
		for (unsigned int i = 0; i < vertexUV.size(); i++)
		{
			vertexUV[i].vertex = FigureVertex(vertexUV[i].vertex, winSize);//正規化された座標に変換

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
	bool Figure::SetUpFigure()
	{
		if (CreateFigureBuffer() &&
			cbvHeap.CreateCBV_SRV_UAV(1) &&//CBVのみ作成
			pipeline.CreateGraphicsPipeline(drawTopology, FIGURE_TYPE, vertexShader->GetShaderBlob(), pixelShader->GetShaderBlob()))
		{
			auto a = cbvHeap.GetHeap();
			CreateConstantView(constBuffer, a);//CBVの中身を作成
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

	//頂点と構成順番のバッファを作成
	bool Figure::CreateFigureBuffer()
	{
		std::vector<unsigned short> index;//頂点バッファの構成順番

		VertexIndexOrder(index, GetWindowSize());//ポリゴンの構成順番を決定

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

	//Widelyからの描画時に呼び出し
	void Figure::DrawCall()
	{
		ApplyTransform();//座標変換

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

		figureItr->position = VertexToPivot(vertCount, vert);//複数の頂点座標から中心座標を算出
		figureItr->drawTopology = GetPrimitiveTopology(vertCount);//描画トポロジーを決定

		//頂点と中心間の距離を記録
		for (unsigned short i = 0; i < vertCount; i++)
		{
			figureItr->vertexUV.push_back({ { vert[i].x - figureItr->position.x,vert[i].y - figureItr->position.y,vert[i].z - figureItr->position.z }, { 0.0F,0.0F } });
		}

		if (!figureItr->SetUpObject() || !figureItr->SetUpFigure())
		{
			figureList.erase(figureItr);
			return nullptr;
		}

		outputObject.push_back(&(*figureItr));

		return &(*figureItr);//作成した図形のポインタ
	}
}