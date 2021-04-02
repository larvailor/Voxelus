
/*
	This is an everything that is needed to launch TypoEngine
*/

#include <TypoEngine.h>

class Application : public TE::App
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

	Application() 
	{

	}

	//-----------------------------------------------
	//		Constructors
	//

	~Application()
	{

	}
};

/* An implementation of an extern declaration in TypoEngine */
TE::App* TE::CreateApp()
{
	return new Application();
}
