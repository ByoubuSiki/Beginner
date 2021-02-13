#include"Figure.h"

namespace Beginner
{

	std::list<Figure> figureList;

	//Vertex Order
	void Figure::VertexIndexOrder(std::vector<unsigned short>& vertexIndex, const Size winSize)
	{
		for (unsigned int i = 0; i < vertexUV.size(); i++)
		{
			vertexUV[i].vertex = FigureVertex(vertexUV[i].vertex, winSize);//�N���b�v���W�ŕ\��

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
	bool Figure::RegistFigure(const HWND hwnd)
	{
		if (CreateFigureBuffer(hwnd) &&
			cbvHeap.CreateCBV_SRV_UAV(device, 1) &&//CBV�̂ݍ쐬
			pipeline.CreateGraphicsPipeline(hwnd, drawTopology, FIGURE_TYPE, vertexShader->GetShaderBlob(), pixelShader->GetShaderBlob()))
		{
			auto a = cbvHeap.GetHeap();
			CreateConstantView(hwnd, constBuffer, a);//CBV�̒��g���쐬
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

	//���_���}�b�v
	bool Figure::CreateFigureBuffer(const HWND hwnd)
	{
		std::vector<unsigned short> index;//���_�o�b�t�@�̍\������

		VertexIndexOrder(index, GetWindowSize(hwnd));//���_�̍\������

		//���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���쐬
		if (!CreateBuffer(hwnd, vertexBuffer, sizeof(vertexUV[0]) * vertexUV.size()) ||
			!CreateBuffer(hwnd, indexBuffer, sizeof(index[0]) * index.size()))
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

	//Regist���ɌĂяo��
	bool Figure::RegistCall(const HWND hwnd)
	{
		if (RegistObject(hwnd) && RegistFigure(hwnd))
		{
			regist = true;//Regist�t���O�𗧂Ă�
			return true;
		}

		return false;
	}

	//Widely����̕`�掞�ɌĂяo��
	void Figure::DrawCall(const HWND hwnd)
	{
		ApplyTransform(hwnd);//���W�ϊ�

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

		Vector3 pivot(0, 0, 0);

		//���_���ƒ��S���W���L�^
		for (unsigned short i = 0; i < vertCount; i++)
		{
			pivot += vert[i];//���W�̍��v�l
		}

		figureItr->position = pivot / vertCount;//���S���W
		figureItr->drawTopology = GetPrimitiveTopology(vertCount);//�`��g�|���W�[���擾

		//���S�ƒ��_�̋����x�N�g������
		for (unsigned short i = 0; i < vertCount; i++)
		{
			const DirectX::XMFLOAT3 vertex = {
				vert[i].x - figureItr->position.x,
				vert[i].y - figureItr->position.y,
				vert[i].z - figureItr->position.z
			};

			figureItr->vertexUV.push_back({ vertex,{ 0.0F,0.0F } });
		}

		return &(*figureItr);//�쐬�����}�`�̃|�C���^
	}
}