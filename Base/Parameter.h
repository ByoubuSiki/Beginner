#pragma once

#include<assimp\Importer.hpp>
#include<assimp\scene.h>
#include<assimp\postprocess.h>

#include<d3d12.h>
#include<d3dcompiler.h>

namespace Beginner
{
	constexpr int LOW_QUALITY = 0;//バッファのクオリティ

	
#pragma region 3D
	//Assimpの読込フラグ
	const unsigned int MODEL_LOAD_FAST = //最適化を最低
	{
		aiProcess_ConvertToLeftHanded |
		aiProcessPreset_TargetRealtime_Fast |
		aiProcess_OptimizeMeshes |//Meshを統合
		aiProcess_OptimizeGraph
	};

	const unsigned int MODEL_LOAD_MEDIUM = //最適化を標準
	{
		aiProcess_ConvertToLeftHanded |
		aiProcessPreset_TargetRealtime_Quality |
		aiProcess_OptimizeMeshes |//Meshを統合
		aiProcess_OptimizeGraph
	};
	
	const unsigned int MODEL_LOAD_MAX = //最適化
	{
		aiProcess_ConvertToLeftHanded |
		aiProcessPreset_TargetRealtime_MaxQuality |
		aiProcess_OptimizeMeshes |
		aiProcess_OptimizeGraph
	};
		

#pragma endregion

//Shader関連
#pragma region Shader

#pragma region SHaderVersion 

//Vertex Shader
#define VERTEX_SHADER_6_4 "vs_6_4"
#define VERTEX_SHADER_6_3 "vs_6_3"
#define VERTEX_SHADER_6_2 "vs_6_2"
#define VERTEX_SHADER_6_1 "vs_6_1"
#define VERTEX_SHADER_6_0 "vs_6_0"
#define VERTEX_SHADER_5_1 "vs_5_1"
#define VERTEX_SHADER_5_0 "vs_5_0"
#define VERTEX_SHADER_4_1 "vs_4_1"
#define VERTEX_SHADER_4_0 "vs_4_0"
#define VERTEX_SHADER_3_0 "vs_3_0"
#define VERTEX_SHADER_2_0 "vs_2_0"

//Pixel Shader
#define PIXEL_SHADER_6_4 "ps_6_4"
#define PIXEL_SHADER_6_3 "ps_6_3"
#define PIXEL_SHADER_6_2 "ps_6_2"
#define PIXEL_SHADER_6_1 "ps_6_1"
#define PIXEL_SHADER_6_0 "ps_6_0"
#define PIXEL_SHADER_5_1 "ps_5_1"
#define PIXEL_SHADER_5_0 "ps_5_0"
#define PIXEL_SHADER_4_1 "ps_4_1"
#define PIXEL_SHADER_4_0 "ps_4_0"
#define PIXEL_SHADER_3_0 "ps_3_0"
#define PIXEL_SHADER_2_0 "ps_2_0"

#pragma endregion

	extern D3D12_INPUT_ELEMENT_DESC shaderInput[6];//Shaderパラメータ

	constexpr unsigned SHADER_COMPILE_FLAGS =
	{
	#ifdef _DEBUG
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION //デバッグモード | 最適化を無効化
	#else
		D3DCOMPILE_OPTIMIZATION_LEVEL3 //最適化を最大にする
	#endif // _DEBUG
	};

#pragma endregion
}