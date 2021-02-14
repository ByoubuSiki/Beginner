#pragma once

#define NOMINMAX

#include<Windows.h>
#include<unordered_map>
#include<d3d12.h>
#include<wrl.h>

#include"struct.h"

namespace Beginner
{
	//���ʕϐ�

	class Eye;
	class Base;

	extern HWND beginHwnd;
	extern std::list<Base*> outputObject;//�`��ΏۃI�u�W�F�N�g
	extern Eye* mainEye;//���_
	extern Microsoft::WRL::ComPtr<ID3D12Device> device;//�`��f�o�C�X
	extern Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;//���߃��X�g
}