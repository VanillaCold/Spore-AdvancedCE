#include "stdafx.h"
#include "AdvancedCEDebug.h"

AdvancedCEDebug::AdvancedCEDebug()
{
}


AdvancedCEDebug::~AdvancedCEDebug()
{
}


void AdvancedCEDebug::ParseLine(const ArgScript::Line& line)
{
	// This method is called when your cheat is invoked.
	// Put your cheat code here.
	//Editor.mbModelSaveLastChild = true;
	
	for each (EditorRigblockPtr part in Editor.GetEditorModel()->mRigblocks)
	{
		if (!part->mBooleanAttributes[Editors::kEditorRigblockModelIsVertebra] && !part->mBooleanAttributes[Editors::kEditorRigblockModelUseSkin] && !part->mpParent)
		{
			part->mpParent = Editor.GetEditorModel()->mRigblocks[0];
			if (part->mpSymmetricRigblock == nullptr)
			{
				
				//SporeDebugPrint("AA");
				//4a1080;
				//part->mBooleanAttributes[Editors::kEditorRigblockModelIsAsymmetric] = true;
				
			}
		}
	}
	
	/*if (Editor.mpActivePart != nullptr)
	{
		Editor.mpActivePart->mpParent = Editor.GetEditorModel()->mRigblocks[0];
	}
	else
	{
		SporeDebugPrint("WHAT");
	}*/
}

const char* AdvancedCEDebug::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "This cheat does something.";
	}
	else {
		return "AdvancedCEDebug: Elaborate description of what this cheat does.";
	}
}
