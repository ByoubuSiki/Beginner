#include"GraphicsPipeline.h"

namespace Beginner
{

	//�o�C�i���R�[�h�̍쐬
	bool GraphicsPipeline::CreateBinaryCode(const OBJECT_TYPE objectType)
	{
		Microsoft::WRL::ComPtr<ID3DBlob> error;//�G���[�󂯎��

		DescriptorRange descRange;//DescriptorRange�̊Ǘ�

		switch (objectType)	//�쐬�I�u�W�F�N�g�ʂ�DescriptorRange�ݒ�
		{
		case FIGURE_TYPE:
			descRange.PushBack(1, D3D12_DESCRIPTOR_RANGE_TYPE_CBV);//1�߂͒萔�o�b�t�@
			break;

		case SPRITE_TYPE:
			descRange.PushBack(1, D3D12_DESCRIPTOR_RANGE_TYPE_CBV);//1�߂͒萔�o�b�t�@
			descRange.PushBack(1, D3D12_DESCRIPTOR_RANGE_TYPE_SRV);//2�߂�ShadeResource
			break;

		case MODEL_TYPE:
			descRange.PushBack(1, D3D12_DESCRIPTOR_RANGE_TYPE_CBV);//1�߂͒萔�o�b�t�@
			//descRange.PushBack(1, D3D12_DESCRIPTOR_RANGE_TYPE_SRV);//2�߂�ShadeResource
			break;

		default: DebugLogOnConsole("�K��O�̃I�u�W�F�N�g\n");
			return false;
		}

		//�p�����[�^�쐬
		RootParametor rootPrametor;
		rootPrametor.PushBack(descRange.GetDescRange(0), (UINT)descRange.GetSize());

		D3D12_STATIC_SAMPLER_DESC sampler = {};
		{
			sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
			sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;//��ԂȂ�
			sampler.MaxLOD = D3D12_FLOAT32_MAX;
			sampler.MinLOD = 0.0F;
			sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		}

		RootSignature rootSignature;//���[�g�V�O�l�`���̐ݒ�

		rootSignature.SetParametor((UINT)rootPrametor.GetSize(), rootPrametor.GetRootParameter(0));
		rootSignature.SetStaticSamper(1, &sampler);

		HRESULT result = D3D12SerializeRootSignature(
			rootSignature.GetRootSignature(), D3D_ROOT_SIGNATURE_VERSION_1_0,
			rootBlob.ReleaseAndGetAddressOf(), error.ReleaseAndGetAddressOf()
		);

		if (FAILED(result))
		{
			DebugLogOnConsole("�o�C�i���R�[�h�̍쐬�����s\n");
			OutPutRootSignatureError(error);
			return false;
		}

		return true;
	}

	//���[�g�V�O�l�`���̍쐬
	bool GraphicsPipeline::CreateRootSignature(const HWND hwnd)
	{
		HRESULT result = device->CreateRootSignature(
			0, rootBlob->GetBufferPointer(), rootBlob->GetBufferSize(),
			IID_PPV_ARGS(rootSignature.ReleaseAndGetAddressOf())
		);

		if (FAILED(result))
		{
			DebugLogOnConsole("���[�g�V�O�l�`���̍쐬�����s\n");
			return false;
		}

		return true;
	}

	//�p�C�v���C���̍쐬
	bool GraphicsPipeline::CreateGraphicsPipeline(
		const HWND hwnd, const D3D12_PRIMITIVE_TOPOLOGY topology, const OBJECT_TYPE objectType,
		const Microsoft::WRL::ComPtr<ID3DBlob>& vsShader,
		const Microsoft::WRL::ComPtr<ID3DBlob>& psShader
	)
	{
		//���[�g�V�O�l�`���̍쐬
		if (!CreateBinaryCode(objectType) || !CreateRootSignature(hwnd))
		{
			return false;
		}

		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc = {};//�p�C�v���C���ݒ�

		//�e���ڂ�ݒ�
		{
			SetVertexShader(pipelineDesc, vsShader);
			SetPixelShader(pipelineDesc, psShader);
			SetRasterizerState(pipelineDesc);
			SetBlendState(pipelineDesc);
			SetDepthStencilStare(pipelineDesc);
			SetInputLayout(pipelineDesc);
			SetSample(pipelineDesc);
			SetPipeline(pipelineDesc, topology);
		}

		//�p�C�v���C���X�e�[�g���쐬
		HRESULT result = device->CreateGraphicsPipelineState(
			&pipelineDesc, IID_PPV_ARGS(pipelineState.ReleaseAndGetAddressOf())
		);

		if (FAILED(result))
		{
			DebugLogOnConsole("�p�C�v���C���X�e�[�g�̍쐬�����s\n");
			return false;
		}

		return true;
	}

