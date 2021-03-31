#pragma once

#include "ECS/components/BaseComponent.h"

class Entity
{
public:
	/////////////////////////////////////////////////
	// 
	//		Methods
	//
	/////////////////////////////////////////////////

	//-----------------------------------------------
	//		Constructors
	//

	Entity();

	//-----------------------------------------------
	//		Destructors
	//

	virtual ~Entity();

	//-----------------------------------------------
	//		Getters
	//

	unsigned int GetId() const;

	//-----------------------------------------------
	//		Components
	//

	template<typename T>
	void AddComponent()
	{
#ifdef DEBUG_MODE
		if (HasComponent<T>())
		{
			std::cout << "	Debug: [Entity::AddComponent] already have component " << static_cast<int>(T::GetStaticComponentType()) << std::endl;
			ASSERT(false);
		}
#endif // DEBUG_MODE

		mComponents.insert(std::make_pair(T::GetStaticComponentType(), std::make_shared<T>(mId)));
	}

	template<typename T>
	bool HasComponent()
	{
		return mComponents.find(T::GetStaticComponentType()) != mComponents.end();
	}

	template<typename T>
	const std::shared_ptr<T> GetComponent()
	{
		ASSERT(HasComponent<T>())
		return std::dynamic_pointer_cast<T>(mComponents.at(T::GetStaticComponentType()));
	}

	template<typename T>
	std::shared_ptr<T> GetMutableComponent()
	{
		ASSERT(HasComponent<T>())
		return std::dynamic_pointer_cast<T>(mComponents.at(T::GetStaticComponentType()));
	}

protected:
	/////////////////////////////////////////////////
	// 
	//		Variables
	//
	/////////////////////////////////////////////////

	/* each Entity has unique Id which links it with Component and vice versa */
	unsigned int mId;

	std::map<ComponentType, std::shared_ptr<BaseComponent>> mComponents;

	// TODO: add scene ref later

private:
	static unsigned int NextEntityId;
};
