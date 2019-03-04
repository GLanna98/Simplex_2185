#include "MyMesh.h"
void MyMesh::Init(void)
{
	m_bBinded = false;
	m_uVertexCount = 0;

	m_VAO = 0;
	m_VBO = 0;

	m_pShaderMngr = ShaderManager::GetInstance();
}
void MyMesh::Release(void)
{
	m_pShaderMngr = nullptr;

	if (m_VBO > 0)
		glDeleteBuffers(1, &m_VBO);

	if (m_VAO > 0)
		glDeleteVertexArrays(1, &m_VAO);

	m_lVertex.clear();
	m_lVertexPos.clear();
	m_lVertexCol.clear();
}
MyMesh::MyMesh()
{
	Init();
}
MyMesh::~MyMesh() { Release(); }
MyMesh::MyMesh(MyMesh& other)
{
	m_bBinded = other.m_bBinded;

	m_pShaderMngr = other.m_pShaderMngr;

	m_uVertexCount = other.m_uVertexCount;

	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}
MyMesh& MyMesh::operator=(MyMesh& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyMesh temp(other);
		Swap(temp);
	}
	return *this;
}
void MyMesh::Swap(MyMesh& other)
{
	std::swap(m_bBinded, other.m_bBinded);
	std::swap(m_uVertexCount, other.m_uVertexCount);

	std::swap(m_VAO, other.m_VAO);
	std::swap(m_VBO, other.m_VBO);

	std::swap(m_lVertex, other.m_lVertex);
	std::swap(m_lVertexPos, other.m_lVertexPos);
	std::swap(m_lVertexCol, other.m_lVertexCol);

	std::swap(m_pShaderMngr, other.m_pShaderMngr);
}
void MyMesh::CompleteMesh(vector3 a_v3Color)
{
	uint uColorCount = m_lVertexCol.size();
	for (uint i = uColorCount; i < m_uVertexCount; ++i)
	{
		m_lVertexCol.push_back(a_v3Color);
	}
}
void MyMesh::AddVertexPosition(vector3 a_v3Input)
{
	m_lVertexPos.push_back(a_v3Input);
	m_uVertexCount = m_lVertexPos.size();
}
void MyMesh::AddVertexColor(vector3 a_v3Input)
{
	m_lVertexCol.push_back(a_v3Input);
}
void MyMesh::CompileOpenGL3X(void)
{
	if (m_bBinded)
		return;

	if (m_uVertexCount == 0)
		return;

	CompleteMesh();

	for (uint i = 0; i < m_uVertexCount; i++)
	{
		//Position
		m_lVertex.push_back(m_lVertexPos[i]);
		//Color
		m_lVertex.push_back(m_lVertexCol[i]);
	}
	glGenVertexArrays(1, &m_VAO);//Generate vertex array object
	glGenBuffers(1, &m_VBO);//Generate Vertex Buffered Object

	glBindVertexArray(m_VAO);//Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, m_uVertexCount * 2 * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);//Generate space for the VBO

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)(1 * sizeof(vector3)));

	m_bBinded = true;

	glBindVertexArray(0); // Unbind VAO
}
void MyMesh::Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mModel)
{
	// Use the buffer and shader
	GLuint nShader = m_pShaderMngr->GetShaderID("Basic");
	glUseProgram(nShader); 

	//Bind the VAO of this object
	glBindVertexArray(m_VAO);

	// Get the GPU variables by their name and hook them to CPU variables
	GLuint MVP = glGetUniformLocation(nShader, "MVP");
	GLuint wire = glGetUniformLocation(nShader, "wire");

	//Final Projection of the Camera
	matrix4 m4MVP = a_mProjection * a_mView * a_mModel;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4MVP));
	
	//Solid
	glUniform3f(wire, -1.0f, -1.0f, -1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);  

	//Wire
	glUniform3f(wire, 1.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1.f, -1.f);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);
	glDisable(GL_POLYGON_OFFSET_LINE);

	glBindVertexArray(0);// Unbind VAO so it does not get in the way of other objects
}
void MyMesh::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft)
{
	//C
	//| \
	//A--B
	//This will make the triangle A->B->C 
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
}
void MyMesh::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	//C--D
	//|  |
	//A--B
	//This will make the triangle A->B->C and then the triangle C->B->D
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyMesh::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = a_fSize * 0.5f;
	//3--2
	//|  |
	//0--1

	vector3 point0(-fValue,-fValue, fValue); //0
	vector3 point1( fValue,-fValue, fValue); //1
	vector3 point2( fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue,-fValue,-fValue); //4
	vector3 point5( fValue,-fValue,-fValue); //5
	vector3 point6( fValue, fValue,-fValue); //6
	vector3 point7(-fValue, fValue,-fValue); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCuboid(vector3 a_v3Dimensions, vector3 a_v3Color)
{
	Release();
	Init();

	vector3 v3Value = a_v3Dimensions * 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-v3Value.x, -v3Value.y, v3Value.z); //0
	vector3 point1(v3Value.x, -v3Value.y, v3Value.z); //1
	vector3 point2(v3Value.x, v3Value.y, v3Value.z); //2
	vector3 point3(-v3Value.x, v3Value.y, v3Value.z); //3

	vector3 point4(-v3Value.x, -v3Value.y, -v3Value.z); //4
	vector3 point5(v3Value.x, -v3Value.y, -v3Value.z); //5
	vector3 point6(v3Value.x, v3Value.y, -v3Value.z); //6
	vector3 point7(-v3Value.x, v3Value.y, -v3Value.z); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// My Code
	std::vector<vector3> tempList;

	for (size_t i = 0; i < a_nSubdivisions; i++)
	{
		float angle = 2.0f*PI / a_nSubdivisions;
		float xPos = (cos(angle*i)*a_fRadius);
		float zPos = (sin(angle*i)*a_fRadius);
		tempList.push_back(vector3(xPos, 0.0f - (a_fHeight/2), zPos));
	}

	for (size_t i = 1; i < tempList.size(); i++)
	{
		AddTri(tempList[i - 1], tempList[i], vector3(0.0f, 0.0f - (a_fHeight / 2), 0.0f));
		AddTri(tempList[i - 1],vector3(0.0f, 0.0f + (a_fHeight / 2), 0.0f), tempList[i]);
	}
	AddTri(tempList[tempList.size()-1], tempList[0], vector3(0.0f, 0.0f - (a_fHeight / 2), 0.0f));
	AddTri(tempList[tempList.size()-1], vector3(0.0f, 0.0f + (a_fHeight / 2), 0.0f), tempList[0]);

	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	std::vector<vector3> tempList1;
	std::vector<vector3> tempList2;

	for (size_t i = 0; i < a_nSubdivisions; i++)
	{
		float angle = 2.0f*PI / a_nSubdivisions;
		float xPos = (cos(angle*i)*a_fRadius);
		float zPos = (sin(angle*i)*a_fRadius);
		tempList1.push_back(vector3(xPos, 0.0f - (a_fHeight / 2), zPos));
		tempList2.push_back(vector3(xPos, 0.0f + (a_fHeight / 2), zPos));
	}

	for (size_t i = 1; i < tempList1.size(); i++)
	{
		AddTri(tempList1[i - 1], tempList1[i], vector3(0.0f, 0.0f - (a_fHeight / 2), 0.0f));
		AddTri(tempList2[i], tempList2[i - 1], vector3(0.0f, 0.0f + (a_fHeight / 2), 0.0f));
		AddQuad(tempList2[i - 1], tempList2[i], tempList1[i - 1], tempList1[i]);
	}
	AddTri(tempList1[tempList1.size() - 1], tempList1[0], vector3(0.0f, 0.0f - (a_fHeight / 2), 0.0f));
	AddTri(tempList2[0], tempList2[tempList2.size() - 1], vector3(0.0f, 0.0f + (a_fHeight / 2), 0.0f));
	AddQuad(tempList2[tempList2.size() - 1], tempList2[0], tempList1[tempList1.size() - 1], tempList1[0]);
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	std::vector<vector3> tempList1; //outside top
	std::vector<vector3> tempList2; //outside bottom
	std::vector<vector3> tempList3; //inside top
	std::vector<vector3> tempList4; //inside bottom

	for (size_t i = 0; i < a_nSubdivisions; i++)
	{
		float angle = 2.0f*PI / a_nSubdivisions;
		float xPosO = (cos(angle*i)*a_fOuterRadius);
		float zPosO = (sin(angle*i)*a_fOuterRadius);
		float xPosI = (cos(angle*i)*a_fInnerRadius);
		float zPosI = (sin(angle*i)*a_fInnerRadius);
		float yTop = 0.0f + (a_fHeight / 2);
		float yBot = 0.0f - (a_fHeight / 2);

		tempList1.push_back(vector3(xPosO, yTop, zPosO));
		tempList2.push_back(vector3(xPosO, yBot, zPosO));
		tempList3.push_back(vector3(xPosI, yTop, zPosI));
		tempList4.push_back(vector3(xPosI, yBot, zPosI));
	}

	for (size_t i = 0; i < tempList1.size() - 1; i++)
	{
		AddQuad(tempList1[i], tempList1[i + 1], tempList2[i], tempList2[i + 1]);
		AddQuad(tempList2[i], tempList2[i + 1], tempList4[i], tempList4[i + 1]);
		AddQuad(tempList4[i], tempList4[i + 1], tempList3[i], tempList3[i + 1]);
		AddQuad(tempList3[i], tempList3[i + 1], tempList1[i], tempList1[i + 1]);
	}
	AddQuad(tempList1[tempList1.size() - 1], tempList1[0], tempList2[tempList1.size() - 1], tempList2[0]);
	AddQuad(tempList2[tempList1.size() - 1], tempList2[0], tempList4[tempList1.size() - 1], tempList4[0]);
	AddQuad(tempList4[tempList1.size() - 1], tempList4[0], tempList3[tempList1.size() - 1], tempList3[0]);
	AddQuad(tempList3[tempList1.size() - 1], tempList3[0], tempList1[tempList1.size() - 1], tempList1[0]);

	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 360)
		a_nSubdivisionsA = 360;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 360)
		a_nSubdivisionsB = 360;

	Release();
	Init();

	// Replace this with your code
	
	//Ok, so after that whole sphere thing, we can make this in an even more convoluted way.
	//A torus is basically a circle where instead of vertices, we have other circles that connect to each other like cylinders
	//So first, let's create a reference circle

	std::vector<vector3> refCircle;

	//And again, make a vector of vector3 vectors. You'll get why in a bit.
	std::vector<std::vector<vector3>> torus;

	//Also, let's make sure we know what radii we are using
	float loopRadius = a_fOuterRadius - a_fInnerRadius / 2;
	float torusRadius = a_fInnerRadius + loopRadius;

	for (size_t i = 0; i < a_nSubdivisionsA; i++)
	{
		float innerAngle = (2 * PI / a_nSubdivisionsA) * i;
		float xPos = torusRadius * cos(innerAngle);
		float yPos = 0.0f;
		float zPos = torusRadius * sin(innerAngle);
		refCircle.push_back(vector3(xPos, yPos, zPos));

		//Now, all I have to do is create a vertical circle centered on each of the vertices of the refCircle.
		//However, I need to calculate the angles so that the outer circles are placed correctly.

		//so first let's start making the outer circles
		std::vector<vector3> outerCircle;
		for (size_t i = 0; i < a_nSubdivisionsB; i++)
		{
			float outerAngle = (2 * PI / a_nSubdivisionsB) * i;

			//So this is what the circles would look like if i didn't have to rotate them based on their position on the torus
			float fakeX = loopRadius * sin(outerAngle) + xPos;
			float fakeY = loopRadius * cos(outerAngle) + yPos;
			float fakeZ = 0.0f + zPos;

			//But, I need to rotate these circles on the y-axis, so we'll do some math to calculate the real x and z positions
			float trueX = fakeX * cos(innerAngle) + fakeZ * sin(innerAngle);
			float trueY = loopRadius * cos(outerAngle);
			float trueZ = fakeZ * cos(innerAngle) - fakeX * sin(innerAngle);

			outerCircle.push_back(vector3(trueX, yPos, trueZ));
		}

		//push the created circles to the torus vector vector
		torus.push_back(outerCircle);
	}

	//Now we should have all the vertices in place, so let's just connect them
	for (size_t i = 0; i < a_nSubdivisionsA-2; i++)
	{
		std::vector<vector3> currentCircle = torus[i];
		std::vector<vector3> nextCircle = torus[i + 1];
		for (size_t j = 0; j < a_nSubdivisionsB-2; j++)
		{
			AddQuad(currentCircle[j], nextCircle[j], currentCircle[j + 1], nextCircle[j + 1]);
		}
		AddQuad(currentCircle[a_nSubdivisionsB-1], nextCircle[a_nSubdivisionsB - 1], currentCircle[0], nextCircle[0]);
	}
	
	//this one didn't work. Oh well.

	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 4)
	{
		GenerateCube(a_fRadius * 2.0f, a_v3Color);
		return;
	}

	Release();
	Init();

	// Replace this with your code
	
	//Ok, let's think about this. A sphere would basically be a pile of circles up to down connected together.
	//so, if i create a big vertical circle with the nearest even number of subdivisions, I might be able to figure out the radius for each of the circles in the pile
	int vSubdivisions = round(a_nSubdivisions / 2.0f) * 2;

	//Nice, so now I'll just create a vertical circle and see what's the radii of each of the paralel vertices
	std::vector<vector3> tempList;

	for (size_t i = 0; i < vSubdivisions; i++)
	{
		float angle = (2 * PI / vSubdivisions) * i;
		float xPos = a_fRadius * sin(angle);
		float yPos = a_fRadius * cos(angle);
		float zPos = 0.0f;
		tempList.push_back(vector3(xPos, yPos, zPos));
	}

	//Ok. So now we have a circle. Now, each of the subdivisions except for the middle two have somewhere to match with.
	//The one after the first one is the same height as the last one, the one after it is next to the second to last...
	//So how do I figure this out? if it has 8 vertical subdivisions, the matches would be like this:
	/*
	    0
	  1   7
	 2     6
	  3   5
	    4
	*/
	//So the matching vectors are i and vSubdivisions-i. If vSubdivisions-i == vSubdivisions, then that one has no pair.
	//Now let's get crazy for a moment, and create a vector of vector3 vectors. That make sense? No? Cool.
	std::vector<std::vector<vector3>> combinedCircles;

	for (size_t i = 1; i < vSubdivisions/2; i++)
	{
		//So let's get the radius of the smaller circle
		vector3 v1 = tempList[i];
		vector3 v2 = tempList[vSubdivisions-i];
		float radius = sqrt((pow((v1[0] - v2[0]), 2) + pow((v1[1] - v2[1]), 2) + pow((v1[2] - v2[2]), 2)))/2;

		//now that we have this, let's create a vector to hold each circle
		std::vector<vector3> innerCircle;
		for (size_t i = 0; i < a_nSubdivisions; i++)
		{
			float angle = (2 * PI / a_nSubdivisions) * i;
			float xPos = radius * cos(angle);
			float yPos = v1[1];
			float zPos = radius * sin(angle);
			innerCircle.push_back(vector3(xPos, yPos, zPos));
		}
		combinedCircles.push_back(innerCircle);
	}

	//Alright, so that got confusing, but basically we now have a vector that hold the different sub-circles that are used to make the larger circle
	//Now, all we have to do is connect the circles together and connect the top and bottom circles to tempList[0] and tempList[vSubdivisions/2]

	std::vector<vector3> topLoop = combinedCircles[0];
	std::vector<vector3> bottomLoop = combinedCircles[combinedCircles.size()-1];
	for (size_t i = 0; i < topLoop.size()-1; i++)
	{
		AddTri(topLoop[i + 1], topLoop[i], tempList[0]);
	}
	AddTri(topLoop[0], topLoop[topLoop.size()-1], tempList[0]);

	for (size_t i = 0; i < bottomLoop.size()-1; i++)
	{
		AddTri(bottomLoop[i + 1], tempList[vSubdivisions / 2], bottomLoop[i]);
	}
	AddTri(bottomLoop[0], tempList[vSubdivisions / 2], bottomLoop[bottomLoop.size() - 1]);

	for (size_t i = 0; i < combinedCircles.size()-1; i++)
	{
		std::vector<vector3> currentLoop = combinedCircles[i];
		std::vector<vector3> nextLoop = combinedCircles[i + 1];

		for (size_t j = 0; j < currentLoop.size()-1; j++)
		{
			AddQuad(currentLoop[j], currentLoop[j + 1], nextLoop[j], nextLoop[j + 1]);
		}
		AddQuad(currentLoop[currentLoop.size() - 1], currentLoop[0], nextLoop[currentLoop.size() - 1], nextLoop[0]);
	}

	//Holy crap this worked. I'm so sorry for creating this

	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}