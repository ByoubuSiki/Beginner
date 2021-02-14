#include"GraphicsPipeline.h"

namespace Beginner
{

	//バイナリコードの作成
	bool GraphicsPipeline::CreateBinaryCode(const OBJECT_TYPE objectType)
	{
		Microsoft::WRL::ComPtr<ID3DBlob> error;//エラー受け取り

		DescriptorRange descRange;//DescriptorRangeの管理

		switch (objectType)	//作成オブジェクト別にDescriptorRange設定
		{
		case FIGURE_TYPE:
			descRange.PushBack(1, D3D12_DESCRIPTOR_RANGE_TYPE_CBV);//1つめは定数バッファ
			break;

		case SPRITE_TYPE:
			descRange.PushBack(1, D3D12_DESCRIPTOR_RANGE_TYPE_CBV);//1つめは定数バッファ
			descRange.PushBack(1, D3D12_DESCRIPTOR_RANGE_TYPE_SRV);//2つめはShadeResource
			break;

		case MODEL_TYPE:
			descRange.PushBack(1, D3D12_DESCRIPTOR_RANGE_TYPE_CBV);//1つめは定数バッファ
			//descRange.PushBack(1, D3D12_DESCRIPTOR_RANGE_TYPE_SRV);//2つめはShadeResource
			break;

		default: DebugLogOnConsole("規定外のオブジェクト\n");
			return false;
		}

		//パラメータ作成
		RootParametor rootPrametor;
		rootPrametor.PushBack(descRange.GetDescRange(0), (UINT)descRange.GetSize());

		D3D12_STATIC_SAMPLER_DESC sampler = {};
		{
			sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
			sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;//補間なし
			sampler.MaxLOD = D3D12_FLOAT32_MAX;
			sampler.MinLOD = 0.0F;
			sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		}

		RootSignature rootSignature;//ルートシグネチャの設定

		rootSignature.SetParametor((UINT)rootPrametor.GetSize(), rootPrametor.GetRootParameter(0));
		rootSignature.SetStaticSamper(1, &sampler);

		HRESULT result = D3D12SerializeRootSignature(
			rootSignature.GetRootSignature(), D3D_ROOT_SIGNATURE_VERSION_1_0,
			rootBlob.ReleaseAndGetAddressOf(), error.ReleaseAndGetAddressOf()
		);

		if (FAILED(result))
		{
			DebugLogOnConsole("バイナリコードの作成が失敗\n");
			OutPutRootSignatureError(error);
			return false;
		}

		return true;
	}

	//ルートシグネチャの作成
	bool GraphicsPipeline::CreateRootSignature()
	{
		HRESULT result = device->CreateRootSignature(
			0, rootBlob->GetBufferPointer(), rootBlob->GetBufferSize(),
			IID_PPV_ARGS(rootSignature.ReleaseAndGetAddressOf())
		);

		if (FAILED(result))
		{
			DebugLogOnConsole("ルートシグネチャの作成が失敗\n");
			return false;
		}

		return true;
	}

	//パイプラインの作成
	bool GraphicsPipeline::CreateGraphicsPipeline(
		const D3D12_PRIMITIVE_TOPOLOGY topology, const OBJECT_TYPE objectType,
		const Microsoft::WRL::ComPtr<ID3DBlob>& vsShader,
		const Microsoft::WRL::ComPtr<ID3DBlob>& psShader
	)
	{
		//ルートシグネチャの作成
		if (!CreateBinaryCode(objectType) || !CreateRootSignature())
		{
			return false;
		}

		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc = {};//パイプライン設定

		//各項目を設定
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

		//パイプラインステートを作成
		HRESULT result = device->CreateGraphicsPipelineState(
			&pipelineDesc, IID_PPV_ARGS(pipelineState.ReleaseAndGetAddressOf())
		);

		if (FAILED(result))
		{
			DebugLogOnConsole("パイプラインステートの作成が失敗\n");
			return false;
		}

		return true;
	}

	//頂点シェーダの設定
	void GraphicsPipeline::SetVertexShader(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc, const Microsoft::WRL::ComPtr<ID3DBlob>& shader)
	{
		pipelineDesc.VS.BytecodeLength = shader->GetBufferSize();
		pipelineDesc.VS.pShaderBytecode = shader->GetBufferPointer();
	}

	//ピクセルシェーダの設定
	void GraphicsPipeline::SetPixelShader(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc, const Microsoft::WRL::ComPtr<ID3DBlob>& shader)
	{
		pipelineDesc.PS.BytecodeLength = shader->GetBufferSize();
		pipelineDesc.PS.pShaderBytecode = shader->GetBufferPointer();
	}

	//ラスタライザの設定
	void GraphicsPipeline::SetRasterizerState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc)
	{
		pipelineDesc.RasterizerState.MultisampleEnable = false;//マルチサンプルアンチエイリアシング(MSAA)
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//カリングなし
		pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//ポリゴン塗りつぶし
		pipelineDesc.RasterizerState.DepthClipEnable = true;//深度方向のクリッピングあり
	}

	//ブレンド設定
	void GraphicsPipeline::SetBlendState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc)
	{
		D3D12_RENDER_TARGET_BLEND_DESC renderBlend = {};
		{
			renderBlend.BlendEnable = false;//ブレンドの有無
			renderBlend.LogicOpEnable = false;//論理演算の有無
			renderBlend.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//書き込むときのマスク
		}

		pipelineDesc.BlendState.AlphaToCoverageEnable = false;//アルファテストの有無
		pipelineDesc.BlendState.IndependentBlendEnable = false;//ブレンドをレンダーターゲットで共通化するか
		pipelineDesc.BlendState.RenderTarget[0] = renderBlend;
	}

	//入力レイアウト
	void GraphicsPipeline::SetInputLayout(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc)
	{
		pipelineDesc.InputLayout.pInputElementDescs = shaderInput;
		pipelineDesc.InputLayout.NumElements = _countof(shaderInput);
	}

	//サンプル設定
	void GraphicsPipeline::SetSample(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc)
	{
		pipelineDesc.SampleDesc.Count = 1;//サンプリングは1ピクセルに1つ
		pipelineDesc.SampleDesc.Quality = LOW_QUALITY;//クオリティは最低
	}

	//深度ステンシル設定
	void GraphicsPipeline::SetDepthStencilStare(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc)
	{
		pipelineDesc.DepthStencilState.DepthEnable = true;
		pipelineDesc.DepthStencilState.StencilEnable = false;
		pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	}

	void GraphicsPipeline::SetPipeline(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc, const D3D12_PRIMITIVE_TOPOLOGY topology)
	{
		pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//サンプルマスク
		pipelineDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;//トライアングルストリップを切り離さない
		pipelineDesc.PrimitiveTopologyType = GetPrimitiveTopologyType(topology);//三角形で表現
		pipelineDesc.NumRenderTargets = 1;//レンダーターゲットは1つ(マルチレンダーターゲットでは変更)
		pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0～1に正規化されたRGBA
		pipelineDesc.pRootSignature = rootSignature.Get();//ルートシグネチャを設定
		pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	}

	//パイプラインステートのゲッター
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