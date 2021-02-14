#include"Sprite.h"

namespace Beginner
{
	std::list<Sprite> spriteList;//�摜�̃�����

	//�e�N�X�`�����R�s�[
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

		//�摜�f�[�^�]��
		commandList->CopyTextureRegion(&copyLocationTexture, 0, 0, 0, &copyLocationUpload, nullptr);

		//�o���A
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

	//���_���W������
	void Sprite::SetVertex(float imageWidth, float imageHeight)
	{
		imageWidth /= 2.0F;
		imageHeight /= 2.0F;

		//����̒��_����E���ɒ��_���� ���S�����_�ɂ��킹��
		vertexUV.push_back({ { -imageWidth,imageHeight,0.0F }, { 0.0F,0.0F } });
		vertexUV.push_back({ { imageWidth,imageHeight,0.0F }, { 1.0F,0.0F } });
		vertexUV.push_back({ { imageWidth,-imageHeight,0.0F }, { 1.0F,1.0F } });
		vertexUV.push_back({ { -imageWidth,-imageHeight,0.0F }, { 0.0F,1.0F } });
	}

	//���_���}�b�v
	bool Sprite::CreateSpriteBuffer()
	{
		std::vector<unsigned short> index;//���_�o�b�t�@�̍\������

		VertexIndexOrder(index, GetWindowSize());//���_�̍\������

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

	//���_�̐ڑ����Ԃ��\��
	void Sprite::VertexIndexOrder(std::vector<unsigned short>& index, const Size winSize)
	{
		for (unsigned int i = 0; i < vertexUV.size(); i++)
		{
			vertexUV[i].vertex = FigureVertex(vertexUV[i].vertex, winSize);//�N���b�v���W�ŕ\��

			//�O�p�`���\�����鏇�ɂȂ��ł䂭
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

	//���_�r���[�̐ݒ�
	void Sprite::SetVertexView(const size_t byte, const UINT typeSize, const D3D12_GPU_VIRTUAL_ADDRESS gpuAddress)
	{
		vertexView.BufferLocation = gpuAddress;
		vertexView.SizeInBytes = (UINT)byte;
		vertexView.StrideInBytes = typeSize;
	}

	//�C���f�b�N�X�r���[�̐ݒ�
	void Sprite::SetIndexView(const UINT indexSize, const D3D12_GPU_VIRTUAL_ADDRESS gpuAddress)
	{
		indexView.BufferLocation = gpuAddress;
		indexView.Format = DXGI_FORMAT_R16_UINT;
		indexView.SizeInBytes = indexSize;
	}

	//Regist���ɌĂяo��
	bool Sprite::SetUpSprite()
	{
		const DirectX::Image* image = scratch.GetImage(0, 0, 0);
		const size_t imageSize = AlignmentedSize(image->rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);

		SetVertex((float)image->width, (float)image->height);//��`�̒��_���W��ݒ�

		if (CreateSpriteBuffer() &&//�O�p�`�̍\������
			cbv_srvHeap.CreateCBV_SRV_UAV(device, 2) &&//CBV��SRV��ד��m�ō쐬
			pipeline.CreateGraphicsPipeline(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, SPRITE_TYPE, vertexShader->GetShaderBlob(), pixelShader->GetShaderBlob()) &&//�p�C�v���C���쐬
			CreateBuffer(textureBuffer, metaData) && //texture�쐻
			CreateBuffer(uploadBuffer, imageSize * image->height) &&//upload�쐬
			BufferMap(uploadBuffer, *image, true) != nullptr)//�f�[�^��]��
		{
			return true;
		}

		return false;
	}

	//�`�掞�ɌĂяo��
	void Sprite::DrawCall()
	{
		ApplyTransform();

		//�p�C�v���C���ƃ��[�g�V�O�l�`����ݒ�
		commandList->SetPipelineState(pipeline.GetPipelineState().Get());
		commandList->SetGraphicsRootSignature(pipeline.GetRootSignature().Get());

		//���b�V���̌`�ƒ��_�o�b�t�@��ݒ�
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		commandList->IASetVertexBuffers(0, 1, &vertexView);
		commandList->IASetIndexBuffer(&indexView);

		//�r���[�̐ݒ�
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

		HRESULT result = DirectX::LoadFromWICFile(//�摜�̓Ǎ�
			wstr.c_str(), DirectX::WIC_FLAGS_NONE, &spriteItr->metaData, spriteItr->scratch
		);

		if (FAILED(result))
		{
			DebugLogOnConsole("�摜�̓Ǎ������s\n");
			spriteList.erase(spriteItr);
			return nullptr;
		}

		if (!spriteItr->SetUpObject() || !spriteItr->SetUpSprite())
		{
			DebugLogOnConsole("SetUp���삪���s\n");
			spriteList.erase(spriteItr);
			return nullptr;
		}

		outputObject.push_back(&(*spriteItr));

		return &(*spriteItr);
	}
}