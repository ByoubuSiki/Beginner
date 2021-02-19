#include"BeginnerHeader.h"

using namespace Beginner;

#ifdef _DEBUG
int main()
{
#else
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
#endif//_DEBUG


	HRESULT result = CoInitializeEx(0, COINIT_MULTITHREADED);

	Window win;
	win.Create(800, 800);

	Begin direct;

	if (!direct.Init(win.hwnd))
	{
		return -1;
	}


	Vector3 a[5] = {
		{400,600,0},
		{600,500,0},
		{500,300,0},
		{300,300,0},
		{200,500,0}
	};

	//Figure* figure = Figure::CreateFigure(_countof(a), a);
	//figure->rotation = { 300,500,1.0F };
	

	Sprite* sprite = Sprite::CreateSprite("Test.png");
	sprite->rotation = { 400.0F,400.0F,1.0F };
	
	//3D
	/*Model* model = Model::CreateModel("models/OBJ/spider.obj");
	model->rotation = { 0,0,100.0F };
	model->rotation = { 0,0,0 };*/
	

	Eye* eye = Eye::CreateEye();
	mainEye = eye;
	eye->farZ = 1000.0F;
	eye->nearZ = 1.0F;
	eye->eyeType = PERSPECTIVE;
	//eye->rotation = { 400,400,0 }; //model用
	eye->rotation = { 0,0,0 }; //figure用


	MSG msg = {};

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//フレーム始めの処理
		direct.FlameStart();
		
		if (GetAsyncKeyState(VK_DOWN)) {
			eye->rotation.y -= 5.0F;
			std::cout << "y " << eye->rotation.y << std::endl;
		}
		if (GetAsyncKeyState(VK_UP)) {
			eye->rotation.y += 5.0F;
			std::cout << "y " << eye->rotation.y << std::endl;
		}
		if (GetAsyncKeyState(VK_RIGHT)) {
			eye->rotation.x -= 5.0F;
			std::cout << "x " << eye->rotation.x << std::endl;
		}
		if (GetAsyncKeyState(VK_LEFT)) {
			eye->rotation.x += 5.0F;
			std::cout << "x " << eye->rotation.x << std::endl;
		}
		

		//フレームの終了処理
		if (!direct.FlameEnd())
		{
			break;
		}
	}

	CoUninitialize();
	return 0;
}