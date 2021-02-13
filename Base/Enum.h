#pragma once

namespace Beginner
{
	enum OBJECT_TYPE //�I�u�W�F�N�g�̎��
	{
		FIGURE_TYPE = 0,
		SPRITE_TYPE = 1,
		MODEL_TYPE = 2
	};

	enum SHADER_TYPE //Shader�̎��
	{
		VERTEX_SHADER = 0,
		PIXEL_SHADER = 1
	};

	enum EYE_TYPE //���_��3D,2D�\��
	{
		ORTHOGRAPHIC = 0,//2D
		PERSPECTIVE = 1//3D
	};
}