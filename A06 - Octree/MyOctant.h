#ifndef __MYOCTANT_H_
#define __MYOCTANT_H_

#include "MyEntityManager.h"

namespace Simplex
{

//System Class
class MyOctant
{
	static uint m_uOctantCount; //will store the number of octants instantiated
	static uint m_uMaxLevel; //will store the maximum level an octant can go to
	static uint m_uIdealEntityCount; //will tell how many ideal Entitites this object will contain

	uint m_uID = 0; //Will store the current ID for this octant
	uint m_uLevel = 0; //Will store the current level of the octant
	uint m_uChildren = 0; //Number of children on the octant (either 0 or 8)

	float m_fSize = 0.0f; //Size of the octant

	MeshManager* m_pMeshMngr = nullptr; //Mesh Manager Singleton
	MyEntityManager* m_pEntityMngr = nullptr; //Entity Nabager Singleton

	vector3 m_v3Center = vector3(0.0f); //Will store the center point of the octant
	vector3 m_v3Min = vector3(0.0f); //Will store the minimum vector of the octant
	vector3 m_v3Max = vector3(0.0f); //Will hold the maximum vector of the octant

	MyOctant* m_pParent = nullptr; //Will store the parent of the current octant
	MyOctant* m_pChild[8]; //Will store the children of the current octant

	std::vector<uint> m_EntityList; //List of Entitites under this octant (Index in Entity Manager)

	MyOctant* m_pRoot = nullptr; //Root octant
	std::vector<MyOctant*> m_lChild; //list of nodes that contain objects (this will be applied to root only)

public:
	/*
	Usage: Constructor
	Arguments: std::vector<vector3> a_pointList -> list of points to make the Rigid Body for
	Output: class object instance
	*/
	MyOctant(uint a_uLevels, uint a_EntityCount);
	/*
	Usage: Copy Constructor
	Arguments: class object to copy
	Output: class object instance
	*/
	MyOctant(MyOctant const& other);
	/*
	Usage: Copy Assignment Operator
	Arguments: class object to copy
	Output: ---
	*/
	MyOctant& operator=(MyOctant const& other);
	/*
	Usage: Destructor
	Arguments: ---
	Output: ---
	*/
	~MyOctant(void);
	/*
	Usage: Changes object contents for other object's
	Arguments: other -> object to swap content from
	Output: ---
	*/
	void Swap(MyOctant& other);

#pragma region Accessors
	/*
	Usage: Gets ID of octant
	Arguments: ---
	Output: ID
	*/
	uint GetID(void);
	/*
	Usage: Sets ID of octant
	Arguments: uint a_uID -> ID to set
	Output: ---
	*/
	void SetID(uint a_uID);
	/*
	Usage: Gets level of octant
	Arguments: ---
	Output: Level
	*/
	uint GetLevel(void);
	/*
	Usage: Sets level of octant
	Arguments: uint a_uLevel -> Level to set
	Output: ---
	*/
	void SetLevel(uint a_uLevel);
	/*
	Usage: Gets the octant's children count
	Arguments: ---
	Output: Children Count (either 0 or 8)
	*/
	uint GetChildren(void);
	/*
	Usage: Sets the octant's children count
	Arguments: uint a_uChildren -> Children count to set
	Output: ---
	*/
	void SetChildren(uint a_uChildren);
	/*
	Usage: Gets the size of the octant
	Arguments: ---
	Output: Size
	*/
	float GetSize(void);
	/*
	Usage: Sets the size of the octant
	Arguments: float a_fSize -> Size to set to the octant
	Output: ---
	*/
	void SetSize(float a_fSize);
	/*
	Usage: Gets the center vector of the octant
	Arguments: ---
	Output: Center
	*/
	vector3 GetCenter(void);
	/*
	Usage: Gets the minimum vector of the octant
	Arguments: ---
	Output: Min
	*/
	vector3 GetMin(void);
	/*
	Usage: Gets the maximum vector of the octant
	Arguments: ---
	Output: Max
	*/
	vector3 GetMax(void);
#pragma endregion

#pragma region Methods
	/*
	Usage: Splits the current Octree into 8, adding them to the children array
	Arguments: uint a_uID -> Id of the current octant, so that the children have the correct ID
	Output: ---
	*/
	void SplitIntoOctants(uint a_uID)
	{

	}

#pragma endregion

private:
	/*
	Usage: Deallocates member fields
	Arguments: ---
	Output: ---
	*/
	void Release(void);
	/*
	Usage: Allocates member fields
	Arguments: ---
	Output: ---
	*/
	void Init(void);
};//class

} //namespace Simplex

#endif //__MYOCTANT_H_

/*
USAGE:
ARGUMENTS: ---
OUTPUT: ---
*/