#pragma once

/*
	An entry point to a TypoEngine
*/

#ifdef TE_PLATFORM_WINDOWS

/* 
    To run the TypoEngine this function should be implemented in your application
	Example:

	class MyApplication : public TE::TypoEngine { }

	TE::App* TE::CreateApp()
	{
		return new MyApplication();
	}
*/
extern TE::App* TE::CreateApp();

int main(int argc, char** argv)
{
	TE::App* app = TE::CreateApp();
	app->Run();
	delete app;
}

#endif
