#pragma once

#include"Resource.h"
#include"DirectCore.h"
#include"..\Sprite\Figure.h"
#include"..\Sprite\Sprite.h"
#include"..\3D\Model.h"
#include"..\Shader\Shader.h"

namespace Beginner
{

	//���[�U�[�����g��DirectX����
	class Begin
	{
	public:
		Begin() :winHandle(nullptr), rtBuffer(), depthBuffer(), prevention(), command(), graphics(), rtvHeap(), init(false) {}
		~Begin()
		{
			DebugLogOnConsole("Widely�N���X���j��\n");
		}

		bool Init(const HWND);//DirectX��������
		void FlameStart();//�t���[���J�n����
		bool FlameEnd();//�t���[���I������
		bool Regist(Base*);//�\���I�u�W�F�N�g��o�^
		bool Regist(Eye*);//���_��o�^

	private:
		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> rtBuffer;//RenderTarget��Buffer
		Microsoft::WRL::ComPtr<ID3D12Resource> depthBuffer;
		DescriptorHeap rtvHeap, dsvHeap;

		DirectCommand command;//DirectX�̖��߃I�u�W�F�N�g
		DirectGraphics graphics;//DirectX�̕`��I�u�W�F�N�g
		Prevention prevention;//�G���[�h�~�I�u�W�F�N�g

		HWND winHandle;//WindowHandle
		bool init : 1;//�������ς݃t���O

		D3D12_CPU_DESCRIPTOR_HANDLE GetRTVAddress();//RT�̃A�h���X���擾

		void Draw();//�o�^���ꂽ�I�u�W�F�N�g��`��
		bool CommandAction();//���[�U�[������̖��߂�����
		bool DeviceCommandInits();//�N���X�̏��������܂Ƃ߂�
		bool BufferInits();//Buffer��View�̏�����
	};

}