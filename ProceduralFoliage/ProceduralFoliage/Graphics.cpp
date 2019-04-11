#include "Graphics.h"

void TW_CALL CopyStdStringToClient(std::string& destinationClientString, const std::string& sourceLibraryString)
{
	destinationClientString = sourceLibraryString;
}

void TW_CALL LoadCB(void* client_data)
{
	Graphics* graphics = static_cast<Graphics*>(client_data);

	graphics->LoadTypeFile(graphics->GetFileName());
}

void TW_CALL ReloadCB(void* client_data)
{
	Graphics* graphics = static_cast<Graphics*>(client_data);

	graphics->Refresh();
}

void TW_CALL ExportCB(void* client_data)
{
	Graphics* graphics = static_cast<Graphics*>(client_data);

	graphics->ExportModel();
}

void TW_CALL LODCall(void *client_data)
{
	Graphics* graphics = static_cast<Graphics*>(client_data);

	graphics->ShowLOD();
	graphics->Refresh();
}

bool Graphics::Init(int screen_width, int screen_height, HWND hwnd)
{

	//Direct3D object.
	direct3D = new Direct3D;
	direct3D->Init(screen_width, screen_height, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);

	camera = new Camera;
	camera->SetPosition(0.0f, 3.0f, -10.0f);

	default_shader = new DefaultShader;
	default_shader->Init(direct3D->GetDevice(), hwnd);

	light = new DiffuseLight;
	light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	light->SetDiffuseColor(0.8f, 0.8f, 0.8f, 1.0f);
	light->SetDirection(0.0f, 1.0f, 0.0f);

	TwInit(TW_DIRECT3D11, direct3D->GetDevice());
	TwWindowSize(screen_width, screen_height);

	//anttweak bar used for the scene, includes options to load and export files, and show the LOD of models
	loader_bar = TwNewBar("Load File");

	TwCopyStdStringToClientFunc(CopyStdStringToClient);
	TwAddVarRW(loader_bar, "File Name", TW_TYPE_STDSTRING, &load_file_name, "");
	TwAddButton(loader_bar, "LOAD", LoadCB, this, " label='LOAD TYPE ");
	TwAddSeparator(loader_bar, "", NULL);
	TwAddButton(loader_bar, "REFRESH", ReloadCB, this, " label='REFRESH MODEL ");
	TwAddSeparator(loader_bar, "", NULL);
	TwAddButton(loader_bar, "EXPORT", ExportCB, this, " label='EXPORT MODEL ");
	TwAddSeparator(loader_bar, "", NULL);
	TwAddButton(loader_bar, "LOD", LODCall, this, " label='SHOW LOD ");

	//where the magic happens, the turtle creates the LSystem and the model
	turtle = new Turtle;
	turtle->Generate(direct3D->GetDevice(),direct3D->GetDeviceContext(), "fern");

	exporter = new ModelExporter;

	lod = new LOD;
	lod->Init(direct3D->GetDevice(), direct3D->GetDeviceContext(), screen_width, screen_height);
	lod->SetQuadPosition(0,3,0);
	lod->SetQuadScale(4, 4, 0);

	//system("C:/users/Ellio/source/repos/ProceduralFoliage/ProceduralTexturesCTP/ProceduralTexturesCTP/runscript.bat");
	return true;
}

void Graphics::LoadTypeFile(std::string file_name)
{
	if (turtle)
	{
		delete turtle;
		turtle = nullptr;
	}

	turtle = new Turtle;
	turtle->Generate(direct3D->GetDevice(), direct3D->GetDeviceContext(), file_name);
}

void Graphics::Refresh()
{
	if (turtle)
	{
		LSystem* LS = turtle->GetLSystem();
		LS->SaveType("temp");
		LoadTypeFile("temp");
	}
}

void Graphics::ExportModel()
{
	exporter->Export(turtle->GetName(), turtle->GetModelList(), world_matrix);
}

bool Graphics::Update()
{
	Render();
	return true;
}

void Graphics::ShowLOD()
{
	render_lods = !render_lods;
}

