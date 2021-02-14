#pragma once

#include"..\Base\Pch.h"
#include"..\Sprite\Base.h"
#include"..\Sprite\GraphicsPipeline.h"
#include"..\Shader\Shader.h"
#include"..\SetUp\DescriptorHeap.h"

namespace Beginner
{
	//3D���f���I�u�W�F�N�g
	class Model :public Base
	{
	public:
		Model() :Model(
			Shader::CreateShader("../Shader/BasicVertex.hlsl", "main", VERTEX_SHADER_5_0),
			Shader::CreateShader("../Shader/Model.hlsl", "main", PIXEL_SHADER_5_0)) {}
		Model(Shader* vertex, Shader* pixel) :vertexShader(vertex), pixelShader(pixel), indexView(), vertexView() {}
		~Model() {}

		static Model* CreateModel(const char*);//Model�N���X�̎��Ԃ��쐻
		void DrawCall()override;//�`�掞�ɌĂяo��

		//Shader�ϐ�
		Shader* pixelShader;
		Shader* vertexShader;

	private:

		void LoadModelData(aiNode*, const aiScene*);//�ċA��Assimp��Node��ǂݎ��
		void LoadModelMesh(aiMesh*);//Assimp��Mesh����ǂݎ��
		void LoadModelVertex(aiMesh*, const unsigned);//���_�̏���ǂݎ��
		bool SetUpModel();//Model�N���X��SetUp����
		bool CreateModelBuffer();//Model�N���X�p�̃o�b�t�@���쐻�E�}�b�v����
		void SetVertexView(const size_t, const UINT, const D3D12_GPU_VIRTUAL_ADDRESS);//���_�r���[�̐ݒ�
		void SetIndexView(const size_t, const D3D12_GPU_VIRTUAL_ADDRESS);//�C���f�b�N�X�r���[�̐ݒ�

		std::vector<Vertex> vertex;//�\�����钸�_���
		std::vector<unsigned> vertexIndex;//�|���S�����\�����钸�_�̔z��ԍ�

		GraphicsPipeline pipeline;//�`��ݒ�

		Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;//���_�p�o�b�t�@�̃|�C���^
		Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer;//���_�C���f�b�N�X�̃|�C���^

		DescriptorHeap cbvHeap;//CBV�p�̃f�B�X�N���v�^�q�[�v

		D3D12_VERTEX_BUFFER_VIEW vertexView;//���_���̃r���[
		D3D12_INDEX_BUFFER_VIEW indexView;//���_�C���f�b�N�X���̃r���[
	};

	extern std::list<Model> modelList;//�쐻����Model�N���X�̎��Ԃ��L�^
}