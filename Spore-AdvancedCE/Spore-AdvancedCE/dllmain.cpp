// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <Spore\Editors\EditorRigblock.h>
#include "AdvancedCEDebug.h"
#include "AdvCE_EditorControls.h"

AdvCE_EditorControls* editorControls;

void Initialize()
{
	CheatManager.AddCheat("AdvCEDebug", new AdvancedCEDebug());
	editorControls = new(AdvCE_EditorControls);
}

void ReparentParts(Editors::cEditor* editor) {
	auto it = eastl::find(editor->mEnabledManipulators.begin(), editor->mEnabledManipulators.end(), id("cEditorManipulator_AdvancedCE"));
	if (editor->IsMode(Editors::Mode::BuildMode) && it != editor->mEnabledManipulators.end())
	{
		for (EditorRigblockPtr part : Editor.GetEditorModel()->mRigblocks)
		{
			if (AdvancedCEDebug::PartCanReparent(part.get()))
			{
				if (editorControls->mpPrevParent) {
					part->mpParent = editorControls->mpPrevParent;
				}
				else {
					part->mpParent = AdvancedCEDebug::GetClosestPart(part.get());
				}
				if (part->mBooleanAttributes[Editors::kEditorRigblockModelActsLikeGrasper] || part->mBooleanAttributes[Editors::kEditorRigblockModelActsLikeFoot])
				{
					HintManager.ShowHint(id("advce-corruptlimb"));
				}
			}
		}
	}
}

// If part loses its parent, apply the previous parent, or find the next closest part to attach to.
// TODO: symmetry.
member_detour(Editor_EditHistoryDetour, Editors::cEditor, void(bool, Editors::EditorStateEditHistory*)) {
	void detoured(bool arg1, Editors::EditorStateEditHistory* pStateHistory){
		ReparentParts(this);
		original_function(this, arg1, pStateHistory);
	}
};

void Dispose()
{
	editorControls = nullptr;
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

