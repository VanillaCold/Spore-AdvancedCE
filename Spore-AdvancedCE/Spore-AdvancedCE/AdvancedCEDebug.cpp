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

EditorRigblockPtr AdvancedCEDebug::GetClosestPart(Editors::EditorRigblock* part)
{
	PropertyListPtr propList;

	if (PropManager.GetPropertyList(id("ClosestPartParents"), id("AdvancedCE"), propList))
	{
		EditorRigblockPtr closest = Editor.GetEditorModel()->mRigblocks[0];
		float closestDistance = 99999999999.0f;
		Vector3 position = part->mPosition;
		for (EditorRigblockPtr part2 : Editor.GetEditorModel()->mRigblocks)
		{
			float dist = Math::distance(part2->mPosition, position); // / (part2->mModelScale);
			if (dist < closestDistance && part != part2.get() && part2->mpParent != part)
			{
				closest = part2;
				closestDistance = dist;
			}

		}
		part->field_138 = closest->mPosition - position;
		return closest;
	}
	return Editor.GetEditorModel()->mRigblocks[0];
}

const char* AdvancedCEDebug::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "Debug developer function for Advanced Creature Edit.";
	}
	else {
		return "AdvancedCEDebug: Debug developer function for Advanced Creature Edit.";
	}
}
