#pragma once

#include "core/Core.h"

namespace TE
{
	class TE_API App
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

		App();

		//-----------------------------------------------
		//		Constructors
		//

		virtual ~App();

		//-----------------------------------------------
		//		Lifecycle
		//

		void Run();

	private:

	};

	App* CreateApp();
}
