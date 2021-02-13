#pragma once

#include"..\Base\Pch.h"

namespace Beginner
{
	//Shader管理クラス
	class Shader
	{
	private:
		Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob;//コンパイル済みシェーダ

	public:
		Shader() {}
		~Shader() {}

		Microsoft::WRL::ComPtr<ID3DBlob> GetShaderBlob();//コンパイル済みシェーダを取得
		static Shader* CreateShader(const char*, const char*, const char*);//シェーダの作成
	};

	bool SameShader(const wchar_t*);

	extern std::unordered_map<std::wstring, Shader> loadShader;//読み込み済みShader
}

