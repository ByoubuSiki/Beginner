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
		Begin() :rtBuffer(), depthBuffer(), rtvHeap(), init(false) {}
		~Begin() { DebugLogOnConsole("Begin�N���X���j��\n"); }

		bool Init(const HWND);//DirectX��������
		void FlameStart();//�t���[���J�n����
		bool FlameEnd();//�t���[���I������

	private:
		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> rtBuffer;//RenderTarget��Buffer
		Microsoft::WRL::ComPtr<ID3D12Resource> depthBuffer;
		DescriptorHeap rtvHeap, dsvHeap;

		bool init : 1;//�������ς݃t���O

		D3D12_CPU_DESCRIPTOR_HANDLE GetRTVAddress();//RT�̃A�h���X���擾

		void Draw();//�o�^���ꂽ�I�u�W�F�N�g��`��
		bool CommandAction();//���[�U�[������̖��߂�����
		bool DeviceCommandInits();//�N���X�̏��������܂Ƃ߂�
		bool BufferInits();//Buffer��View�̏�����
	};

}