void Graphics::RenderLOD(float x, float y, float z)
{
	lod->SetRenderTarget(direct3D->GetDeviceContext(), direct3D->GetDepthStencilView());

	lod->ClearRenderTarget(direct3D->GetDeviceContext(), direct3D->GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 0.0f);

	RenderScene(x, y, z);

	direct3D->SetBackBufferTarget();
}

void Graphics::RenderScene(float x, float y, float z)
{
	camera->Render();

	camera->GetViewMatrix(view_matrix);
	direct3D->GetProjectionMatrix(projection_matrix);
	static float ang = 0;
	if(render_lods == false)
	{
		ang += 0.01f;
		if (ang > 360)
		{
			ang = 0;
		}
	}

	XMMATRIX cam_rot;
	XMMATRIX cam_trans;
	cam_rot = XMMatrixRotationY(ang + y);
	cam_trans = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	std::vector<Model*> transparent_list;

	for (int i = 0; i < turtle->GetModelList().size(); i++)
	{

		Model* model = turtle->GetModelList()[i];

		if (model->GetBlendAmount() != 0.0f)
		{
			//transparent objects need to render after opaque
			transparent_list.push_back(model);
		}
		else
		{
			model->Render(direct3D->GetDeviceContext());
			world_matrix = cam_trans * cam_rot;

			default_shader->Render(direct3D->GetDeviceContext(), model->GetIndexCount(), world_matrix, view_matrix, projection_matrix,
				model->GetTexture(), model->GetMetallic(), model->GetRoughness(), light->GetDirection(), light->GetAmbientColor(), light->GetDiffuseColor(), model->GetBlendAmount());

			direct3D->GetWorldMatrix(world_matrix);
		}
	}

	for (int i = 0; i < transparent_list.size(); i++)
	{
		Model* model = transparent_list[i];

		model->Render(direct3D->GetDeviceContext());
		world_matrix = model->GetTransform() * cam_trans * cam_rot;

		direct3D->TurnOnAlphaBlending();
		default_shader->Render(direct3D->GetDeviceContext(), model->GetIndexCount(), world_matrix, view_matrix, projection_matrix,
			model->GetTexture(), model->GetMetallic(), model->GetRoughness(), light->GetDirection(), light->GetAmbientColor(), light->GetDiffuseColor(), model->GetBlendAmount());

		direct3D->GetWorldMatrix(world_matrix);
	}
	
	direct3D->TurnOffAlphaBlending();
}

bool Graphics::Render()
{
	if (render_lods == true)
	{
		//render the LOD render texture to its quads.
		static float ang = 0;
		ang += 0.01f;
		if (ang > 360)
		{
			ang = 0;
		}
		XMMATRIX cam_rot;
		cam_rot = XMMatrixRotationY(ang);
		std::vector<Model*> lod_models = lod->GetModel();
		direct3D->StartScene(0.3f, 0.3f, 0.3f, 1.0f);

		for (int i = 0; i < lod_models.size(); i++)
		{
			if (i == 0)
			{
				//render normally
				RenderLOD(0, 0, 0);
				lod_models[0]->SetPosition(0, 5, 0);
			}
			if (i == 1)
			{
				//render again at 90 degree angle
				RenderLOD(0,90,0);
				lod_models[1]->SetRotation(0, 1.5f, 0, 1);
				lod_models[1]->SetPosition(0, 5, 0);
			}
			
			lod_models[i]->Render(direct3D->GetDeviceContext());

			world_matrix = lod_models[i]->GetTransform() * cam_rot;

			direct3D->TurnOnAlphaBlending();
			default_shader->Render(direct3D->GetDeviceContext(), lod_models[i]->GetIndexCount(), world_matrix, view_matrix, projection_matrix,
				lod->GetShaderResourceView(), lod_models[i]->GetMetallic(), lod_models[i]->GetRoughness(), light->GetDirection(), light->GetAmbientColor(), light->GetDiffuseColor(), lod_models[i]->GetBlendAmount());
		}

		direct3D->TurnOffAlphaBlending();
		
	}
	else
	{
		direct3D->StartScene(0.3f, 0.3f, 0.3f, 1.0f);
		RenderScene(0,0,0);
	}

	TwDraw();
	direct3D->EndScene();

	return true;
}