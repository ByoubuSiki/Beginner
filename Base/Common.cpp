#include"Common.h"

namespace Beginner
{
	std::list<Base*> registBase;//�`��ΏۃI�u�W�F�N�g
	Eye* mainEye;//���_
	Microsoft::WRL::ComPtr<ID3D12Device> device;//�`��f�o�C�X
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;//���߃��X�g
}