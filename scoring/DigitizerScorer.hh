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

#ifndef DigitizerScorer_hh
#define DigitizerScorer_hh

#include "TsVNtupleScorer.hh"
#include "Pulse.hh"
#include "G4Threading.hh"
#include "G4AutoLock.hh"

class DigitizerScorer : public TsVNtupleScorer
{
public:
	DigitizerScorer(TsParameterManager* pM, TsMaterialManager* mM, TsGeometryManager* gM, TsScoringManager* scM, TsExtensionManager* eM,
				G4String scorerName, G4String quantity, G4String outFileName, G4bool isSubScorer);

	virtual ~DigitizerScorer();

	G4bool ProcessHits(G4Step*,G4TouchableHistory*);

	void UserHookForEndOfEvent();

protected:
	// Output variables
	G4double fEnergy;
	G4int fEventID;

	G4double fStartGlobalTime;
	G4double fEndGlobalTime;
	G4double fTopasTime;
	G4double fXPos;
	G4double fYPos;
	G4double fZPos;

	std::map<G4String,std::map<G4int,Pulse*>*> pulses;
};
#endif
