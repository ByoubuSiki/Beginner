#include"Eye.h"

namespace Beginner
{
	std::list<Eye> eyeList;

	Eye::Eye() :
		viewMatrix(DirectX::XMMatrixIdentity()),
		projctionMatrix(DirectX::XMMatrixIdentity()),
		fov(90.0F), nearZ(1.0F), farZ(10.0F), eyeType(ORTHOGRAPHIC)
	{

	}

	Eye::~Eye()
	{}

	//���_�̍s��̍X�V
	void Eye::UpdateViewMatrix(const HWND hwnd)
	{
		const Vector3 radAngle = {
			rotation.x * DirectX::XM_PI / 180.0F,
			rotation.y * DirectX::XM_PI / 180.0F,
			rotation.z * DirectX::XM_PI / 180.0F
		};

		const Size size = GetWindowSize(hwnd);

		const Vector3 clipPosition =
		{
			2.0F * position.x / size.x,2.0F * position.y / size.y,position.z
		};

		viewMatrix = DirectX::XMMatrixLookAtLH(
			DirectX::XMVectorSet(clipPosition.x, clipPosition.y, clipPosition.z, 0.0F),
			DirectX::XMVectorSet(
				clipPosition.x + std::sin(radAngle.y) * std::cos(radAngle.z),
				clipPosition.y - std::sin(radAngle.x) * std::cos(radAngle.z),
				clipPosition.z + std::cos(radAngle.x) * std::cos(radAngle.y),
				0.0F),
			DirectX::XMVectorSet(0.0F, 1.0F, 0.0F, 0.0F)
		);


		if (eyeType == PERSPECTIVE)
		{
			projctionMatrix = DirectX::XMMatrixPerspectiveFovLH(
				fov * DirectX::XM_PI / 180.0F, (float)size.x / (float)size.y, nearZ, farZ
			);
		}
	}

	//Eye�̍쐬
	Eye* Eye::CreateEye()
	{
		eyeList.push_back(Eye());
		auto pushItr = --eyeList.end();

		return &(*pushItr);
	}
}