#include"Model.h"

namespace Beginner
{
	std::list<Model> modelList;//�쐻����Model�N���X�̎��Ԃ��L�^

	//���f���쐬
	Model* Model::CreateModel(const char* filename)
	{
		Assimp::Importer importer;

		//Assimp�œǂݎ��
		const auto scene = importer.ReadFile(filename, MODEL_LOAD_MEDIUM);

		if (!scene)
		{
			DebugLogOnConsole("���f���ǂݎ�莸�s\n");
			return nullptr;
		}

		//���̂�z��ɒǉ�
		modelList.push_back({});
		auto itr = --modelList.end();

		itr->LoadModelData(scene->mRootNode, scene);

		if (!itr->SetUpObject() || !itr->SetUpModel())
		{
			DebugLogOnConsole("Model�N���X��SetUp�����s\n");
			modelList.erase(itr);
			return nullptr;
		}

		outputObject.push_back(&(*itr));

		return &(*itr);
	}

	// �`�掞�ɌĂяo��
	void Model::DrawCall()
	{
		ApplyTransform();//���W�ϊ�

		//�p�C�v���C���ƃ��[�g�V�O�l�`����ݒ�
		commandList->SetPipelineState(pipeline.GetPipelineState().Get());
		commandList->SetGraphicsRootSignature(pipeline.GetRootSignature().Get());

		//�O�p�`�ŕ`��@���_���ƒ��_�C���f�b�N�X��ݒ�
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		commandList->IASetVertexBuffers(0, 1, &vertexView);
		commandList->IASetIndexBuffer(&indexView);

		//CBV�ESRV��ݒ�
		const auto handle = cbvHeap.GetHeap()->GetGPUDescriptorHandleForHeapStart();
		commandList->SetDescriptorHeaps(1, cbvHeap.GetHeap().GetAddressOf());
		commandList->SetGraphicsRootDescriptorTable(0, handle);

		//�`��
		commandList->DrawIndexedInstanced((UINT)vertexIndex.size(), 1, 0, 0, 0);
	}

	//�ċA��Assimp��Node��ǂݎ��
	void Model::LoadModelData(aiNode* node, const aiScene* scene)
	{
		for (unsigned i = 0; i < node->mNumMeshes; i++)//Node����Mesh��ǂݎ��
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

			LoadModelMesh(mesh);//Mesh�̏���ǂݎ��
		}

		for (unsigned i = 0; i < node->mNumChildren; i++) //�qNode��T��
		{
			LoadModelData(node->mChildren[i], scene);
		}
	}

	//Assimp��Mesh����ǂݎ��
	void Model::LoadModelMesh(aiMesh* mesh)
	{
		const auto offset = vertex.size();//���_�C���f�b�N�X�̈ʒu�𒲐�

		for (unsigned i = 0; i < mesh->mNumVertices; i++)//���_��������ǂݎ��
		{
			LoadModelVertex(mesh, i);//���_����ǂݎ��
		}

		for (unsigned i = 0; i < mesh->mNumFaces; i++)//Mesh���̖ʂ𒲂ׂ�
		{
			for (unsigned j = 0; j < mesh->mFaces[i].mNumIndices; j++)//�ʓ��̒��_�ԍ��𒲂ׂ�
			{
				vertexIndex.push_back(mesh->mFaces[i].mIndices[j] + offset);//���_�ԍ���ǉ�
			}
		}
	}

	//���_�̏���ǂݎ��
	void Model::LoadModelVertex(aiMesh* mesh, const unsigned vertexIndex)
	{
		//�z��ɒ��_���̓��ꕨ��ǉ�
		vertex.push_back(Vertex());
		auto itr = --vertex.end();

		if (mesh->HasPositions())//���W���
		{
			itr->position = { mesh->mVertices[vertexIndex].x,mesh->mVertices[vertexIndex].y, mesh->mVertices[vertexIndex].z };
		}

		if (mesh->HasNormals())//�@�����
		{
			itr->normal = { mesh->mNormals[vertexIndex].x,mesh->mNormals[vertexIndex].y,mesh->mNormals[vertexIndex].z };
		}

		/*for (unsigned i = 0; i < mesh->HasTextureCoords(i); i++)
		{
			itr->texcoord.push_back({ mesh->mTextureCoords[i][vertexIndex].x, mesh->mTextureCoords[i][vertexIndex].y, mesh->mTextureCoords[i][vertexIndex].z });
		}

		for (unsigned i = 0; mesh->HasVertexColors(i); i++)//���_�J���[
		{
			itr->color.push_back(
				{ mesh->mColors[i]->r, mesh->mColors[i]->g, mesh->mColors[i]->b, mesh->mColors[i]->a }
			);
		}

		if (mesh->HasTangentsAndBitangents())//�ڐ�
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

	//Model�N���X��SetUp����
	bool Model::SetUpModel()
	{
		//Model�p�̃o�b�t�@�쐻 && CBV�p�f�B�X�N���v�^�q�[�v�쐻 && �O���t�B�b�N�p�C�v���C�����쐻
		if (CreateModelBuffer() &&
			cbvHeap.CreateCBV_SRV_UAV(device, 1) &&
			pipeline.CreateGraphicsPipeline(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, MODEL_TYPE, vertexShader->GetShaderBlob(), pixelShader->GetShaderBlob()))
		{
			auto heap = cbvHeap.GetHeap();
			CreateConstantView(constBuffer, heap);//CBV�p��View���쐻

			return true;
		}

		return false;
	}

	//Model�N���X�p�̃o�b�t�@���쐻�E�}�b�v����
	bool Model::CreateModelBuffer()
	{
		//���_���E���_�C���f�b�N�X�p�̃o�b�t�@���쐻
		if (!CreateBuffer(vertexBuffer, sizeof(vertex[0]) * vertex.size()) ||
			!CreateBuffer(indexBuffer, sizeof(vertexIndex[0]) * vertexIndex.size()))
		{
			return false;
		}

		//���_���E���_�C���f�b�N�X�̃o�b�t�@���}�b�v
		if (BufferMap(vertexBuffer, vertex.begin(), vertex.end(), true) == nullptr ||
			BufferMap(indexBuffer, vertexIndex.begin(), vertexIndex.end(), true) == nullptr)
		{
			return false;
		}

		//���_���E���_�C���f�b�N�X�p�̃r���[��ݒ�
		SetVertexView(vertex.size(), sizeof(vertex[0]), vertexBuffer->GetGPUVirtualAddress());
		SetIndexView(vertexIndex.size() * sizeof(vertexIndex[0]), indexBuffer->GetGPUVirtualAddress());

		return true;
	}

	//���_�r���[�̐ݒ�
	void Model::SetVertexView(const size_t byte, const UINT typeSize, const D3D12_GPU_VIRTUAL_ADDRESS gpuAddress)
	{
		vertexView.BufferLocation = gpuAddress;
		vertexView.SizeInBytes = (UINT)byte * typeSize;
		vertexView.StrideInBytes = typeSize;
	}

	//�C���f�b�N�X�r���[�̐ݒ�
	void Model::SetIndexView(const size_t indexSize, const D3D12_GPU_VIRTUAL_ADDRESS gpuAddress)
	{
		indexView.BufferLocation = gpuAddress;
		indexView.Format = DXGI_FORMAT_R32_UINT;
		indexView.SizeInBytes = (UINT)indexSize;
	}

}
