#include "MyRigidBody.h"
#include "MyOctant.h"
using namespace Simplex;

//Variables

//Allocation
void Simplex::MyOctant::Init(void)
{
}
void Simplex::MyOctant::Swap(MyOctant & other)
{
}
void Simplex::MyOctant::Release(void)
{
}
void Simplex::MyOctant::ConstructList(void)
{
}

//Accessors
float Simplex::MyOctant::GetSize(void) { return m_fSize; }
vector3 Simplex::MyOctant::GetCenter(void) { return m_v3Center; }
vector3 Simplex::MyOctant::GetMin(void) { return m_v3Min; }
vector3 Simplex::MyOctant::GetMax(void) { return m_v3Max; }
MyOctant* Simplex::MyOctant::GetChild(uint a_uChild) { return m_pChild[a_uChild]; }
MyOctant* Simplex::MyOctant::GetParent(void) { return m_pParent; }

//The Big 3
Simplex::MyOctant::MyOctant(uint a_uMaxLevel, uint a_uIdealEntityCount)
{
	Init();

	m_uOctantCount = 0;

	m_uMaxLevel = a_uMaxLevel;
	m_uIdealEntityCount = a_uIdealEntityCount;
	
}
Simplex::MyOctant::MyOctant(vector3 a_v3Center, float a_fSize)
{
}
Simplex::MyOctant::MyOctant(MyOctant const & other)
{
	m_uID = other.m_uID;
	m_uLevel = other.m_uLevel;
	m_uChildren = other.m_uChildren;
	m_fSize = other.m_fSize;
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
	return false;
}
void Simplex::MyOctant::Display(uint a_uIndex, vector3 a_v3Color)
{
}
void Simplex::MyOctant::Display(vector3 a_v3Color)
{
}
void Simplex::MyOctant::DisplayLeafs(vector3 a_v3Color)
{
}
void Simplex::MyOctant::ClearEntityList(void)
{
}
void Simplex::MyOctant::Subdivide(uint a_uRBIndex)
{
}
bool Simplex::MyOctant::IsLeaf(void)
{
	return false;
}
bool Simplex::MyOctant::ContainsMoreThan(uint a_uEntities)
{
	return false;
}
void Simplex::MyOctant::KillBranches(void)
{
}
void Simplex::MyOctant::ConstructTree(uint a_uMaxLevel)
{
}
void Simplex::MyOctant::AssignIDtoEntity(void)
{
}
uint Simplex::MyOctant::GetOctantCount(void)
{
	return uint();
}
