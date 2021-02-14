#pragma once

#include"..\Base\Pch.h"
#include"Base.h"

namespace Beginner
{
	//視点
	class Eye
	{
	public:
		Eye();
		~Eye();

		Vector3 position;//中心座標
		Vector3 rotation;//回転角度
		Vector3 scale;//拡縮サイズ

		EYE_TYPE eyeType;//描画モード
		float fov;//画角
		float nearZ, farZ;//視界の奥行

		void UpdateViewMatrix();//ビュー行列の更新
		DirectX::XMMATRIX& GetViewMatrix() { return viewMatrix; }
		DirectX::XMMATRIX& GetProjMatrix() { return projctionMatrix; }

		bool GetRegist()const { return regist; }
		static Eye* CreateEye();//Eyeの作成

	private:
		DirectX::XMMATRIX viewMatrix;//ビュー行列
		DirectX::XMMATRIX projctionMatrix;//プロジェクション行列7

		bool regist : 1;
	};

	extern std::list<Eye> eyeList;//視点
}