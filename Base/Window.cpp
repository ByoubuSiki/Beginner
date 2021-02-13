#include"Window.h"

Window::Window()
{

}

Window::~Window()
{
	UnregisterClass(w.lpszClassName, w.hInstance);
}

//クラスの登録
void Window::RegistClass()
{
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProcedure;//コールバック関数の指定
	w.lpszClassName = _T("DXSample");//アプリケーションクラス名
	w.hInstance = GetModuleHandle(nullptr);//ハンドルの取得

	RegisterClassEx(&w);//アプリケーションクラスの登録

	wrc = { 0,0,width,height };//ウィンドウサイズ
}

//ウィンドウの表示
void Window::Show()
{
	//ウィンドウサイズの補正
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	//ウィンドウオブジェクトの作成
	hwnd = CreateWindow(
		w.lpszClassName,//クラス名
		_T("DX12テスト"),//タイトルバー
		WS_OVERLAPPEDWINDOW,//タイトルバーと境界線があるウィンドウ
		CW_USEDEFAULT,//OSに任せる
		CW_USEDEFAULT,//OSに任せる
		width,//ウィンドウ横幅
		height,//ウィンドウ縦幅
		nullptr,//親ウィンドウハンドル
		nullptr,//メニューハンドル
		w.hInstance,//呼び出しアプリケーションハンドル
		nullptr//追加パラメータ
	);

	//ウィンドウ表示
	ShowWindow(hwnd, SW_SHOW);
}

//ウィンドウの作成
void Window::Create(const LONG window_width, const LONG window_height)
{
	width = window_width;
	height = window_height;

	RegistClass();

	Show();
}

//ウィンドウプロージャ
LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//ウィンドウが削除
	if (msg == WM_DESTROY)
	{
		//OSに終了を通知
		PostQuitMessage(0);
		return 0;
	}

	//規定の処理を実行
	return DefWindowProc(hwnd, msg, wparam, lparam);
}