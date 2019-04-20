#include "Renderer3D.h"
#include "Object3D.h"
#include "VoxelMap3D.h"
#include "Model3D.h"

int main()
{
	Renderer3D* mainRenderer = new Renderer3D();
	Object3D* firstObject = new Object3D(
		new VoxelMap3D(
			new Model3D("C:/Users/Burak/Desktop/untitled66666.obj") // 2, 666, 66666
		));
	firstObject->SetPosition(Vec3f(0, 0, -4.6)); // 2, -4, -4.6
	mainRenderer->AddObject(firstObject);
	mainRenderer->Render();
	/*FirstProgram program = FirstProgram();
	Image2D* img = new Image2D(Vec2i(400, 400));
	img->PostProcess(&FirstProgram::Shader, &program);
	img->Render();*/
	while (true);
	return 0;
}