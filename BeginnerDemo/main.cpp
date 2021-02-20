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

	Figure* figure = Figure::CreateFigure(_countof(a), a);
	figure->position = { 0,0,1.0F };
	

	Sprite* sprite = Sprite::CreateSprite("Test.png");
	sprite->position = { 0.0F,0.0F,1.0F };
	
	//3D
	/*Model* model = Model::CreateModel("models/OBJ/spider.obj");
	model->position = { 0,0,100.0F };
	model->rotation = { 0,0,0 };*/
	

	Eye* eye = Eye::CreateEye();
	mainEye = eye;
	eye->farZ = 1000.0F;
	eye->nearZ = 1.0F;
	eye->eyeType = EYE_TYPE::ORTHOGRAPHIC;
	eye->rotation = { 0,0,0 };


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
		
		/*if (GetAsyncKeyState(VK_DOWN)) {
			model->rotation.z -= 1.0F;
			std::cout << "z " << model->rotation.z << std::endl;
		}
		if (GetAsyncKeyState(VK_UP)) {
			model->rotation.z += 1.0F;
			std::cout << "z " << model->rotation.z << std::endl;
		}
		if (GetAsyncKeyState(VK_RIGHT)) {
			model->rotation.x -= 1.0F;
			std::cout << "x " << model->rotation.x << std::endl;
		}
		if (GetAsyncKeyState(VK_LEFT)) {
			model->rotation.x += 1.0F;
			std::cout << "x " << model->rotation.x << std::endl;
		}
		if (GetAsyncKeyState(0x41)) {//A
			model->rotation.y += 1.0F;
			std::cout << "y " << model->rotation.y << std::endl;
		}
		if (GetAsyncKeyState(0x53)) {//S
			model->rotation.y -= 1.0F;
			std::cout << "y " << model->rotation.y << std::endl;
		}*/

		if (GetAsyncKeyState(VK_DOWN)) {
			sprite->position.z -= 2.0F;
			std::cout << "z " << sprite->position.z << std::endl;
		}
		if (GetAsyncKeyState(VK_UP)) {
			sprite->position.z += 2.0F;
			std::cout << "z " << sprite->position.z << std::endl;
		}
		if (GetAsyncKeyState(VK_RIGHT)) {
			sprite->position.x -= 2.0F;
			std::cout << "x " << sprite->position.x << std::endl;
		}
		if (GetAsyncKeyState(VK_LEFT)) {
			sprite->position.x += 2.0F;
			std::cout << "x " << sprite->position.x << std::endl;
		}
		if (GetAsyncKeyState(0x41)) {//A
			sprite->position.y += 2.0F;
			std::cout << "y " << sprite->position.y << std::endl;
		}
		if (GetAsyncKeyState(0x53)) {//S
			sprite->position.y -= 2.0F;
			std::cout << "y " << sprite->position.y << std::endl;
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