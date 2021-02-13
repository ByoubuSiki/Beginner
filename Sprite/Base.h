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

		bool RegistObject(const HWND);//ObjectBase��Regist����
		void ApplyTransform(const HWND);//���W�ϊ�

		bool regist : 1;//Regist�t���O
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer;//���W�ϊ��p�萔�o�b�t�@
		DirectX::XMMATRIX* mapTransform;//���W�ϊ��p�}�b�v�A�h���X

	public:
		Base() :regist(false), position(0, 0, 0), rotation(0, 0, 0), scale(1, 1, 1) {}
		~Base() {}

		Vector3 position;//���S���W
		Vector3 rotation;//��]�p�x
		Vector3 scale;//�g�k�T�C�Y

		//Getter
		bool GetRegist()const;
		virtual bool RegistCall(const HWND) = 0;//Regist������
		virtual void DrawCall(const HWND) = 0;//Draw������
	};

}
