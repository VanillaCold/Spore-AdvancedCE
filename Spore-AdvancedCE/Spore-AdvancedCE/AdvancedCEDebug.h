#pragma once

#include <Spore\BasicIncludes.h>

class AdvancedCEDebug 
	: public ArgScript::ICommand
{
public:
	AdvancedCEDebug();
	~AdvancedCEDebug();

	// Called when the cheat is invoked
	void ParseLine(const ArgScript::Line& line) override;

	static EditorRigblockPtr GetClosestPart(Editors::EditorRigblock* part);
	
	// Returns a string containing the description. If mode != DescriptionMode::Basic, return a more elaborated description
	const char* GetDescription(ArgScript::DescriptionMode mode) const override;
};

