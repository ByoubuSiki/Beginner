#pragma once

namespace Beginner
{
	enum OBJECT_TYPE //オブジェクトの種類
	{
		FIGURE_TYPE = 0,
		SPRITE_TYPE = 1,
		MODEL_TYPE = 2
	};

	enum SHADER_TYPE //Shaderの種類
	{
		VERTEX_SHADER = 0,
		PIXEL_SHADER = 1
	};

	enum EYE_TYPE //視点の3D,2D表示
	{
		ORTHOGRAPHIC = 0,//2D
		PERSPECTIVE = 1//3D
	};
}