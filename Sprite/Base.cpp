#include"Base.h"

namespace Beginner
{

	//定数マップ
	bool Base::ConstantMap()
	{
		//マップのアドレスを取得
		HRESULT result = constBuffer->Map(0, nullptr, (void**)&mapTransform);

		if (FAILED(result))
		{
			DebugLogOnConsole("定数マップの失敗\n");
			return false;
		}

		*mapTransform = DirectX::XMMatrixIdentity();

		return true;
	}

	//Baseのセットアップ
	bool Base::SetUpObject()
	{
		const size_t constAlign = AlignmentedSize(sizeof(*mapTransform), D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);

		//定数バッファを作成
		if (CreateBuffer(constBuffer, constAlign) && ConstantMap())
		{
			return true;
		}

		return false;
	}

	//角度の適用
	void Base::ApplyTransform()
	{
		//拡大行列
		*mapTransform = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

		*mapTransform *= DirectX::XMMatrixRotationRollPitchYaw(//回転行列
			rotation.x * DirectX::XM_PI / 180.0F,
			rotation.y * DirectX::XM_PI / 180.0F,
			rotation.z * DirectX::XM_PI / 180.0F
		);

		*mapTransform *= DirectX::XMMatrixTranslation(//移動行列
			2.0F * position.x / (float)GetWindowSize().x - 1.0F,
			2.0F * position.y / (float)GetWindowSize().y - 1.0F,
			position.z
		);

		if (mainEye->eyeType == PERSPECTIVE)//3Dモードの場合
		{
			*mapTransform *= mainEye->GetViewMatrix();
			*mapTransform *= mainEye->GetProjMatrix();
		}
	}
}
