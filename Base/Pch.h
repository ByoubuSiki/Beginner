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
	//���O�\��
	void DebugLogOnConsole(const char*);
	void DebugLogOnOutput(const std::string&);

	std::wstring TransformToWideChar(const std::string&);//string��wstring�ϊ�

	size_t AlignmentedSize(const size_t, const size_t);//�l��C�ӂ̒l�ɐ���

	//�x�N�g����0�`�P�Ő��K��
	DirectX::XMFLOAT3 FigureVertex(const Vector3&, const Size);
	DirectX::XMFLOAT3 FigureVertex(const DirectX::XMFLOAT3&, const Size);

	//�o�b�t�@�̍쐬
	bool CreateBuffer(const HWND, Microsoft::WRL::ComPtr<ID3D12Resource>&, const size_t);
	bool CreateBuffer(const HWND, Microsoft::WRL::ComPtr<ID3D12Resource>&, const DirectX::TexMetadata&);
	bool CreateDepthBuffer(const HWND, Microsoft::WRL::ComPtr<ID3D12Resource>&);

	//�o�b�t�@�Ƀ}�b�v
	VertexUV* BufferMap(Microsoft::WRL::ComPtr<ID3D12Resource>&, std::vector<VertexUV>::iterator, std::vector<VertexUV>::iterator, const bool);
	unsigned short* BufferMap(Microsoft::WRL::ComPtr<ID3D12Resource>&, std::vector<unsigned short>::iterator, std::vector<unsigned short>::iterator, const bool);
	unsigned* BufferMap(Microsoft::WRL::ComPtr<ID3D12Resource>&, std::vector<unsigned>::iterator, std::vector<unsigned>::iterator, const bool);
	Vector3* BufferMap(Microsoft::WRL::ComPtr<ID3D12Resource>&, std::vector<Vector3>::iterator, std::vector<Vector3>::iterator, const bool);
	Vertex* BufferMap(Microsoft::WRL::ComPtr<ID3D12Resource>&, std::vector<Vertex>::iterator, std::vector<Vertex>::iterator, const bool);
	uint8_t* BufferMap(Microsoft::WRL::ComPtr<ID3D12Resource>&, const DirectX::Image&, const bool);

	D3D12_PRIMITIVE_TOPOLOGY GetPrimitiveTopology(const unsigned short);//���_������|���S���}�`���擾
	D3D12_PRIMITIVE_TOPOLOGY_TYPE GetPrimitiveTopologyType(const D3D12_PRIMITIVE_TOPOLOGY);//�O���t�B�b�N�p�C�v���C���p�̃v���~�e�B�u���擾

	Size GetWindowSize(const HWND);	//�E�B���h�E�T�C�Y���擾

	//XMFLOAT3��Vector3�Ԃ̕ϊ�
	Vector3 TransformToVector3(const DirectX::XMFLOAT3&);
	DirectX::XMFLOAT3 TransformToXMFLOAT3(const Vector3&);

	void OutPutShaderError(HRESULT&, Microsoft::WRL::ComPtr<ID3DBlob>&);//Shader�̃G���[��\��

	Vector3 VertexToPivot(const unsigned int, const Vector3[]);//�}�`�̒��S���W���擾

	void OutPutRootSignatureError(Microsoft::WRL::ComPtr<ID3DBlob>&);//���[�g�V�O�l�`���̃G���[�\��

	//�r���[�̍쐻
	void CreateConstantView(const HWND, Microsoft::WRL::ComPtr<ID3D12Resource>&, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>&);
	void CreateShaderResourceView(const HWND, Microsoft::WRL::ComPtr<ID3D12Resource>&, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>&, const DXGI_FORMAT);
	void CreateDepthView(const HWND, Microsoft::WRL::ComPtr<ID3D12Resource>&, const D3D12_CPU_DESCRIPTOR_HANDLE);
}