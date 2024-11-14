#include "stdafx.h"
#include "AdvCE_EditorControls.h"

AdvCE_EditorControls::AdvCE_EditorControls()
{
	WindowManager.GetMainWindow()->AddWinProc(this);
}


AdvCE_EditorControls::~AdvCE_EditorControls()
{
}

// For internal use, do not modify.
int AdvCE_EditorControls::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int AdvCE_EditorControls::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* AdvCE_EditorControls::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(IWinProc);
	CLASS_CAST(AdvCE_EditorControls);
	return nullptr;
}

cEditor* AdvCE_EditorControls::IsInEditor() const {
	if (App::IGameModeManager::Get() && GameModeManager.GetActiveModeID() == kEditorMode) {
		auto editor = GetEditor();
		if (!editor || editor->mModelTypes.empty()) return nullptr;
		if (!editor->IsMode(Editors::Mode::BuildMode)) return nullptr;
		return editor;
	}
	return nullptr;
}

bool AdvCE_EditorControls::IsVehicleEditor() const {
	//if (!IsInEditor()) { return false; }
	auto editor = GetEditor();

	switch (editor->mModelTypes[0]) {
		case kVehicleHarvester:
		case kVehicleMilitaryLand:
		case kVehicleMilitaryWater:
		case kVehicleMilitaryAir:
		case kVehicleCulturalLand:
		case kVehicleCulturalWater:
		case kVehicleCulturalAir:
		case kVehicleEconomicLand:
		case kVehicleEconomicWater:
		case kVehicleEconomicAir:
		case kVehicleColonyLand:
		case kVehicleColonyWater:
		case kVehicleColonyAir:
		case kVehicleUFO:
			return true;
			break;
		default: return false;
	}
}

void AdvCE_EditorControls::SetAdvCEManips(bool state) {
	auto editor = GetEditor();
	if (state) {
		editor->mEnabledManipulators = mVehicleManipulatorsAdvCE;
		// programmatically suppress snapping on all rigblocks
		if (editor->GetEditorModel()) {
			for (auto rigblock : editor->GetEditorModel()->mRigblocks) {
				rigblock->mModelSymmetrySnapDelta *= 0.001f;
			}
		}
	}
	else {
		editor->mEnabledManipulators = mVehicleManipulatorsDefault;
		// programmatically re-enable snapping on all rigblocks
		if (editor->GetEditorModel()) {
			for (auto rigblock : editor->GetEditorModel()->mRigblocks) {
				rigblock->mModelSymmetrySnapDelta /= 0.001f;
			}
		}
	}
}

// exclude things that match this description, because they like to crash.
bool AdvCE_EditorControls::IsRigblockChassis(EditorRigblockPtr part) const {
	if (!part) { return false; }
	return part->mBooleanAttributes[kEditorRigblockModelPreferToBeOnPlaneOfSymmetry];
}

int AdvCE_EditorControls::GetEventFlags() const
{
	return kEventFlagBasicInput | kEventFlagAdvanced;
}

bool AdvCE_EditorControls::HandleUIMessage(IWindow* window, const Message& message)
{
	cEditor* editor = IsInEditor();
	// In the editor
	if (editor) {

		// Store the part's previous parent.
		if (message.IsType(kMsgMouseDown) && message.Mouse.IsLeftButton()) {
			if (editor->mpActivePart && editor->mpActivePart->mpParent) {
				mpPrevParent = editor->mpActivePart->mpParent;
			}
			else {
				mpPrevParent = nullptr;
			}
		}

		// Allow Shift and Ctrl to use  Advanced CE movement,
		// and Remap moving the entire vehicle to Shift+Ctrl.
		if (IsVehicleEditor()) {
			// Pressed or Unpressed CTRL
			if (message.Key.vkey == VK_CONTROL) {
				if (message.IsType(kMsgKeyDown) && !mbCtrlHeld) {
					mbCtrlHeld = true;
				}
				else if (message.IsType(kMsgKeyUp) && mbCtrlHeld) {
					mbCtrlHeld = false;
				}
			}
			// Pressed or Unpressed SHIFT
			else if (message.Key.vkey == VK_SHIFT) {
				if (message.IsType(kMsgKeyDown) && !mbShiftHeld) {
					mbShiftHeld = true;
				}
				else if (message.IsType(kMsgKeyUp) && mbShiftHeld) {
					mbShiftHeld = false;
				}
			}



			if (message.IsType(kMsgKeyDown) || message.IsType(kMsgKeyUp)) {
				// Player is trying to move the entire vehicle.
				// This has been remapped to ctrl + shift
				if ((mbCtrlHeld && mbShiftHeld) || mbCtrlHeld == mbShiftHeld) {
					SetAdvCEManips(false);
				}
				else {
					SetAdvCEManips(true);
				}
			}


		}
	}
	// Outside the editor
	else {
		// make sure to turn off Ctrl if it is unpressed even outside the editor.
		if (mbCtrlHeld && message.IsType(kMsgKeyUp) && (message.Key.vkey == VK_CONTROL || message.Key.vkey == VK_SHIFT)) {
			mbCtrlHeld = false;
			mbShiftHeld = false;
		}
	}
	return false;
}