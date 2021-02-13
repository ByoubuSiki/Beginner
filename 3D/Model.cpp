#include"Model.h"

namespace Beginner
{
	std::list<Model> modelList;//作製したModelクラスの実態を記録

	//モデル作成
	Model* Model::CreateModel(const char* filename)
	{
		Assimp::Importer importer;

		//Assimpで読み取り
		const auto scene = importer.ReadFile(filename, MODEL_LOAD_MEDIUM);

		if (!scene)
		{
			DebugLogOnConsole("モデル読み取り失敗\n");
			return nullptr;
		}

		//実体を配列に追加
		modelList.push_back({});
		auto itr = --modelList.end();

		itr->LoadModelData(scene->mRootNode, scene);

		return &(*itr);
	}

	//Regist時に呼び出し
	bool Model::RegistCall(const HWND hwnd)
	{
		if (RegistObject(hwnd) && RegistModel(hwnd))
		{
			regist = true;
			return true;
		}

		DebugLogOnConsole("ModelのRegistが失敗\n");
		return false;
	}

	// 描画時に呼び出し
	void Model::DrawCall(const HWND hwnd)
	{
		ApplyTransform(hwnd);//座標変換

		//パイプラインとルートシグネチャを設定
		commandList->SetPipelineState(pipeline.GetPipelineState().Get());
		commandList->SetGraphicsRootSignature(pipeline.GetRootSignature().Get());

		//三角形で描画　頂点情報と頂点インデックスを設定
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		commandList->IASetVertexBuffers(0, 1, &vertexView);
		commandList->IASetIndexBuffer(&indexView);

		//CBV・SRVを設定
		const auto handle = cbvHeap.GetHeap()->GetGPUDescriptorHandleForHeapStart();
		commandList->SetDescriptorHeaps(1, cbvHeap.GetHeap().GetAddressOf());
		commandList->SetGraphicsRootDescriptorTable(0, handle);

		//描画
		commandList->DrawIndexedInstanced((UINT)vertexIndex.size(), 1, 0, 0, 0);
	}

	//再帰でAssimpのNodeを読み取り
	void Model::LoadModelData(aiNode* node, const aiScene* scene)
	{
		for (unsigned i = 0; i < node->mNumMeshes; i++)//Node内のMeshを読み取り
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

			LoadModelMesh(mesh);//Meshの情報を読み取り
		}

		for (unsigned i = 0; i < node->mNumChildren; i++) //子Nodeを探索
		{
			LoadModelData(node->mChildren[i], scene);
		}
	}

	//AssimpのMesh情報を読み取り
	void Model::LoadModelMesh(aiMesh* mesh)
	{
		const auto offset = vertex.size();//頂点インデックスの位置を調整

		for (unsigned i = 0; i < mesh->mNumVertices; i++)//頂点情報を一つずつ読み取り
		{
			LoadModelVertex(mesh, i);//頂点情報を読み取り
		}

		for (unsigned i = 0; i < mesh->mNumFaces; i++)//Mesh内の面を調べる
		{
			for (unsigned j = 0; j < mesh->mFaces[i].mNumIndices; j++)//面内の頂点番号を調べる
			{
				vertexIndex.push_back(mesh->mFaces[i].mIndices[j] + offset);//頂点番号を追加
			}
		}
	}

	//頂点の情報を読み取り
	void Model::LoadModelVertex(aiMesh* mesh, const unsigned vertexIndex)
	{
		//配列に頂点情報の入れ物を追加
		vertex.push_back(Vertex());
		auto itr = --vertex.end();

		if (mesh->HasPositions())//座標情報
		{
			itr->position = { mesh->mVertices[vertexIndex].x,mesh->mVertices[vertexIndex].y, mesh->mVertices[vertexIndex].z };
		}

		if (mesh->HasNormals())//法線情報
		{
			itr->normal = { mesh->mNormals[vertexIndex].x,mesh->mNormals[vertexIndex].y,mesh->mNormals[vertexIndex].z };
		}

		/*for (unsigned i = 0; i < mesh->HasTextureCoords(i); i++)
		{
			itr->texcoord.push_back({ mesh->mTextureCoords[i][vertexIndex].x, mesh->mTextureCoords[i][vertexIndex].y, mesh->mTextureCoords[i][vertexIndex].z });
		}

		for (unsigned i = 0; mesh->HasVertexColors(i); i++)//頂点カラー
		{
			itr->color.push_back(
				{ mesh->mColors[i]->r, mesh->mColors[i]->g, mesh->mColors[i]->b, mesh->mColors[i]->a }
			);
		}

		if (mesh->HasTangentsAndBitangents())//接戦
		{
			itr->tangent = {//texture x axis
				mesh->mTangents[vertexIndex].x,mesh->mTangents[vertexIndex].y,mesh->mTangents[vertexIndex].z
			};

			itr->bitangent = {//texture y axis
				mesh->mBitangents[vertexIndex].x,mesh->mBitangents[vertexIndex].y,mesh->mBitangents[vertexIndex].z
			};
		}
		*/
	}

	//Modelクラス固有のRegist動作
	bool Model::RegistModel(const HWND hwnd)
	{
		//Model用のバッファ作製 && CBV用ディスクリプタヒープ作製 && グラフィックパイプラインを作製
		if (CreateModelBuffer(hwnd) &&
			cbvHeap.CreateCBV_SRV_UAV(device, 1) &&
			pipeline.CreateGraphicsPipeline(hwnd, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, MODEL_TYPE, vertexShader->GetShaderBlob(), pixelShader->GetShaderBlob()))
		{
			auto heap = cbvHeap.GetHeap();
			CreateConstantView(hwnd, constBuffer, heap);//CBV用のViewを作製

			return true;
		}

		return false;
	}

	//Modelクラス用のバッファを作製・マップ処理
	bool Model::CreateModelBuffer(const HWND hwnd)
	{
		//頂点情報・頂点インデックス用のバッファを作製
		if (!CreateBuffer(hwnd, vertexBuffer, sizeof(vertex[0]) * vertex.size()) ||
			!CreateBuffer(hwnd, indexBuffer, sizeof(vertexIndex[0]) * vertexIndex.size()))
		{
			return false;
		}

		//頂点情報・頂点インデックスのバッファをマップ
		if (BufferMap(vertexBuffer, vertex.begin(), vertex.end(), true) == nullptr ||
			BufferMap(indexBuffer, vertexIndex.begin(), vertexIndex.end(), true) == nullptr)
		{
			return false;
		}

		//頂点情報・頂点インデックス用のビューを設定
		SetVertexView(vertex.size(), sizeof(vertex[0]), vertexBuffer->GetGPUVirtualAddress());
		SetIndexView(vertexIndex.size() * sizeof(vertexIndex[0]), indexBuffer->GetGPUVirtualAddress());

		return true;
	}

	//頂点ビューの設定
	void Model::SetVertexView(const size_t byte, const UINT typeSize, const D3D12_GPU_VIRTUAL_ADDRESS gpuAddress)
	{
		vertexView.BufferLocation = gpuAddress;
		vertexView.SizeInBytes = (UINT)byte * typeSize;
		vertexView.StrideInBytes = typeSize;
	}

	//インデックスビューの設定
	void Model::SetIndexView(const size_t indexSize, const D3D12_GPU_VIRTUAL_ADDRESS gpuAddress)
	{
		indexView.BufferLocation = gpuAddress;
		indexView.Format = DXGI_FORMAT_R32_UINT;
		indexView.SizeInBytes = (UINT)indexSize;
	}

}
