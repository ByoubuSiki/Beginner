#pragma once

#include"Pch.h"

//ウィンドウの作成クラス
class Window
{
private:
	WNDCLASSEX w = {};
	RECT wrc = {};

	void RegistClass();//ウィンドウクラスの作成・登録
	void Show();//ウィンドウの表示

public:

	Window();
	~Window();

	void Create(const LONG, const LONG);//ウィンドウの作成

	HWND hwnd;//ウィンドウハンドル
	LONG width, height;//ウィンドウサイズ
};

//ウィンドウプロージャ
LRESULT WindowProcedure(HWND, UINT, WPARAM, LPARAM);