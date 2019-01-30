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

bool Graphics::Init(int screen_width, int screen_height, HWND hwnd)
{
	bool result;


	// Initialize the Direct3D object.
	m_Direct3D = new Direct3D;
	m_Direct3D->Init(screen_width, screen_height, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);

	//Initialize the Camera object.
	m_camera = new Camera;
	m_camera->SetPosition(0.0f, 3.0f, -10.0f);

	//Initialize the Shaders
	m_default_shader = new DefaultShader;
	m_default_shader->Init(m_Direct3D->GetDevice(), hwnd);

	//Initialize the Diffuse Lights
	m_Light = new DiffuseLight;
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.5f, -0.5f, 1.0f);

	TwInit(TW_DIRECT3D11, m_Direct3D->GetDevice());
	TwWindowSize(screen_width, screen_height);

	loader_bar = TwNewBar("Load File");

	TwCopyStdStringToClientFunc(CopyStdStringToClient);
	TwAddVarRW(loader_bar, "File Name", TW_TYPE_STDSTRING, &load_file_name, "");
	TwAddButton(loader_bar, "LOAD", LoadCB, this, " label='LOAD TYPE ");
	TwAddSeparator(loader_bar, "", NULL);
	TwAddButton(loader_bar, "REFRESH", ReloadCB, this, " label='REFRESH MODEL ");
	TwAddSeparator(loader_bar, "", NULL);
	TwAddButton(loader_bar, "EXPORT", ExportCB, this, " label='EXPORT MODEL ");



	turtle = new Turtle;
	turtle->Generate(m_Direct3D->GetDevice(),m_Direct3D->GetDeviceContext(), "fern");

	exporter = new ModelExporter;

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
	turtle->Generate(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), file_name);
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

bool Graphics::Frame()
{
	Render();
	return true;
}

bool Graphics::Render()
{

	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_camera->Render();

	m_camera->GetViewMatrix(view_matrix);
	m_Direct3D->GetProjectionMatrix(projection_matrix);
	static float ang = 0;
	ang += 0.01f;
	if (ang > 360)
	{
		ang = 0;
	}

	XMMATRIX cam_rot;
	XMMATRIX cam_trans;
	cam_rot = XMMatrixRotationY(ang);
	cam_trans = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	
	std::vector<Model*> transparent_list;

	XMMATRIX move_mat, rot_mat, scale_mat;

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
			model->Render(m_Direct3D->GetDeviceContext());

			world_matrix = model->GetTransform() * cam_trans * cam_rot;

			m_default_shader->Render(m_Direct3D->GetDeviceContext(), model->GetIndexCount(), world_matrix, view_matrix, projection_matrix,
				model->GetTexture(), model->GetMetallic(), model->GetRoughness(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), model->GetBlendAmount());

			m_Direct3D->GetWorldMatrix(world_matrix);
		}
	}

	for (int i = 0; i < transparent_list.size(); i++)
	{
		Model* model = transparent_list[i];

		model->Render(m_Direct3D->GetDeviceContext());

		world_matrix = model->GetTransform() * cam_trans * cam_rot;

		m_Direct3D->TurnOnAlphaBlending();
		m_default_shader->Render(m_Direct3D->GetDeviceContext(), model->GetIndexCount(), world_matrix, view_matrix, projection_matrix,
			model->GetTexture(), model->GetMetallic(), model->GetRoughness(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), model->GetBlendAmount());

		m_Direct3D->GetWorldMatrix(world_matrix);
	}
	m_Direct3D->TurnOffAlphaBlending();
	TwDraw();
	m_Direct3D->EndScene();

	return true;
}