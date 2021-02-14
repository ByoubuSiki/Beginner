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

	/*Figure* figure = Figure::CreateFigure(_countof(a), a);
	figure->position = { 300,500,1.0F };*/
	

	/*Sprite* sprite = Sprite::CreateSprite("Test.png");
	sprite->position = { 400.0F,400.0F,1.5F };*/
	
	//3D
	/*Model* model = Model::CreateModel("models/OBJ/spider.obj");
	model->position = { 0,0,100.0F };
	model->rotation = { 0,0,0 };*/
	

	Eye* eye = Eye::CreateEye();
	mainEye = eye;
	eye->farZ = 1000.0F;
	eye->nearZ = 1.0F;
	eye->eyeType = PERSPECTIVE;
	//eye->position = { 400,400,0 }; //model�p
	eye->position = { 0,0,0 }; //figure�p


	MSG msg = {};

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//�t���[���n�߂̏���
		direct.FlameStart();
		
		

		//�t���[���̏I������
		if (!direct.FlameEnd())
		{
			break;
		}
	}

	CoUninitialize();
	return 0;
}