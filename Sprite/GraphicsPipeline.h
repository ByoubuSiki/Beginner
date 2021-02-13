#pragma once

#include"..\Base\Pch.h"
#include"DescriptorRange.h"
#include"RootParametor.h"
#include"RootSignature.h"

namespace Beginner
{

	//�O���t�B�b�N�p�C�v���C������������
	class GraphicsPipeline
	{
	private:
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;//�p�C�v���C���{��
		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;//���[�g�V�O�l�`��
		Microsoft::WRL::ComPtr<ID3DBlob> rootBlob;//�V�O�l�`���̃f�[�^

		bool CreateBinaryCode(const OBJECT_TYPE);//�o�C�i���R�[�h�̍쐬
		bool CreateRootSignature(const HWND);//���[�g�V�O�l�`���̍쐬

		//���_�V�F�[�_�̐ݒ�
		void SetVertexShader(D3D12_GRAPHICS_PIPELINE_STATE_DESC&, const Microsoft::WRL::ComPtr<ID3DBlob>&);

		//�s�N�Z���V�V�F�[�_�̐ݒ�
		void SetPixelShader(D3D12_GRAPHICS_PIPELINE_STATE_DESC&, const Microsoft::WRL::ComPtr<ID3DBlob>&);

		void SetRasterizerState(D3D12_GRAPHICS_PIPELINE_STATE_DESC&);//���X�^���C�U�̐ݒ�
		void SetBlendState(D3D12_GRAPHICS_PIPELINE_STATE_DESC&);//�u�����h�ݒ�
		void SetInputLayout(D3D12_GRAPHICS_PIPELINE_STATE_DESC&);//���͔z�u�̐ݒ�
		void SetSample(D3D12_GRAPHICS_PIPELINE_STATE_DESC&);//�T���v���ݒ�
		void SetPipeline(D3D12_GRAPHICS_PIPELINE_STATE_DESC&, const D3D12_PRIMITIVE_TOPOLOGY);
		void SetDepthStencilStare(D3D12_GRAPHICS_PIPELINE_STATE_DESC&);//�[�x�X�e���V���ݒ�

	public:
		GraphicsPipeline() :pipelineState(nullptr), rootBlob(nullptr), rootSignature(nullptr) {}
		~GraphicsPipeline() {}

		Microsoft::WRL::ComPtr<ID3D12PipelineState> GetPipelineState();
		Microsoft::WRL::ComPtr<ID3DBlob> GetRootBlob();
		Microsoft::WRL::ComPtr<ID3D12RootSignature> GetRootSignature();

		bool CreateGraphicsPipeline(//�p�C�v���C���̍쐬
			const HWND, const D3D12_PRIMITIVE_TOPOLOGY, const OBJECT_TYPE,
			const Microsoft::WRL::ComPtr<ID3DBlob>&,
			const Microsoft::WRL::ComPtr<ID3DBlob>&
		);
	};

}