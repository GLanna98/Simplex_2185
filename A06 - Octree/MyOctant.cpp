/*----------------------------------------------
Programmer: Gabriel L'Huillier Lanna (gll1460@rit.edu)
Date: March-April 2019
----------------------------------------------*/
#include "MyRigidBody.h"
#include "MyOctant.h"
using namespace Simplex;

//Initualize Static Variables
uint MyOctant::ms_uIdealEntityCount = 5;
uint MyOctant::ms_uMaxLevel = 3;
uint MyOctant::ms_uOctantCount = 0;

//Allocation
void Simplex::MyOctant::Init(void)
{
	//Initialize non static variables
	m_uID = 0;
	m_uLevel = 0;
	m_uChildren = 0;

	m_v3HalfWidth = vector3(0.0f);

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	m_v3Center = vector3(0.0f);
	m_v3Min = vector3(0.0f);
	m_v3Max = vector3(0.0f);

	m_pParent = nullptr;
	for (size_t i = 0; i < m_uChildren; i++)
	{
		m_pChild[i] = nullptr;
	}
}
void Simplex::MyOctant::Swap(MyOctant & other)
{
	//swap non static variables from another octant
	std::swap(m_uID, other.m_uID);
	std::swap(m_uLevel, other.m_uLevel);
	std::swap(m_uChildren, other.m_uChildren);

	std::swap(m_v3HalfWidth, other.m_v3HalfWidth);

	std::swap(m_pMeshMngr, other.m_pMeshMngr);
	std::swap(m_pEntityMngr, other.m_pEntityMngr);

	std::swap(m_v3Center, other.m_v3Center);
	std::swap(m_v3Min, other.m_v3Min);
	std::swap(m_v3Max, other.m_v3Max);

	std::swap(m_pParent, other.m_pParent);

	for (size_t i = 0; i < m_uChildren; i++)
	{
		std::swap(m_pChild[i], other.m_pChild[i]);
	}

	std::swap(m_lEntityList, other.m_lEntityList);

	std::swap(m_pRoot, other.m_pRoot);
	std::swap(m_lChild, other.m_lChild);
}
void Simplex::MyOctant::Release(void)
{
	//Call Kill branches on the root octant
	if (m_uID == 0)
	{
		KillBranches();
	}

	//Zero out the other variables
	ms_uIdealEntityCount = 0;
	ms_uMaxLevel = 0;
	ms_uOctantCount = 0;
	m_uLevel = 0;
	m_uChildren = 0;
	m_v3HalfWidth = vector3(0.0f);
	m_v3Center = vector3(0.0f);
	m_v3Min = vector3(0.0f);
	m_v3Max = vector3(0.0f);
	m_pEntityMngr = nullptr;
	m_pMeshMngr = nullptr;
}
void Simplex::MyOctant::ConstructList(void)
{
	//Recursively go through all octants under this one, adding the ones with children to the list 
	for (size_t i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->ConstructList();
	}

	if (m_lEntityList.size() > 0)
	{
		m_lChild.push_back(this);
	}
}

//Accessors
vector3 Simplex::MyOctant::GetHalfWidth(void) { return m_v3HalfWidth; }
vector3 Simplex::MyOctant::GetCenter(void) { return m_v3Center; }
vector3 Simplex::MyOctant::GetMin(void) { return m_v3Min; }
vector3 Simplex::MyOctant::GetMax(void) { return m_v3Max; }
MyOctant* Simplex::MyOctant::GetChild(uint a_uChild) { return m_pChild[a_uChild]; }
MyOctant* Simplex::MyOctant::GetParent(void) { return m_pParent; }

//The Big 3

