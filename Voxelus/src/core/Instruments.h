#pragma once

class Instruments
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

	Instruments() = delete;

	/////////////////////////////////////////////////
	// 
	//		Variables
	//
	/////////////////////////////////////////////////

	//-----------------------------------------------
	//		Brush
	//

	static float BrushColorR;
	static float BrushColorG;
	static float BrushColorB;

	enum class Type
	{
		Add = 0,
		Delete,
		Brush
	};
	static Type Current;
};
