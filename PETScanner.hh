//
// ********************************************************************
// *                                                                  *
// *                                                                  *
// * This file was obtained from Topas MC Inc under the license       *
// * agreement set forth at http://www.topasmc.org/registration       *
// * Any use of this file constitutes full acceptance of              *
// * this TOPAS MC license agreement.                                 *
// *                                                                  *
// ********************************************************************
//

#ifndef PETScanner_hh
#define PETScanner_hh

#include "TsVGeometryComponent.hh"

class PETScanner : public TsVGeometryComponent
{
public:
	PETScanner(TsParameterManager* pM, TsExtensionManager* eM, TsMaterialManager* mM, TsGeometryManager* gM,
							TsVGeometryComponent* parentComponent, G4VPhysicalVolume* parentVolume, G4String& name);
	~PETScanner();

	G4VPhysicalVolume* Construct();
	void UpdateForSpecificParameterChange(G4String parameter);

private:
	G4double  LeafHalfLength;
	G4double  MaximumLeafOpen;
	G4double* XPlusLeavesOpen;
	G4double* XMinusLeavesOpen;
	std::vector<G4VPhysicalVolume*> XPlusLeaves;
	std::vector<G4VPhysicalVolume*> XMinusLeaves;
	G4int NbOfLeavesPerSide;


	//4D vector
	//vector of rings
	//which is a vector of blocks
	//which is a 2D vector of crystals in a grid
	std::vector<std::vector<std::vector<std::vector<G4VPhysicalVolume*>>>> Crystals;
	G4int NbDetectorBlocksPerRing;
	G4int NbRings;

	//number of crystals per detector in the horizontal direction
	G4int NbCrystalsHorizontal;
	G4int NbCrystalsVertical;

	//distance between crystals in the horizontal direction
	G4double MarginHorizontal;
	G4double MarginVertical;

	//distance between rings of detectors
	G4double MarginRings;

	//distance between adjacent detector blocks in same ring
	G4double MarginBlocks;

	//dimensions of crystals
	G4double Crystallx;
	G4double Crystally;
	G4double Crystallz;
};

#endif
