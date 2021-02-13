#pragma once

#include"..\Base\Pch.h"
#include"Base.h"
#include"..\Shader\Shader.h"
#include"GraphicsPipeline.h"
#include"..\Setup\DescriptorHeap.h"

namespace Beginner
{

	//�}�`�`��
	class Figure : public Base
	{
	private:
		void VertexIndexOrder(std::vector<unsigned short>&, const Size);//Vertex Order
		bool RegistFigure(const HWND);//Regist���̓���
		void SetVertexView(const size_t, const UINT, const D3D12_GPU_VIRTUAL_ADDRESS);//���_�r���[�̐ݒ�
		void SetIndexView(const UINT, const D3D12_GPU_VIRTUAL_ADDRESS);//�C���f�b�N�X�r���[�̐ݒ�
		bool CreateFigureBuffer(const HWND);//���_�ƍ\�����Ԃ̃o�b�t�@���쐬

		D3D12_PRIMITIVE_TOPOLOGY drawTopology;//�`��
		GraphicsPipeline pipeline;//�p�C�v���C��

		//View
		D3D12_VERTEX_BUFFER_VIEW vertexView;
		D3D12_INDEX_BUFFER_VIEW indexView;

		std::vector<VertexUV> vertexUV;//���_���W

		//Buffer
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer;
		DescriptorHeap cbvHeap;

	public:
		Figure() :Figure::Figure(
			Shader::CreateShader("../Shader/BasicVertex.hlsl", "main", VERTEX_SHADER_5_0),
			Shader::CreateShader("../Shader/Figure.hlsl", "main", PIXEL_SHADER_5_0)) {}
		Figure(Shader* vertex, Shader* pixel)
			:vertexUV(), pipeline(), cbvHeap(), vertexView(), indexView(), vertexBuffer(nullptr),
			indexBuffer(nullptr), drawTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
		{
			vertexShader = vertex; pixelShader = pixel;
		}
		~Figure() {}

		bool RegistCall(const HWND)override;//Regist���ɌĂяo��
		void DrawCall(const HWND)override;//�`�掞�ɌĂяo��

		static Figure* CreateFigure(const unsigned short, Vector3[]);//�}�`�̍쐬

		//Shader
		Shader* pixelShader;
		Shader* vertexShader;
	};

	extern std::list<Figure> figureList;//�}�`
}