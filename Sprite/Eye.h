#pragma once

#include"..\Base\Pch.h"
#include"Base.h"

namespace Beginner
{
	//���_
	class Eye
	{
	public:
		Eye();
		~Eye();

		Vector3 position;//���S���W
		Vector3 rotation;//��]�p�x
		Vector3 scale;//�g�k�T�C�Y

		EYE_TYPE eyeType;//�`�惂�[�h
		float fov;//��p
		float nearZ, farZ;//���E�̉��s

		void UpdateViewMatrix();//�r���[�s��̍X�V
		DirectX::XMMATRIX& GetViewMatrix() { return viewMatrix; }
		DirectX::XMMATRIX& GetProjMatrix() { return projctionMatrix; }

		bool GetRegist()const { return regist; }
		static Eye* CreateEye();//Eye�̍쐬

	private:
		DirectX::XMMATRIX viewMatrix;//�r���[�s��
		DirectX::XMMATRIX projctionMatrix;//�v���W�F�N�V�����s��7

		bool regist : 1;
	};

	extern std::list<Eye> eyeList;//���_
}