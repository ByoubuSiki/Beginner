#pragma once

#include"..\Base\Pch.h"

namespace Beginner
{
	//Shader�Ǘ��N���X
	class Shader
	{
	private:
		Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob;//�R���p�C���ς݃V�F�[�_

	public:
		Shader() {}
		~Shader() {}

		Microsoft::WRL::ComPtr<ID3DBlob> GetShaderBlob();//�R���p�C���ς݃V�F�[�_���擾
		static Shader* CreateShader(const char*, const char*, const char*);//�V�F�[�_�̍쐬
	};

	bool SameShader(const wchar_t*);

	extern std::unordered_map<std::wstring, Shader> loadShader;//�ǂݍ��ݍς�Shader
}

