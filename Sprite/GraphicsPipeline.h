#pragma once

#include"..\Base\Pch.h"
#include"DescriptorRange.h"
#include"RootParametor.h"
#include"RootSignature.h"

namespace Beginner
{

	//グラフィックパイプラインを持たせる
	class GraphicsPipeline
	{
	private:
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;//パイプライン本体
		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;//ルートシグネチャ
		Microsoft::WRL::ComPtr<ID3DBlob> rootBlob;//シグネチャのデータ

		bool CreateBinaryCode(const OBJECT_TYPE);//バイナリコードの作成
		bool CreateRootSignature(const HWND);//ルートシグネチャの作成

		//頂点シェーダの設定
		void SetVertexShader(D3D12_GRAPHICS_PIPELINE_STATE_DESC&, const Microsoft::WRL::ComPtr<ID3DBlob>&);

		//ピクセルシシェーダの設定
		void SetPixelShader(D3D12_GRAPHICS_PIPELINE_STATE_DESC&, const Microsoft::WRL::ComPtr<ID3DBlob>&);

		void SetRasterizerState(D3D12_GRAPHICS_PIPELINE_STATE_DESC&);//ラスタライザの設定
		void SetBlendState(D3D12_GRAPHICS_PIPELINE_STATE_DESC&);//ブレンド設定
		void SetInputLayout(D3D12_GRAPHICS_PIPELINE_STATE_DESC&);//入力配置の設定
		void SetSample(D3D12_GRAPHICS_PIPELINE_STATE_DESC&);//サンプル設定
		void SetPipeline(D3D12_GRAPHICS_PIPELINE_STATE_DESC&, const D3D12_PRIMITIVE_TOPOLOGY);
		void SetDepthStencilStare(D3D12_GRAPHICS_PIPELINE_STATE_DESC&);//深度ステンシル設定

	public:
		GraphicsPipeline() :pipelineState(nullptr), rootBlob(nullptr), rootSignature(nullptr) {}
		~GraphicsPipeline() {}

		Microsoft::WRL::ComPtr<ID3D12PipelineState> GetPipelineState();
		Microsoft::WRL::ComPtr<ID3DBlob> GetRootBlob();
		Microsoft::WRL::ComPtr<ID3D12RootSignature> GetRootSignature();

		bool CreateGraphicsPipeline(//パイプラインの作成
			const HWND, const D3D12_PRIMITIVE_TOPOLOGY, const OBJECT_TYPE,
			const Microsoft::WRL::ComPtr<ID3DBlob>&,
			const Microsoft::WRL::ComPtr<ID3DBlob>&
		);
	};

}