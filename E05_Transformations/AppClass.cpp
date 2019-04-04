#include "AppClass.h"
void Application::InitVariables(void)
{
	//Make MyMesh objects
	for (size_t i = 0; i < 46; i++)
	{
		//create basic cube on center of screen
		MyMesh* m_pMesh = new MyMesh();
		m_pMesh->GenerateCube(1.0f, C_BLACK);
		spaceInvader.push_back(m_pMesh);
	}
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	//Control movement
	static float value = 0.0f;
	value += 0.01f;

#pragma region A lot of cubes being generated
	//Translate all 46 cubes to their respective "Space invader" position, with the movement constant being added to each
	spaceInvader[0]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-3.0f + value, 7.0f, 0.0f)));
	spaceInvader[1]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(3.0f + value, 7.0f, 0.0f)));
	spaceInvader[2]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-2.0f + value, 6.0f, 0.0f)));
	spaceInvader[3]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(2.0f + value, 6.0f, 0.0f)));
	spaceInvader[4]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-3.0f + value, 5.0f, 0.0f)));
	spaceInvader[5]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-2.0f + value, 5.0f, 0.0f)));
	spaceInvader[6]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-1.0f + value, 5.0f, 0.0f)));
	spaceInvader[7]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(0.0f + value, 5.0f, 0.0f)));
	spaceInvader[8]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(1.0f + value, 5.0f, 0.0f)));
	spaceInvader[9]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(2.0f + value, 5.0f, 0.0f)));
	spaceInvader[10]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(3.0f + value, 5.0f, 0.0f)));
	spaceInvader[11]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-4.0f + value, 4.0f, 0.0f)));
	spaceInvader[12]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-3.0f + value, 4.0f, 0.0f)));
	spaceInvader[13]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-1.0f + value, 4.0f, 0.0f)));
	spaceInvader[14]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(0.0f + value, 4.0f, 0.0f)));
	spaceInvader[15]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(1.0f + value, 4.0f, 0.0f)));
	spaceInvader[16]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(3.0f + value, 4.0f, 0.0f)));
	spaceInvader[17]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(4.0f + value, 4.0f, 0.0f)));
	spaceInvader[18]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-5.0f + value, 3.0f, 0.0f)));
	spaceInvader[19]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-4.0f + value, 3.0f, 0.0f)));
	spaceInvader[20]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-3.0f + value, 3.0f, 0.0f)));
	spaceInvader[21]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-2.0f + value, 3.0f, 0.0f)));
	spaceInvader[22]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-1.0f + value, 3.0f, 0.0f)));
	spaceInvader[23]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(0.0f + value, 3.0f, 0.0f)));
	spaceInvader[24]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(1.0f + value, 3.0f, 0.0f)));
	spaceInvader[25]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(2.0f + value, 3.0f, 0.0f)));
	spaceInvader[26]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(3.0f + value, 3.0f, 0.0f)));
	spaceInvader[27]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(4.0f + value, 3.0f, 0.0f)));
	spaceInvader[28]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(5.0f + value, 3.0f, 0.0f)));
	spaceInvader[29]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-5.0f + value, 2.0f, 0.0f)));
	spaceInvader[30]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-3.0f + value, 2.0f, 0.0f)));
	spaceInvader[31]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-2.0f + value, 2.0f, 0.0f)));
	spaceInvader[32]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-1.0f + value, 2.0f, 0.0f)));
	spaceInvader[33]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(0.0f + value, 2.0f, 0.0f)));
	spaceInvader[34]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(1.0f + value, 2.0f, 0.0f)));
	spaceInvader[35]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(2.0f + value, 2.0f, 0.0f)));
	spaceInvader[36]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(3.0f + value, 2.0f, 0.0f)));
	spaceInvader[37]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(5.0f + value, 2.0f, 0.0f)));
	spaceInvader[38]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-5.0f + value, 1.0f, 0.0f)));
	spaceInvader[39]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-3.0f + value, 1.0f, 0.0f)));
	spaceInvader[40]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(3.0f + value, 1.0f, 0.0f)));
	spaceInvader[41]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(5.0f + value, 1.0f, 0.0f)));
	spaceInvader[42]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-2.0f + value, 0.0f, 0.0f)));
	spaceInvader[43]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-1.0f + value, 0.0f, 0.0f)));
	spaceInvader[44]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(1.0f + value, 0.0f, 0.0f)));
	spaceInvader[45]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(2.0f + value, 0.0f, 0.0f)));
#pragma endregion	

	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//delete all mesh objects on the space invader
	for (std::vector<MyMesh*>::iterator it = spaceInvader.begin(); it != spaceInvader.end(); ++it)
	{
		delete *it;
		*it = nullptr;
		SafeDelete(*it);
	}
	//release GUI
	ShutdownGUI();
}