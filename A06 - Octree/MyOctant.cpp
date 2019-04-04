#include "MyRigidBody.h"
#include "MyOctant.h"
using namespace Simplex;
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
//Accessors
uint Simplex::MyOctant::GetID(void) { return m_uID; }
void Simplex::MyOctant::SetID(uint a_uID) { m_uID = a_uID; }
uint Simplex::MyOctant::GetLevel(void) { return m_uLevel; }
void Simplex::MyOctant::SetLevel(uint a_uLevel) { m_uLevel = a_uLevel; }
uint Simplex::MyOctant::GetChildren(void) { return m_uChildren; }
void Simplex::MyOctant::SetChildren(uint a_uChildren) { m_uChildren = a_uChildren; }
float Simplex::MyOctant::GetSize(void) { return m_fSize; }
void Simplex::MyOctant::SetSize(float a_fSize) { m_fSize = a_fSize; }
vector3 Simplex::MyOctant::GetCenter(void) { return m_v3Center; }
vector3 Simplex::MyOctant::GetMin(void) { return m_v3Min; }
vector3 Simplex::MyOctant::GetMax(void) { return m_v3Max; }
//The Big 3
Simplex::MyOctant::MyOctant(uint a_uLevels, uint a_EntityCount)
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
	m_EntityList = other.m_EntityList;
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