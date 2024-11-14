// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <Spore\Editors\EditorRigblock.h>
#include "AdvancedCEDebug.h"
#include "AdvCE_EditorControls.h"

void Initialize()
{
	CheatManager.AddCheat("AdvCEDebug", new AdvancedCEDebug());
	AdvCE_EditorControls* editorControls = new(AdvCE_EditorControls);
	//Editor.CommitEditHistory()
}

bool PartCanReparent(EditorRigblockPtr part) {
	return (!part->mBooleanAttributes[Editors::kEditorRigblockModelIsVertebra] && !part->mBooleanAttributes[Editors::kEditorRigblockModelUseSkin]
		&& !part->mBooleanAttributes[Editors::kEditorRigblockModelCanBeParentless]
		&& !part->mpParent);
}

// If part loses its parent, find the next closest part to attach to.
// TODO: eventually make this try to reparent to the previous parent.
member_detour(Editor_EditHistoryDetour, Editors::cEditor, void(bool, Editors::EditorStateEditHistory*)) {
	void detoured(bool arg1, Editors::EditorStateEditHistory * pStateHistory){
		auto it = eastl::find(this->mEnabledManipulators.begin(), this->mEnabledManipulators.end(), id("cEditorManipulator_AdvancedCE"));
		if (this->IsMode(Editors::Mode::BuildMode) && it != this->mEnabledManipulators.end())
		{
			for each (EditorRigblockPtr part in Editor.GetEditorModel()->mRigblocks)
			{
				if (PartCanReparent(part))
				{
					part->mpParent = AdvancedCEDebug::GetClosestPart(part.get());//Editor.GetEditorModel()->mRigblocks[0];
					if (part->mBooleanAttributes[Editors::kEditorRigblockModelActsLikeGrasper] || part->mBooleanAttributes[Editors::kEditorRigblockModelActsLikeFoot])
					{
						HintManager.ShowHint(id("advce-corruptlimb"));
					}
				}
			}
		}
		original_function(this, arg1, pStateHistory);
	}
};

void Dispose()
{
	// This method is called when the game is closing
}

void AttachDetours()
{
	Editor_EditHistoryDetour::attach(GetAddress(Editors::cEditor, CommitEditHistory));
}


// Generally, you don't need to touch any code here
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		ModAPI::AddPostInitFunction(Initialize);
		ModAPI::AddDisposeFunction(Dispose);

		PrepareDetours(hModule);
		AttachDetours();
		CommitDetours();
		break;

	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

