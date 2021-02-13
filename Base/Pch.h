#pragma once

#define NOMINMAX

#include<iostream>
#include<tchar.h>
#include<string>
#include<vector>
#include<clocale>
#include<dxgi1_6.h>
#include<DirectXMath.h>
#include<unordered_map>

#include"..\DirectXTex\d3dx12.h"
#include"..\DirectXTex\DirectXTex.h"
#include"Parameter.h"
#include"Common.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"DirectXTex.lib")

namespace Beginner
{
	//ログ表示
	void DebugLogOnConsole(const char*);
	void DebugLogOnOutput(const std::string&);

	std::wstring TransformToWideChar(const std::string&);//stringをwstring変換

	size_t AlignmentedSize(const size_t, const size_t);//値を任意の値に整列

	//ベクトルを0～１で正規化
	DirectX::XMFLOAT3 FigureVertex(const Vector3&, const Size);
	DirectX::XMFLOAT3 FigureVertex(const DirectX::XMFLOAT3&, const Size);

	//バッファの作成
	bool CreateBuffer(const HWND, Microsoft::WRL::ComPtr<ID3D12Resource>&, const size_t);
	bool CreateBuffer(const HWND, Microsoft::WRL::ComPtr<ID3D12Resource>&, const DirectX::TexMetadata&);
	bool CreateDepthBuffer(const HWND, Microsoft::WRL::ComPtr<ID3D12Resource>&);

	//バッファにマップ
	VertexUV* BufferMap(Microsoft::WRL::ComPtr<ID3D12Resource>&, std::vector<VertexUV>::iterator, std::vector<VertexUV>::iterator, const bool);
	unsigned short* BufferMap(Microsoft::WRL::ComPtr<ID3D12Resource>&, std::vector<unsigned short>::iterator, std::vector<unsigned short>::iterator, const bool);
	unsigned* BufferMap(Microsoft::WRL::ComPtr<ID3D12Resource>&, std::vector<unsigned>::iterator, std::vector<unsigned>::iterator, const bool);
	Vector3* BufferMap(Microsoft::WRL::ComPtr<ID3D12Resource>&, std::vector<Vector3>::iterator, std::vector<Vector3>::iterator, const bool);
	Vertex* BufferMap(Microsoft::WRL::ComPtr<ID3D12Resource>&, std::vector<Vertex>::iterator, std::vector<Vertex>::iterator, const bool);
	uint8_t* BufferMap(Microsoft::WRL::ComPtr<ID3D12Resource>&, const DirectX::Image&, const bool);

	D3D12_PRIMITIVE_TOPOLOGY GetPrimitiveTopology(const unsigned short);//頂点数からポリゴン図形を取得
	D3D12_PRIMITIVE_TOPOLOGY_TYPE GetPrimitiveTopologyType(const D3D12_PRIMITIVE_TOPOLOGY);//グラフィックパイプライン用のプリミティブを取得

	Size GetWindowSize(const HWND);	//ウィンドウサイズを取得

	//XMFLOAT3とVector3間の変換
	Vector3 TransformToVector3(const DirectX::XMFLOAT3&);
	DirectX::XMFLOAT3 TransformToXMFLOAT3(const Vector3&);

	void OutPutShaderError(HRESULT&, Microsoft::WRL::ComPtr<ID3DBlob>&);//Shaderのエラーを表示

	Vector3 VertexToPivot(const unsigned int, const Vector3[]);//図形の中心座標を取得

	void OutPutRootSignatureError(Microsoft::WRL::ComPtr<ID3DBlob>&);//ルートシグネチャのエラー表示

	//ビューの作製
	void CreateConstantView(const HWND, Microsoft::WRL::ComPtr<ID3D12Resource>&, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>&);
	void CreateShaderResourceView(const HWND, Microsoft::WRL::ComPtr<ID3D12Resource>&, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>&, const DXGI_FORMAT);
	void CreateDepthView(const HWND, Microsoft::WRL::ComPtr<ID3D12Resource>&, const D3D12_CPU_DESCRIPTOR_HANDLE);
}