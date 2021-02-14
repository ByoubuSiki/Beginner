#pragma once

#include"..\Base\Pch.h"
#include"Eye.h"

namespace Beginner
{

	//�S�I�u�W�F�N�g����
	class Base
	{
	private:
		bool ConstantMap();//�萔�̃}�b�v����

	protected:
		bool SetUpObject();//Base�̃Z�b�g�A�b�v
		void ApplyTransform();//���W�ϊ�

		Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer;//���W�ϊ��p�萔�o�b�t�@
		DirectX::XMMATRIX* mapTransform;//���W�ϊ��p�}�b�v�A�h���X

	public:
		Base() :position(0, 0, 0), rotation(0, 0, 0), scale(1, 1, 1) {}
		~Base() {}

		Vector3 position;//���S���W
		Vector3 rotation;//��]�p�x
		Vector3 scale;//�g�k�T�C�Y

		//Getter
		virtual void DrawCall() = 0;//Draw������
	};

}
