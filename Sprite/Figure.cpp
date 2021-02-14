#include"Figure.h"

namespace Beginner
{
	std::list<Figure> figureList;//�쐬�����}�`�̃�����

	//�|���S���\����������
	void Figure::VertexIndexOrder(std::vector<unsigned short>& vertexIndex, const Size winSize)
	{
		for (unsigned int i = 0; i < vertexUV.size(); i++)
		{
			vertexUV[i].vertex = FigureVertex(vertexUV[i].vertex, winSize);//���K�����ꂽ���W�ɕϊ�

			//�O�p�`���\�����鏇�ɂȂ��ł䂭
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

	//Regist�̓���
	bool Figure::SetUpFigure()
	{
		if (CreateFigureBuffer() &&
			cbvHeap.CreateCBV_SRV_UAV(device, 1) &&//CBV�̂ݍ쐬
			pipeline.CreateGraphicsPipeline(drawTopology, FIGURE_TYPE, vertexShader->GetShaderBlob(), pixelShader->GetShaderBlob()))
		{
			auto a = cbvHeap.GetHeap();
			CreateConstantView(constBuffer, a);//CBV�̒��g���쐬
			return true;
		}

		return false;
	}

	//���_�r���[�̐ݒ�
	void Figure::SetVertexView(const size_t byte, const UINT typeSize, const D3D12_GPU_VIRTUAL_ADDRESS gpuAddress)
	{
		vertexView.BufferLocation = gpuAddress;
		vertexView.SizeInBytes = (UINT)byte;
		vertexView.StrideInBytes = typeSize;
	}

	//�C���f�b�N�X�r���[�̐ݒ�
	void Figure::SetIndexView(const UINT indexSize, const D3D12_GPU_VIRTUAL_ADDRESS gpuAddress)
	{
		indexView.BufferLocation = gpuAddress;
		indexView.Format = DXGI_FORMAT_R16_UINT;
		indexView.SizeInBytes = indexSize;
	}

	//���_�ƍ\�����Ԃ̃o�b�t�@���쐬
	bool Figure::CreateFigureBuffer()
	{
		std::vector<unsigned short> index;//���_�o�b�t�@�̍\������

		VertexIndexOrder(index, GetWindowSize());//�|���S���̍\�����Ԃ�����

		//���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���쐬
		if (!CreateBuffer(vertexBuffer, sizeof(vertexUV[0]) * vertexUV.size()) ||
			!CreateBuffer(indexBuffer, sizeof(index[0]) * index.size()))
		{
			return false;
		}

		//�f�[�^�̃}�b�v������
		if (BufferMap(vertexBuffer, vertexUV.begin(), vertexUV.end(), true) == nullptr ||
			BufferMap(indexBuffer, index.begin(), index.end(), true) == nullptr)
		{
			return false;
		}

		//�r���[��ݒ�
		SetVertexView(sizeof(vertexUV[0]) * vertexUV.size(), sizeof(vertexUV[0]), vertexBuffer->GetGPUVirtualAddress());
		SetIndexView((UINT)(index.size() * sizeof(index[0])), indexBuffer->GetGPUVirtualAddress());

		return true;
	}

	//Widely����̕`�掞�ɌĂяo��
	void Figure::DrawCall()
	{
		ApplyTransform();//���W�ϊ�

		//�p�C�v���C���ƃ��[�g�V�O�l�`����ݒ�
		commandList->SetPipelineState(pipeline.GetPipelineState().Get());
		commandList->SetGraphicsRootSignature(pipeline.GetRootSignature().Get());

		//���b�V���̌`�ƒ��_�o�b�t�@��ݒ�
		commandList->IASetPrimitiveTopology(drawTopology);
		commandList->IASetIndexBuffer(&indexView);
		commandList->IASetVertexBuffers(0, 1, &vertexView);

		//CBV��K�p
		const D3D12_GPU_DESCRIPTOR_HANDLE handle = cbvHeap.GetHeap()->GetGPUDescriptorHandleForHeapStart();
		commandList->SetDescriptorHeaps(1, cbvHeap.GetHeap().GetAddressOf());
		commandList->SetGraphicsRootDescriptorTable(0, handle);

		//���_��`��
		commandList->DrawIndexedInstanced(indexView.SizeInBytes / sizeof(unsigned short), 1, 0, 0, 0);
	}

	//�}�`�̍쐬
	Figure* Figure::CreateFigure(const unsigned short vertCount, Vector3 vert[])
	{
		//List��Figure�I�u�W�F�N�g��ǉ�
		figureList.push_back(Figure());
		auto figureItr = --figureList.end();

		figureItr->position = VertexToPivot(vertCount, vert);//�����̒��_���W���璆�S���W���Z�o
		figureItr->drawTopology = GetPrimitiveTopology(vertCount);//�`��g�|���W�[������

		//���_�ƒ��S�Ԃ̋������L�^
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

		return &(*figureItr);//�쐬�����}�`�̃|�C���^
	}
}