#pragma once

#include"..\Base\Pch.h"
#include"Base.h"
#include"..\Shader\Shader.h"
#include"GraphicsPipeline.h"
#include"..\Setup\DescriptorHeap.h"

namespace Beginner
{

	//図形描画
	class Figure : public Base
	{
	private:
		void VertexIndexOrder(std::vector<unsigned short>&, const Size);//Vertex Order
		bool RegistFigure(const HWND);//Regist時の動作
		void SetVertexView(const size_t, const UINT, const D3D12_GPU_VIRTUAL_ADDRESS);//頂点ビューの設定
		void SetIndexView(const UINT, const D3D12_GPU_VIRTUAL_ADDRESS);//インデックスビューの設定
		bool CreateFigureBuffer(const HWND);//頂点と構成順番のバッファを作成

		D3D12_PRIMITIVE_TOPOLOGY drawTopology;//描画
		GraphicsPipeline pipeline;//パイプライン

		//View
		D3D12_VERTEX_BUFFER_VIEW vertexView;
		D3D12_INDEX_BUFFER_VIEW indexView;

		std::vector<VertexUV> vertexUV;//頂点座標

		//Buffer
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer;
		DescriptorHeap cbvHeap;

	public:
		Figure() :Figure::Figure(
			Shader::CreateShader("../Shader/BasicVertex.hlsl", "main", VERTEX_SHADER_5_0),
			Shader::CreateShader("../Shader/Figure.hlsl", "main", PIXEL_SHADER_5_0)) {}
		Figure(Shader* vertex, Shader* pixel)
			:vertexUV(), pipeline(), cbvHeap(), vertexView(), indexView(), vertexBuffer(nullptr),
			indexBuffer(nullptr), drawTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
		{
			vertexShader = vertex; pixelShader = pixel;
		}
		~Figure() {}

		bool RegistCall(const HWND)override;//Regist時に呼び出し
		void DrawCall(const HWND)override;//描画時に呼び出し

		static Figure* CreateFigure(const unsigned short, Vector3[]);//図形の作成

		//Shader
		Shader* pixelShader;
		Shader* vertexShader;
	};

	extern std::list<Figure> figureList;//図形
}