//For the root only
Simplex::MyOctant::MyOctant(uint a_uMaxLevel, uint a_uIdealEntityCount)
{
	//Increase the oct count as this has been created
	ms_uOctantCount++;

	Init();
	//Use the properties to set the static variables
	ms_uMaxLevel = a_uMaxLevel;
	ms_uIdealEntityCount = a_uIdealEntityCount;

	//OK, so now I need to set the m_fSize, m_v3Center, m_v3Max and m_v3Min of the octant to encompass all the objects.
	//Now, I have set up a sphere radius member variable on the AppClass, so the m_fSize should just be the radius, plus the radius of the minecraft cube, times two
	//This might not be fully precise though, since the entities could be scaled to go way over the usual size, right? 
	//So maybe I should go through all entities, compare each of their global maxes and mins to get the absolute min and max, and then use that to calculate the variables
	//Ok, so first lets loop through all entities adding their max vector to a list
	/*std::vector<vector3> lMax;
	for (size_t i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
	{
		MyEntity* pCurrentEntity = m_pEntityMngr->GetEntity(i);
		MyRigidBody* pCurrentRB = pCurrentEntity->GetRigidBody();
		lMax.push_back(pCurrentRB->GetMaxGlobal());
	}*/
	//Now what do I do with this?
	//Whatever I do, its going to take for ever to sort out which is the furthest vector.
	//RigidBodies have all these properties I want anyways. i could just create a rigidbody that uses all the maxes and a rigid body that uses all the mins and use this information
	//Ok, so I'm not gonna leave my code here, but after doing that I realized the center and size variables were going to be a pain to find like this.
	//I think the best option is to actually do the same thing as above but with a single rigid body that takes all max and min vectors
	std::vector<vector3> lPoints;
	for (size_t i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
	{
		MyEntity* pCurrentEntity = m_pEntityMngr->GetEntity(i);
		MyRigidBody* pCurrentRB = pCurrentEntity->GetRigidBody();
		lPoints.push_back(pCurrentRB->GetMaxGlobal());
		lPoints.push_back(pCurrentRB->GetMinGlobal());
	}
	MyRigidBody* thisRB = new MyRigidBody(lPoints);
	
	//Cool, now lets just assign these to the variables
	m_v3Max = thisRB->GetMaxGlobal();
	m_v3Min = thisRB->GetMinGlobal();
	m_v3Center = thisRB->GetCenterGlobal();

	//I have also decided I don't like fSize as a concept, so Ill just use v3HalfWidth as a replacement
	m_v3HalfWidth = thisRB->GetHalfWidth();

	//delete the Rigid body
	SafeDelete(thisRB);

	//This was more complicated then I thought, but now its just a case of calling the construct tree method, which is gonna suck to figure out
	ConstructTree();
}
Simplex::MyOctant::MyOctant(vector3 a_v3Center, vector3 a_v3HalfWidth)
{
	//Increase the oct count as this has been created
	ms_uOctantCount++;

	Init();

	//Pass the properties into the variables and use them to calculate the other vectors
	m_v3Center = a_v3Center;
	m_v3HalfWidth = a_v3HalfWidth;
	m_v3Max = m_v3Center + m_v3HalfWidth;
	m_v3Min = m_v3Center - m_v3HalfWidth;
}
Simplex::MyOctant::MyOctant(MyOctant const & other)
{
	m_uID = other.m_uID;
	m_uLevel = other.m_uLevel;
	m_uChildren = other.m_uChildren;
	m_v3HalfWidth = other.m_v3HalfWidth;
	m_pMeshMngr = other.m_pMeshMngr;
	m_pEntityMngr = other.m_pEntityMngr;
	m_v3Center = other.m_v3Center;
	m_v3Min = other.m_v3Min;
	m_v3Max = other.m_v3Max;
	m_pParent = other.m_pParent;
	*m_pChild = *other.m_pChild;
	m_lEntityList = other.m_lEntityList;
	m_pRoot = other.m_pRoot;
	m_lChild = other.m_lChild;
}
MyOctant & Simplex::MyOctant::operator=(MyOctant const & other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyOctant temp(other);
		Swap(temp);
	}
	return *this;
}
Simplex::MyOctant::~MyOctant(void) { Release(); }

