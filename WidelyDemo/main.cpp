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


	/*Vector3 a[5] = {
		{400,600,0},
		{600,500,0},
		{500,300,0},
		{300,300,0},
		{200,500,0}
	};

	Figure* figure = Figure::CreateFigure(_countof(a), a);
	figure->position = { 300,500,1.0F };

	if (!direct.Regist(figure))
	{
		return -1;
	}*/

	/*Sprite* sprite = Sprite::CreateSprite("Test.png");
	sprite->position = { 400.0F,400.0F,1.0F };


	if (!direct.Regist(sprite))
	{
		return -1;
	}*/
	
	//3D
	Model* model = Model::CreateModel("models/OBJ/spider.obj");
	model->position = { 0,0,100.0F };
	model->rotation = { 0,0,0 };

	if (!direct.Regist(model))
	{
		return false;
	}

	Eye* eye = Eye::CreateEye();
	eye->farZ = 1000.0F;
	eye->nearZ = 1.0F;
	eye->eyeType = PERSPECTIVE;
	eye->position = { 400,400,0 };
	if (!direct.Regist(eye))
	{
		return -1;
	}


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
			model->rotation.y -= 5.0F;
			std::cout <<"y "<< model->rotation.y << std::endl;
		}
		if (GetAsyncKeyState(VK_UP)) {
			model->rotation.y += 5.0F;
			std::cout << "y " << model->rotation.y << std::endl;
		}
		if (GetAsyncKeyState(VK_RIGHT)) {
			model->rotation.z -= 5.0F;
			std::cout << "z " << model->rotation.z << std::endl;
		}
		if (GetAsyncKeyState(VK_LEFT)) {
			model->rotation.z += 5.0F;
			std::cout << "z " << model->rotation.z << std::endl;
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