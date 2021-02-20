#pragma once

#include"..\Base\Pch.h"

namespace Beginner
{

	bool CreateFence();//�t�F���X������
	void WaitGPUCommandEnd();//GPU�������I���܂ő҂�

	void RenderTargetBarrier(const Microsoft::WRL::ComPtr<ID3D12Resource>&);//�����_�[�^�[�Q�b�g�o���A
	void PresentBarrier(const Microsoft::WRL::ComPtr<ID3D12Resource>&);//�t���b�v�o���A
	void RegionBarrier(const Microsoft::WRL::ComPtr<ID3D12Resource>&);//Region���̃o���A

	//�f�o�b�O�p�̃��C���[��L����
	bool EnableDebugLayer();
}