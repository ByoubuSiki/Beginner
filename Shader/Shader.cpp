#include"Shader.h"

namespace Beginner
{

	std::unordered_map<std::wstring, Shader> loadShader;

	//Shaderの作成
	Shader* Shader::CreateShader(const char* hlslName, const char* entryPoint, const char* version)
	{
		const std::wstring wFileName = TransformToWideChar(hlslName);//ファイル名の形式変更

		if (SameShader(wFileName.c_str()))//Shaderが読み込み済み
		{
			return &loadShader[wFileName];
		}

		loadShader[wFileName] = {};

		Microsoft::WRL::ComPtr<ID3DBlob> errorBlod;//エラー受け取り用

		HRESULT result = D3DCompileFromFile(//シェーダーコンパイル
			wFileName.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, version,
			SHADER_COMPILE_FLAGS, 0, loadShader[wFileName].shaderBlob.ReleaseAndGetAddressOf(),
			errorBlod.ReleaseAndGetAddressOf()
		);

		if (FAILED(result))
		{
			DebugLogOnConsole("Shaderの読込が失敗\n");
			OutPutShaderError(result, errorBlod);//エラー内容を表示
			return nullptr;
		}

		return &loadShader[wFileName];
	}

	//Shaderデータ取得
	Microsoft::WRL::ComPtr<ID3DBlob> Shader::GetShaderBlob()
	{
		return shaderBlob;
	}

	//Is Same Name Shader
	bool SameShader(const wchar_t* shaderName)
	{
		//同名のシェーダーがある
		if (loadShader.find(shaderName) != loadShader.end())
		{
			return true;
		}

		return false;
	}
}