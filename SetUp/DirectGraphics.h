#pragma once

#include"..\Base\Pch.h"
#include"DescriptorHeap.h"

namespace Beginner
{

	bool CreateDevice();//�f�o�C�X�̏�����
	bool CreateDXGIFactory();//DXGI�̊Ǘ��I�u�W�F�N�g��������
	bool GetUseAdapter();//�g�p�ł���A�_�v�^�̎擾

	bool CreateSwapChain();//�X���b�v�`�F�C���̍쐬

	UINT GetBackBufferSize();//�o�b�N�o�b�t�@����Ԃ�
	//RTV�Ƃ̊֘A�t��
	bool CreateBackBuffer(std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>&, D3D12_CPU_DESCRIPTOR_HANDLE);

	bool CreateUntilAdapter();//�A�_�v�^�̎擾�܂ł̏�����

	bool CreateUntilEnd(//�c�������������s��
		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>&,
		const D3D12_CPU_DESCRIPTOR_HANDLE
	);
}