//Other Methods
bool Simplex::MyOctant::IsColliding(uint a_uRBIndex)
{
	//Get the entity with this index from the entity manager
	MyEntity* pThisEntity = m_pEntityMngr->GetEntity(a_uRBIndex);
	
	//Get the max and min of the entity
	vector3 v3ThisMax = pThisEntity->GetRigidBody()->GetMaxGlobal();
	vector3 v3ThisMin = pThisEntity->GetRigidBody()->GetMinGlobal();

	//return whether or not there is an AABB collision
	return (m_v3Min.x <= v3ThisMax.x && m_v3Max.x >= v3ThisMin.x) &&
		   (m_v3Min.y <= v3ThisMax.y && m_v3Max.y >= v3ThisMin.y) &&
		   (m_v3Min.z <= v3ThisMax.z && m_v3Max.z >= v3ThisMin.z);
}
void Simplex::MyOctant::Display(uint a_uIndex, vector3 a_v3Color)
{
	//If this is the specified octant, draw it. If not, recursively go through all the branches of this octant calling this method
	if (m_uID == a_uIndex)
	{
		m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(m_v3HalfWidth * 2.0f), a_v3Color);
	}
	else
	{
		if (m_uChildren > 0)
		{
			for (size_t i = 0; i < m_uChildren; i++)
			{
				m_pChild[i]->Display(a_uIndex, a_v3Color);
			}
		}
	}
}
void Simplex::MyOctant::Display(vector3 a_v3Color)
{
	//Draw this and then recursively go through the children drawing them
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(m_v3HalfWidth * 2.0f), a_v3Color);
	if (m_uChildren > 0)
	{
		for (size_t i = 0; i < m_uChildren; i++)
		{
			m_pChild[i]->Display(a_v3Color);
		}
	}
}
void Simplex::MyOctant::DisplayLeafs(vector3 a_v3Color)
{
	//Ok, so I don't really get this method. Since its asking me to display the non empty leafs, I assume its just the same as display, but only for leafs
	if (IsLeaf())
	{
		m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(m_v3HalfWidth * 2.0f), a_v3Color);
	}
	else
	{
		for (size_t i = 0; i < m_uChildren; i++)
		{
			m_pChild[i]->DisplayLeafs(a_v3Color);
		}
	}
}
void Simplex::MyOctant::ClearEntityList(void)
{
	//recursively get to all the leafs and then go clearing the entity lists
	if (m_uChildren > 0)
	{
		for (size_t i = 0; i < m_uChildren; i++)
		{
			m_pChild[i]->ClearEntityList();
		}
	}
	m_lEntityList.clear();
}
void Simplex::MyOctant::Subdivide(void)
{
	//First, check if this can and needs to be subdivided
	if (m_uLevel >= ms_uMaxLevel || !ContainsMoreThan(ms_uIdealEntityCount))
	{
		return;
	}
	else
	{
		//There are now children, so set the count to 8
		m_uChildren = 8;

		//The halfwidths of the children are just going to be the halfwidth of this divided by two
		vector3 v3ChildHalfWidth = m_v3HalfWidth/2.0f;

		//Ok, now its time to calculate the centers. Ill create a to hold this and then do calculations
		vector3 v3ChildCenter;

		//Front, Top, Right
		v3ChildCenter = m_v3Center + v3ChildHalfWidth;
		m_pChild[0] = new MyOctant(v3ChildCenter, v3ChildHalfWidth);
		m_pChild[0]->m_uID = this->m_uID + 1;

		//Front, Top, Left
		v3ChildCenter = m_v3Center + vector3(-v3ChildHalfWidth.x, v3ChildHalfWidth.y, v3ChildHalfWidth.z);
		m_pChild[1] = new MyOctant(v3ChildCenter, v3ChildHalfWidth);
		m_pChild[1]->m_uID = this->m_uID + 2;

		//Front, Bottom, Right
		v3ChildCenter = m_v3Center + vector3(v3ChildHalfWidth.x, -v3ChildHalfWidth.y, v3ChildHalfWidth.z);
		m_pChild[2] = new MyOctant(v3ChildCenter, v3ChildHalfWidth);
		m_pChild[2]->m_uID = this->m_uID + 3;

		//Front, Bottom, Left
		v3ChildCenter = m_v3Center + vector3(-v3ChildHalfWidth.x, -v3ChildHalfWidth.y, v3ChildHalfWidth.z);
		m_pChild[3] = new MyOctant(v3ChildCenter, v3ChildHalfWidth);
		m_pChild[3]->m_uID = this->m_uID + 4;

		//Back, Top, Right
		v3ChildCenter = m_v3Center + vector3(v3ChildHalfWidth.x, v3ChildHalfWidth.y, -v3ChildHalfWidth.z);
		m_pChild[4] = new MyOctant(v3ChildCenter, v3ChildHalfWidth);
		m_pChild[4]->m_uID = this->m_uID + 5;

		//Back, Top, Left
		v3ChildCenter = m_v3Center + vector3(-v3ChildHalfWidth.x, v3ChildHalfWidth.y, -v3ChildHalfWidth.z);
		m_pChild[5] = new MyOctant(v3ChildCenter, v3ChildHalfWidth);
		m_pChild[5]->m_uID = this->m_uID + 6;

		//Back, Bottom, Right
		v3ChildCenter = m_v3Center + vector3(v3ChildHalfWidth.x, -v3ChildHalfWidth.y, -v3ChildHalfWidth.z);
		m_pChild[6] = new MyOctant(v3ChildCenter, v3ChildHalfWidth);
		m_pChild[6]->m_uID = this->m_uID + 7;

		//Back, Bottom, Left
		v3ChildCenter = m_v3Center + vector3(-v3ChildHalfWidth.x, -v3ChildHalfWidth.y, -v3ChildHalfWidth.z);
		m_pChild[7] = new MyOctant(v3ChildCenter, v3ChildHalfWidth);
		m_pChild[7]->m_uID = this->m_uID + 8;

		ClearEntityList();
		AssignIDtoEntity();

		//Ok good. Now the children are created. Now we call this method again on each of them
		for (size_t i = 0; i < m_uChildren; i++)
		{
			//Also, assign the correct values to the member variables here
			m_pChild[i]->m_pParent = this;
			m_pChild[i]->m_uLevel = this->m_uLevel + 1;
			m_pChild[i]->Subdivide();
		}
	}
}
bool Simplex::MyOctant::IsLeaf(void)
{
	return m_uChildren <= 0;
}
bool Simplex::MyOctant::ContainsMoreThan(uint a_uEntities)
{
	return m_lEntityList.size() > a_uEntities;
}
void Simplex::MyOctant::KillBranches(void)
{
	//Alright, so this one is complicated too. Obviously I need to start deleting from the leaves, all the way up to the branches. 
	//So I guess first I need to recursively call this method until I get to a leaf
	if (!IsLeaf())
	{
		for (size_t i = 0; i < m_uChildren; i++)
		{
			m_pChild[i]->KillBranches();
		}
	}
	//Now I should be on a leaf. So from here I should go by deleting the children
	//I just realized if this is a leaf, there is nothing to delete...
	//I guess its fine. If I add a check to see if there are any children, the leaves will be skipped, and I'll recursively delete the children of each octant
	//until I get to the root, which should then be the only octant remaining
	if (m_uChildren > 0)
	{
		for (size_t i = 0; i < m_uChildren; i++)
		{
			m_pChild[i]->m_pEntityMngr = nullptr;
			m_pChild[i]->m_pParent = nullptr;
			m_pChild[i]->m_pMeshMngr = nullptr;
			m_pChild[i] = nullptr;
			SafeDelete(m_pChild[i]);
		}
	}
}
void Simplex::MyOctant::ConstructTree(void)
{
	//First we make sure that the tree is empty and this is the root
	if (m_uID == 0 && ms_uOctantCount == 1)
	{
		//Then we reset the entities by clearing the entities and then adding them again
		ClearEntityList();
		AssignIDtoEntity();

		//Finally, we subdivide
		Subdivide();
	}
}
void Simplex::MyOctant::AssignIDtoEntity(void)
{
	//This should go to each leaf, so lets do recursion again
	if (!IsLeaf())
	{
		for (size_t i = 0; i < m_uChildren; i++)
		{
			m_pChild[i]->AssignIDtoEntity();
		};
	}
	else
	{
		//Now that we have all leaves, go through each entity check for collision and add them to the entity list
		int count = m_pEntityMngr->GetEntityCount();
		for (size_t i = 0; i < count; i++)
		{
			if (IsColliding(i))
			{
				m_lEntityList.push_back(i);
				//also, add this id as a dimension for each entity so that we can use these for collision checks
				//We also need to clear dimensions on each instance before this so that the dimensions of the first octant aren't applied to the subsequent ones
				m_pEntityMngr->ClearDimensionSet(i);
				m_pEntityMngr->AddDimension(i, m_uID);
			}
		}
	}
}
uint Simplex::MyOctant::GetOctantCount(void)
{
	return ms_uOctantCount;
}
