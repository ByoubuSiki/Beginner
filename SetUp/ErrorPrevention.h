#pragma once

#include"..\Base\Pch.h"

namespace Beginner
{

	//�t�F���X�ƃo���A�̋@�\������
	class Prevention
	{
	public:
		Prevention();
		~Prevention();

		bool CreateFence(const Microsoft::WRL::ComPtr<ID3D12Device>&);//�t�F���X������
		void WaitGPUCommandEnd(const Microsoft::WRL::ComPtr<ID3D12CommandQueue>&);//GPU�������I���܂ő҂�

		void RenderTargetBarrier(//�����_�[�^�[�Q�b�g�o���A
			const Microsoft::WRL::ComPtr<ID3D12Resource>&,
			const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&
		);

		void PresentBarrier(//�t���b�v�o���A
			const Microsoft::WRL::ComPtr<ID3D12Resource>&,
			const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&
		);

		void RegionBarrier(//Region���̃o���A
			const Microsoft::WRL::ComPtr<ID3D12Resource>&,
			const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&
		);

		Microsoft::WRL::ComPtr<ID3D12Fence> GetFence();

	private:
		Microsoft::WRL::ComPtr<ID3D12Fence> fence;//�t�F���X�I�u�W�F�N�g
		UINT64 fenceValue;
	};

	//�f�o�b�O�p�̃��C���[��L����
	bool EnableDebugLayer();
}