#include"DrawRange.h"

namespace Beginner
{

	void DrawRange::SetViewPort(const FLOAT width, const FLOAT height)
	{
		viewPort.Width = width;
		viewPort.Height = height;
		viewPort.TopLeftX = 0;
		viewPort.TopLeftY = 0;
		viewPort.MaxDepth = 1.0F;
		viewPort.MinDepth = 0.0F;
	}

	void DrawRange::SetRect(const UINT width, const UINT height)
	{
		rect.top = 0;
		rect.left = 0;
		rect.right = rect.left + width;
		rect.bottom = rect.top + height;
	}

	D3D12_VIEWPORT& DrawRange::GetViewPort()
	{
		return viewPort;
	}

	D3D12_RECT& DrawRange::GetRect()
	{
		return rect;
	}
}