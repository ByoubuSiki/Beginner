#pragma once

#include"Base.h"
#include"..\Shader\Shader.h"
#include"..\Base\Pch.h"
#include"..\SetUp\DescriptorHeap.h"
#include"GraphicsPipeline.h"

namespace Beginner
{

	class Sprite :public Base
	{
	private:
		void CopyTexture(const HWND);//�e�N�X�`�����R�s�[
		void SetVertex(float, float);//���_����ݒ�
		bool CreateSpriteBuffer(const HWND);//���_�ƍ\�����Ԃ̃o�b�t�@���쐬
		void VertexIndexOrder(std::vector<unsigned short>&, const Size);//���_���K���ƃg�|���W����
		void SetVertexView(const size_t, const UINT, const D3D12_GPU_VIRTUAL_ADDRESS);//���_�r���[�̐ݒ�
		void SetIndexView(const UINT, const D3D12_GPU_VIRTUAL_ADDRESS);//�C���f�b�N�X�r���[�̐ݒ�

		bool RegistSprite(const HWND);//Sprite��Regist������܂Ƃ߂�

		//Buffer
		Microsoft::WRL::ComPtr<ID3D12Resource> textureBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource> uploadBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer;
		DescriptorHeap cbv_srvHeap;

		std::vector<VertexUV> vertexUV;//���_���W
		GraphicsPipeline pipeline;//�O���t�B�b�N�p�C�v���C��

		//Image
		DirectX::TexMetadata metaData;
		DirectX::ScratchImage scratch;

		//View
		D3D12_VERTEX_BUFFER_VIEW vertexView;
		D3D12_INDEX_BUFFER_VIEW indexView;

	public:
		Sprite() :Sprite::Sprite(
			Shader::CreateShader("../Shader/BasicVertex.hlsl", "main", VERTEX_SHADER_5_0),
			Shader::CreateShader("../Shader/Sprite.hlsl", "main", PIXEL_SHADER_5_0)) {}
		Sprite(Shader* vertex, Shader* pixel) :
			vertexShader(vertex), pixelShader(pixel), indexView(), vertexView(),
			textureBuffer(nullptr), uploadBuffer(nullptr), cbv_srvHeap(), metaData() {}
		~Sprite() {}

		//Shader
		Shader* pixelShader;
		Shader* vertexShader;

		bool RegistCall(const HWND)override;//Regist���ɌĂяo��
		void DrawCall(const HWND)override;//�`�掞�ɌĂяo��

		static Sprite* CreateSprite(const char*);//Create Sprite
	};

	extern std::list<Sprite> spriteList;//�摜
}