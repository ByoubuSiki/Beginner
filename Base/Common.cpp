#include"Common.h"

namespace Beginner
{
	HWND beginHwnd;//�Ώۂ̃E�B���h�E�n���h��
	std::list<Base*> outputObject;//�`��ΏۃI�u�W�F�N�g
	Eye* mainEye;//���_
	Microsoft::WRL::ComPtr<ID3D12Device> device;//�`��f�o�C�X
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;//���߃��X�g
}