#include"Base.h"

namespace Beginner
{

	//�萔�}�b�v
	bool Base::ConstantMap()
	{
		//�}�b�v�̃A�h���X���擾
		HRESULT result = constBuffer->Map(0, nullptr, (void**)&mapTransform);

		if (FAILED(result))
		{
			DebugLogOnConsole("�萔�}�b�v�̎��s\n");
			return false;
		}

		*mapTransform = DirectX::XMMatrixIdentity();

		return true;
	}

	//Base��Regist�����Ƃ��̓���
	bool Base::RegistObject(const HWND hwnd)
	{
		const size_t constAlign = AlignmentedSize(sizeof(*mapTransform), D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);

		//�萔�o�b�t�@���쐬
		if (CreateBuffer(hwnd, constBuffer, constAlign) && ConstantMap())
		{
			return true;
		}

		return false;
	}

	//�p�x�̓K�p
	void Base::ApplyTransform(const HWND hwnd)
	{
		//�g��s��
		*mapTransform = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

		*mapTransform *= DirectX::XMMatrixRotationRollPitchYaw(//��]�s��
			rotation.x * DirectX::XM_PI / 180.0F,
			rotation.y * DirectX::XM_PI / 180.0F,
			rotation.z * DirectX::XM_PI / 180.0F
		);

		*mapTransform *= DirectX::XMMatrixTranslation(//�ړ��s��
			2.0F * position.x / (float)GetWindowSize(hwnd).x - 1.0F,
			2.0F * position.y / (float)GetWindowSize(hwnd).y - 1.0F,
			position.z
		);

		if (mainEye->eyeType == PERSPECTIVE)//3D���[�h�̏ꍇ
		{
			*mapTransform *= mainEye->GetViewMatrix();
			*mapTransform *= mainEye->GetProjMatrix();
		}
	}

	//Regist�ϐ��̃Q�b�^�[
	bool Base::GetRegist() const
	{
		return regist;
	}
}
