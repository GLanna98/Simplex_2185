/*----------------------------------------------
Programmer: Gabriel L'Huillier Lanna (gll1460@rit.edu)
Date: March-April 2019
The layout of this class was taken from Simplex/Physics/Octant.h
----------------------------------------------*/
#ifndef __MYOCTANT_H_
#define __MYOCTANT_H_

#include "MyEntityManager.h"

namespace Simplex
{

//System Class
class MyOctant
{
	static uint ms_uOctantCount; //will store the number of octants instantiated
	static uint ms_uMaxLevel; //will store the maximum level an octant can go to
	static uint ms_uIdealEntityCount; //will tell how many ideal Entitites this object will contain

	uint m_uID = 0; //Will store the current ID for this octant
	uint m_uLevel = 0; //Will store the current level of the octant
	uint m_uChildren = 0; //Number of children on the octant (either 0 or 8)

	//float m_fSize = 0.0f; //Size of the octant - replaced by m_v3HalfWidth

	MeshManager* m_pMeshMngr = nullptr; //Mesh Manager Singleton
	MyEntityManager* m_pEntityMngr = nullptr; //Entity Nabager Singleton

	vector3 m_v3Center = vector3(0.0f); //Will store the center point of the octant
	vector3 m_v3Min = vector3(0.0f); //Will store the minimum vector of the octant
	vector3 m_v3Max = vector3(0.0f); //Will hold the maximum vector of the octant
	vector3 m_v3HalfWidth = vector3(0.0f); //Will hold the half size of the octant

	MyOctant* m_pParent = nullptr; //Will store the parent of the current octant
	MyOctant* m_pChild[8]; //Will store the children of the current octant

	std::vector<uint> m_lEntityList; //List of Entitites under this octant (Index in Entity Manager)

	MyOctant* m_pRoot = nullptr; //Root octant
	std::vector<MyOctant*> m_lChild; //list of nodes that contain objects (this will be applied to root only)

public:
	/*
	Usage: Constructor (Using current level and desired entity count as parameters)
	Arguments: 
	- a_uMaxLevel -> Max level of subdivision
	- a_uIdealEntityCount -> Describes the desired count of entities in each octant
	Output: class object instance
	*/
	MyOctant(uint a_uMaxLevel = 2, uint a_uIdealEntityCount = 5);
	/*
	Usage: Constructor (Using center and size as parameters)
	Arguments:
	- a_v3Center -> Center vector of the octant
	- a_fSize -> Size of each edge of the octant
	Output: class object instance
	*/
	MyOctant(vector3 a_v3Center, vector3 a_v3HalfWidth);
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
	Usage: Gets the half width of the octant
	Arguments: ---
	Output: HalfWidth
	*/
	vector3 GetHalfWidth(void);
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
	Usage: Check if there is a specified entity inside the octant
	Arguments: uint a_uRBIndex -> Id of the entity in the entity manager
	Output: if there is a collision
	*/
	bool IsColliding(uint a_uRBIndex);
	/*
	Usage: Display the octant by index
	Arguments: 
	- uint a_uIndex -> Octant to display
	- vectore3 a_v3Color -> Color of the octant
	Output: ---
	*/
	void Display(uint a_uIndex, vector3 a_v3Color = C_YELLOW);
	/*
	Usage: Display the octant
	Arguments: vector3 a_v3Color -> Color of the octant
	Output: ---
	*/
	void Display(vector3 a_v3Color = C_YELLOW);
	/*
	Usage: Display the non-empty leafs (no subdivisions) in the octree
	Arguments: vector3 a_v3Color -> Color of the octant
	Output: ---
	*/
	void DisplayLeafs(vector3 a_v3Color = C_YELLOW);
	/*
	Usage: Clear the entity list for each node
	Arguments: ---
	Output: ---
	*/
	void ClearEntityList(void);
	/*
	Usage: Creates 8 new octants as children of this one
	Arguments: ---
	Output: ---
	*/
	void Subdivide(void);
	/*
	Usage: Get the specified child of this octant from the array
	Arguments:uint a_uChild -> Index of child in child array
	Output: the octant object for the child
	*/
	MyOctant* GetChild(uint a_uChild);
	/*
	Usage: Get the parent of this octant
	Arguments: ---
	Output: the octant object for the parent
	*/
	MyOctant* GetParent(void);
	/*
	Usage: Check if the octant is not subdivided
	Arguments: ---
	Output: It is a leaf (no children)
	*/
	bool IsLeaf(void);
	/*
	Usage: Check if the octant contains more than the specified entity count
	Arguments: uint a_uEntities -> Number of entities to check
	Output: It contains more than the specified amount of entities
	*/
	bool ContainsMoreThan(uint a_uEntities);
	/*
	Usage: Delete all of the octants in its lineage
	Arguments: ---
	Output: ---
	*/
	void KillBranches(void);
	/*
	Usage: Creates the tree
	Arguments: Max level of tree when constructing
	Output: ---
	*/
	void ConstructTree(void);
	/*
	Usage: Go through all octants setting IDs to each entity in them
	Arguments: ---
	Output: ---
	*/
	void AssignIDtoEntity(void);
	/*
	Usage: Counts all octants
	Arguments: ---
	Output: ---
	*/
	uint GetOctantCount(void);

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
	/*
	Usage: Creates a list for all leafs that contain objects
	Arguments: ---
	Output: ---
	*/
	void ConstructList(void);
};//class

} //namespace Simplex

#endif //__MYOCTANT_H_

/*
USAGE:
ARGUMENTS: ---
OUTPUT: ---
*/