#pragma once

#include <Spore\BasicIncludes.h>

#define AdvCE_EditorControlsPtr intrusive_ptr<AdvCE_EditorControls>

// To avoid repeating UTFWin:: all the time.
using namespace UTFWin;
using namespace Editors;

class AdvCE_EditorControls 
	: public IWinProc
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("AdvCE_EditorControls");
	
	AdvCE_EditorControls();
	~AdvCE_EditorControls();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;
	
	bool IsVehicleEditor() const;
	void SetAdvCEManips(bool state);
	bool IsRigblockChassis(EditorRigblockPtr par) const;

	int GetEventFlags() const override;
	// This is the function you have to implement, called when a window you added this winproc to received an event
	bool HandleUIMessage(IWindow* pWindow, const Message& message) override;

private:

	const eastl::vector<uint32_t> mVehicleManipulatorsDefault = {
		EditorManipulators::kEditorManipulationPlanarInterpenetration,
		EditorManipulators::kEditorManipulationPinning,
		EditorManipulators::kEditorManipulationDeformHandle,
		EditorManipulators::kEditorManipulationRotationHandle,
	};
	const eastl::vector<uint32_t> mVehicleManipulatorsAdvCE = {
		EditorManipulators::kEditorManipulationInterpenetration,
		EditorManipulators::kEditorManipulationPinning,
		EditorManipulators::kEditorManipulationStacking,
		EditorManipulators::kEditorManipulationDeformHandle,
		EditorManipulators::kEditorManipulationRotationHandle,
		id("cEditorManipulator_AdvancedCE"),
	};

	bool mbInVehicleEditor = false;
	bool mbCtrlHeld = false;
	bool mbShiftHeld = false;
};
