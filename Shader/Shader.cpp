#include"Shader.h"

namespace Beginner
{

	std::unordered_map<std::wstring, Shader> loadShader;

	//Shader�̍쐬
	Shader* Shader::CreateShader(const char* hlslName, const char* entryPoint, const char* version)
	{
		const std::wstring wFileName = TransformToWideChar(hlslName);//�t�@�C�����̌`���ύX

		if (SameShader(wFileName.c_str()))//Shader���ǂݍ��ݍς�
		{
			return &loadShader[wFileName];
		}

		loadShader[wFileName] = {};

		Microsoft::WRL::ComPtr<ID3DBlob> errorBlod;//�G���[�󂯎��p

		HRESULT result = D3DCompileFromFile(//�V�F�[�_�[�R���p�C��
			wFileName.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, version,
			SHADER_COMPILE_FLAGS, 0, loadShader[wFileName].shaderBlob.ReleaseAndGetAddressOf(),
			errorBlod.ReleaseAndGetAddressOf()
		);

		if (FAILED(result))
		{
			DebugLogOnConsole("Shader�̓Ǎ������s\n");
			OutPutShaderError(result, errorBlod);//�G���[���e��\��
			return nullptr;
		}

		return &loadShader[wFileName];
	}

	//Shader�f�[�^�擾
	Microsoft::WRL::ComPtr<ID3DBlob> Shader::GetShaderBlob()
	{
		return shaderBlob;
	}

	//Is Same Name Shader
	bool SameShader(const wchar_t* shaderName)
	{
		//�����̃V�F�[�_�[������
		if (loadShader.find(shaderName) != loadShader.end())
		{
			return true;
		}

		return false;
	}
}