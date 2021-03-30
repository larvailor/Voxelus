#pragma once

enum class ComponentType
{
	None = -1,
	Transform
};

class BaseComponent
{
public:
	/////////////////////////////////////////////////
	// 
	//		Methods
	//
	/////////////////////////////////////////////////

	//-----------------------------------------------
	//		Destructors
	//

	virtual ~BaseComponent();

	//-----------------------------------------------
	//		Getters
	//

	virtual unsigned int GetEntityId() const;
	virtual ComponentType GetComponentType() const;

	//-----------------------------------------------
	//		Setters
	//

	virtual void SetEntityId(unsigned int entityId);

protected:
	/////////////////////////////////////////////////
	// 
	//		Methods
	//
	/////////////////////////////////////////////////

	//-----------------------------------------------
	//		Constructors
	//

	BaseComponent(unsigned int entityId, ComponentType componentType);

	/////////////////////////////////////////////////
	// 
	//		Variables
	//
	/////////////////////////////////////////////////

	unsigned int mEntityId;

	ComponentType mComponentType;
};
