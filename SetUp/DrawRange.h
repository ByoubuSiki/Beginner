#pragma once
#include"..\Base\Pch.h"

namespace Beginner
{

	//描画範囲と画像範囲を管理
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
		D3D12_VIEWPORT viewPort;//レンダリング範囲
		D3D12_RECT rect;//画像の表示範囲

		void SetViewPort(const FLOAT, const FLOAT);//ビューポート設定
		void SetRect(const UINT, const UINT);//シザー矩形設定
	};
}