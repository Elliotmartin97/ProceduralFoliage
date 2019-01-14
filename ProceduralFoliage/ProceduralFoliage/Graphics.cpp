#include "Graphics.h"

bool Graphics::Init(int screen_width, int screen_height, HWND hwnd)
{
	bool result;


	// Initialize the Direct3D object.
	m_Direct3D = new Direct3D;
	m_Direct3D->Init(screen_width, screen_height, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);

	//Initialize the Camera object.
	m_camera = new Camera;
	m_camera->SetPosition(0.0f, 2.0f, -10.0f);

	//Initialize the Shaders
	m_default_shader = new DefaultShader;
	m_default_shader->Init(m_Direct3D->GetDevice(), hwnd);

	//Initialize the Diffuse Lights
	m_Light = new DiffuseLight;
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.5f, -0.5f, 1.0f);

	turtle = new Turtle;
	turtle->Generate(m_Direct3D->GetDevice(),m_Direct3D->GetDeviceContext(), "fern");

	return true;
}

bool Graphics::Frame()
{
	Render();
	return true;
}

bool Graphics::Render()
{
	XMMATRIX world_matrix, view_matrix, projection_matrix;

	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_camera->Render();

	m_Direct3D->GetWorldMatrix(world_matrix);
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
	


	XMMATRIX move_mat, rot_mat, scale_mat;

	for (int i = 0; i < turtle->GetModelList().size(); i++)
	{
		Model* model = turtle->GetModelList()[i];

		model->Render(m_Direct3D->GetDeviceContext());

		world_matrix = model->GetTransform() * cam_trans * cam_rot;

		m_default_shader->Render(m_Direct3D->GetDeviceContext(), model->GetIndexCount(), world_matrix, view_matrix, projection_matrix,
			model->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), model->GetBlendAmount());

		m_Direct3D->GetWorldMatrix(world_matrix);
	}
	m_Direct3D->EndScene();

	return true;
}