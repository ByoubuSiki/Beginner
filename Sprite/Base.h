#pragma once

#include"..\Base\Pch.h"
#include"Eye.h"

namespace Beginner
{

	//全オブジェクト共通
	class Base
	{
	private:
		bool ConstantMap();//定数のマップ動作

	protected:

		bool RegistObject(const HWND);//ObjectBaseのRegist動作
		void ApplyTransform(const HWND);//座標変換

		bool regist : 1;//Registフラグ
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer;//座標変換用定数バッファ
		DirectX::XMMATRIX* mapTransform;//座標変換用マップアドレス

	public:
		Base() :regist(false), position(0, 0, 0), rotation(0, 0, 0), scale(1, 1, 1) {}
		~Base() {}

		Vector3 position;//中心座標
		Vector3 rotation;//回転角度
		Vector3 scale;//拡縮サイズ

		//Getter
		bool GetRegist()const;
		virtual bool RegistCall(const HWND) = 0;//Regist時動作
		virtual void DrawCall(const HWND) = 0;//Draw時動作
	};

}
