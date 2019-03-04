#include "AppClass.h"
void Application::InitVariables(void)
{
	//Change this to your name and email
	m_sProgrammer = "Gabriel Lanna - gll1460@rit.edu";
	
	//Set the position and target of the camera
	//(I'm at [0,0,10], looking at [0,0,0] and up is the positive Y axis)
	m_pCameraMngr->SetPositionTargetAndUpward(AXIS_Z * 20.0f, ZERO_V3, AXIS_Y);

	//if the light position is zero move it
	if (m_pLightMngr->GetPosition(1) == ZERO_V3)
		m_pLightMngr->SetPosition(vector3(0.0f, 0.0f, 3.0f));

	//if the background is cornflowerblue change it to black (its easier to see)
	if (vector3(m_v4ClearColor) == C_BLUE_CORNFLOWER)
	{
		m_v4ClearColor = vector4(ZERO_V3, 1.0f);
	}
	
	//if there are no segments create 7
	if(m_uOrbits < 1)
		m_uOrbits = 7;

	//Populate the vector to hold all the route counts for each path
	for (size_t i = 0; i < m_uOrbits; i++)
	{
		static uint route = 0;
		allRouteCounts.push_back(route);
	}

	float fSize = 1.0f; //initial size of orbits

	//creating a color using the spectrum 
	uint uColor = 650; //650 is Red
	//prevent division by 0
	float decrements = 250.0f / (m_uOrbits > 1? static_cast<float>(m_uOrbits - 1) : 1.0f); //decrement until you get to 400 (which is violet)
	/*
		This part will create the orbits, it start at 3 because that is the minimum subdivisions a torus can have
	*/
	uint uSides = 3; //start with the minimal 3 sides
	for (uint i = uSides; i < m_uOrbits + uSides; i++)
	{
		vector3 v3Color = WaveLengthToRGB(uColor); //calculate color based on wavelength
		m_shapeList.push_back(m_pMeshMngr->GenerateTorus(fSize, fSize - 0.1f, 3, i, v3Color)); //generate a custom torus and add it to the meshmanager

		//create a vector to hold all the vertices of the current torus
		std::vector<vector3> thisPath;
		for (size_t j = 0; j < i; j++)
		{
			float xPos = cos(((2 * PI) / i)*j) * (fSize - 0.05f);
			float yPos = sin(((2 * PI) / i)*j) * (fSize - 0.05f);
			thisPath.push_back(vector3(xPos, yPos, 0.0f));
		}
		allPaths.push_back(thisPath); //add this path to the list of all paths

		fSize += 0.5f; //increment the size for the next orbit
		uColor -= static_cast<uint>(decrements); //decrease the wavelength
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

	matrix4 m4View = m_pCameraMngr->GetViewMatrix(); //view Matrix
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix(); //Projection Matrix
	matrix4 m4Offset = IDENTITY_M4; //offset of the orbits, starts as the global coordinate system
	/*
		The following offset will orient the orbits as in the demo, start without it to make your life easier.
	*/
	//m4Offset = glm::rotate(IDENTITY_M4, 1.5708f, AXIS_Z);

	//Get a timer
	static float fTimer = 0;	//store the new timer
	static uint uClock = m_pSystem->GenClock(); //generate a new clock for that timer
	fTimer += m_pSystem->GetDeltaTime(uClock); //get the delta time for that timer

	//get the timer for the lerp
	float fPercentage = MapValue(fTimer, 0.0f, 1.0f, 0.0f, 1.0f);

	//get a vector to hold all of the lerp info for each sphere
	std::vector<vector3> allLerps;
	for (size_t i = 0; i < m_uOrbits; i++)
	{
		//set the start and end paths
		vector3 startPos = allPaths[i][allRouteCounts[i]];
		vector3 endPos;

		if (allRouteCounts[i] == allPaths[i].size() - 1)
		{
			endPos = allPaths[i][0];
		}
		else
		{
			endPos = allPaths[i][allRouteCounts[i] + 1];
		}

		//save the lerp
		allLerps.push_back(glm::lerp(startPos, endPos, fPercentage));

		//change the route counter if needed
		if (fPercentage >= 1.0f)
		{
			if (allRouteCounts[i] == allPaths[i].size() - 1)
			{
				allRouteCounts[i] = 0;
			}
			else
			{
				allRouteCounts[i] += 1;
			}
			fTimer = m_pSystem->GetDeltaTime(uClock);
		}
	}

	// draw a shapes
	for (uint i = 0; i < m_uOrbits; ++i)
	{
		m_pMeshMngr->AddMeshToRenderList(m_shapeList[i], glm::rotate(m4Offset, 1.5708f, AXIS_X));

		//calculate the current position
		vector3 v3CurrentPos = allLerps[i];
		matrix4 m4Model = glm::translate(m4Offset, v3CurrentPos);

		//draw spheres
		m_pMeshMngr->AddSphereToRenderList(m4Model * glm::scale(vector3(0.15)), C_WHITE);
	}

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
	//release GUI
	ShutdownGUI();
}