	//���_�V�F�[�_�̐ݒ�
	void GraphicsPipeline::SetVertexShader(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc, const Microsoft::WRL::ComPtr<ID3DBlob>& shader)
	{
		pipelineDesc.VS.BytecodeLength = shader->GetBufferSize();
		pipelineDesc.VS.pShaderBytecode = shader->GetBufferPointer();
	}

	//�s�N�Z���V�F�[�_�̐ݒ�
	void GraphicsPipeline::SetPixelShader(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc, const Microsoft::WRL::ComPtr<ID3DBlob>& shader)
	{
		pipelineDesc.PS.BytecodeLength = shader->GetBufferSize();
		pipelineDesc.PS.pShaderBytecode = shader->GetBufferPointer();
	}

	//���X�^���C�U�̐ݒ�
	void GraphicsPipeline::SetRasterizerState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc)
	{
		pipelineDesc.RasterizerState.MultisampleEnable = false;//�}���`�T���v���A���`�G�C���A�V���O(MSAA)
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//�J�����O�Ȃ�
		pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//�|���S���h��Ԃ�
		pipelineDesc.RasterizerState.DepthClipEnable = true;//�[�x�����̃N���b�s���O����
	}

	//�u�����h�ݒ�
	void GraphicsPipeline::SetBlendState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc)
	{
		D3D12_RENDER_TARGET_BLEND_DESC renderBlend = {};
		{
			renderBlend.BlendEnable = false;//�u�����h�̗L��
			renderBlend.LogicOpEnable = false;//�_�����Z�̗L��
			renderBlend.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//�������ނƂ��̃}�X�N
		}

		pipelineDesc.BlendState.AlphaToCoverageEnable = false;//�A���t�@�e�X�g�̗L��
		pipelineDesc.BlendState.IndependentBlendEnable = false;//�u�����h�������_�[�^�[�Q�b�g�ŋ��ʉ����邩
		pipelineDesc.BlendState.RenderTarget[0] = renderBlend;
	}

	//���̓��C�A�E�g
	void GraphicsPipeline::SetInputLayout(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc)
	{
		pipelineDesc.InputLayout.pInputElementDescs = shaderInput;
		pipelineDesc.InputLayout.NumElements = _countof(shaderInput);
	}

	//�T���v���ݒ�
	void GraphicsPipeline::SetSample(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc)
	{
		pipelineDesc.SampleDesc.Count = 1;//�T���v�����O��1�s�N�Z����1��
		pipelineDesc.SampleDesc.Quality = LOW_QUALITY;//�N�I���e�B�͍Œ�
	}

	//�[�x�X�e���V���ݒ�
	void GraphicsPipeline::SetDepthStencilStare(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc)
	{
		pipelineDesc.DepthStencilState.DepthEnable = true;
		pipelineDesc.DepthStencilState.StencilEnable = false;
		pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	}

	void GraphicsPipeline::SetPipeline(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc, const D3D12_PRIMITIVE_TOPOLOGY topology)
	{
		pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//�T���v���}�X�N
		pipelineDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;//�g���C�A���O���X�g���b�v��؂藣���Ȃ�
		pipelineDesc.PrimitiveTopologyType = GetPrimitiveTopologyType(topology);//�O�p�`�ŕ\��
		pipelineDesc.NumRenderTargets = 1;//�����_�[�^�[�Q�b�g��1��(�}���`�����_�[�^�[�Q�b�g�ł͕ύX)
		pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0�`1�ɐ��K�����ꂽRGBA
		pipelineDesc.pRootSignature = rootSignature.Get();//���[�g�V�O�l�`����ݒ�
		pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	}

	//�p�C�v���C���X�e�[�g�̃Q�b�^�[
	Microsoft::WRL::ComPtr<ID3D12PipelineState> GraphicsPipeline::GetPipelineState()
	{
		return pipelineState;
	}

	Microsoft::WRL::ComPtr<ID3D12RootSignature> GraphicsPipeline::GetRootSignature()
	{
		return rootSignature;
	}

	Microsoft::WRL::ComPtr<ID3DBlob> GraphicsPipeline::GetRootBlob()
	{
		return rootBlob;
	}
}