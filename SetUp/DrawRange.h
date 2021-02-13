#pragma once
#include"..\Base\Pch.h"

namespace Beginner
{

	//�`��͈͂Ɖ摜�͈͂��Ǘ�
	class DrawRange
	{
	public:
		DrawRange() :viewPort(), rect() {}
		DrawRange(const UINT width, const UINT height) :viewPort(), rect()
		{
			SetViewPort((FLOAT)width, (FLOAT)height);
			SetRect(width, height);
		}
		DrawRange(const Size size)
		{
			SetViewPort(size.x, size.y);
			SetRect((LONG)size.x, (LONG)size.y);
		}

		~DrawRange() {}

		D3D12_VIEWPORT& GetViewPort();
		D3D12_RECT& GetRect();

	private:
		D3D12_VIEWPORT viewPort;//�����_�����O�͈�
		D3D12_RECT rect;//�摜�̕\���͈�

		void SetViewPort(const FLOAT, const FLOAT);//�r���[�|�[�g�ݒ�
		void SetRect(const UINT, const UINT);//�V�U�[��`�ݒ�
	};
}