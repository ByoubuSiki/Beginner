#pragma once

#include"Base.h"
#include"..\Shader\Shader.h"
#include"..\Base\Pch.h"
#include"..\SetUp\DescriptorHeap.h"
#include"GraphicsPipeline.h"

namespace Beginner
{

	class Sprite :public Base
	{
	private:
		void CopyTexture();//テクスチャをコピー
		void SetVertex(float, float);//頂点情報を設定
		bool CreateSpriteBuffer();//頂点と構成順番のバッファを作成
		void VertexIndexOrder(std::vector<unsigned short>&, const Size);//頂点正規化とトポロジ決め
		void SetVertexView(const size_t, const UINT, const D3D12_GPU_VIRTUAL_ADDRESS);//頂点ビューの設定
		void SetIndexView(const UINT, const D3D12_GPU_VIRTUAL_ADDRESS);//インデックスビューの設定
		bool SetUpSprite();//SpriteのSetUp動作

		//Buffer
		Microsoft::WRL::ComPtr<ID3D12Resource> textureBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource> uploadBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer;
		DescriptorHeap cbv_srvHeap;

		std::vector<VertexUV> vertexUV;//頂点座標
		GraphicsPipeline pipeline;//グラフィックパイプライン

		//Image
		DirectX::TexMetadata metaData;
		DirectX::ScratchImage scratch;

		//View
		D3D12_VERTEX_BUFFER_VIEW vertexView;
		D3D12_INDEX_BUFFER_VIEW indexView;

	public:
		Sprite() :Sprite::Sprite(
			Shader::CreateShader("../Shader/BasicVertex.hlsl", "main", VERTEX_SHADER_5_0),
			Shader::CreateShader("../Shader/Sprite.hlsl", "main", PIXEL_SHADER_5_0)) {}
		Sprite(Shader* vertex, Shader* pixel) :
			vertexShader(vertex), pixelShader(pixel), indexView(), vertexView(),
			textureBuffer(nullptr), uploadBuffer(nullptr), cbv_srvHeap(), metaData() {}
		~Sprite() {}

		//Shader
		Shader* pixelShader;
		Shader* vertexShader;

		void DrawCall()override;//描画時に呼び出し

		static Sprite* CreateSprite(const char*);//Create Sprite
	};

	extern std::list<Sprite> spriteList;//画像
}