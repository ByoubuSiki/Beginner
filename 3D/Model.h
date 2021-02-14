#pragma once

#include"..\Base\Pch.h"
#include"..\Sprite\Base.h"
#include"..\Sprite\GraphicsPipeline.h"
#include"..\Shader\Shader.h"
#include"..\SetUp\DescriptorHeap.h"

namespace Beginner
{
	//3Dモデルオブジェクト
	class Model :public Base
	{
	public:
		Model() :Model(
			Shader::CreateShader("../Shader/BasicVertex.hlsl", "main", VERTEX_SHADER_5_0),
			Shader::CreateShader("../Shader/Model.hlsl", "main", PIXEL_SHADER_5_0)) {}
		Model(Shader* vertex, Shader* pixel) :vertexShader(vertex), pixelShader(pixel), indexView(), vertexView() {}
		~Model() {}

		static Model* CreateModel(const char*);//Modelクラスの実態を作製
		void DrawCall()override;//描画時に呼び出し

		//Shader変数
		Shader* pixelShader;
		Shader* vertexShader;

	private:

		void LoadModelData(aiNode*, const aiScene*);//再帰でAssimpのNodeを読み取り
		void LoadModelMesh(aiMesh*);//AssimpのMesh情報を読み取り
		void LoadModelVertex(aiMesh*, const unsigned);//頂点の情報を読み取り
		bool SetUpModel();//ModelクラスのSetUp動作
		bool CreateModelBuffer();//Modelクラス用のバッファを作製・マップ処理
		void SetVertexView(const size_t, const UINT, const D3D12_GPU_VIRTUAL_ADDRESS);//頂点ビューの設定
		void SetIndexView(const size_t, const D3D12_GPU_VIRTUAL_ADDRESS);//インデックスビューの設定

		std::vector<Vertex> vertex;//構成する頂点情報
		std::vector<unsigned> vertexIndex;//ポリゴンを構成する頂点の配列番号

		GraphicsPipeline pipeline;//描画設定

		Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;//頂点用バッファのポインタ
		Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer;//頂点インデックスのポインタ

		DescriptorHeap cbvHeap;//CBV用のディスクリプタヒープ

		D3D12_VERTEX_BUFFER_VIEW vertexView;//頂点情報のビュー
		D3D12_INDEX_BUFFER_VIEW indexView;//頂点インデックス情報のビュー
	};

	extern std::list<Model> modelList;//作製したModelクラスの実態を記録
}