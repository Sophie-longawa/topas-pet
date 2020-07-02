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

#ifndef PETScorer_hh
#define PETScorer_hh

#include "TsVNtupleScorer.hh"

class PETScorer : public TsVNtupleScorer
{
public:
	PETScorer(TsParameterManager* pM, TsMaterialManager* mM, TsGeometryManager* gM, TsScoringManager* scM, TsExtensionManager* eM,
				G4String scorerName, G4String quantity, G4String outFileName, G4bool isSubScorer);

	virtual ~PETScorer();

	G4bool ProcessHits(G4Step*,G4TouchableHistory*);

protected:
	// Output variables
	G4double fEnergy;
	G4float fWeight;
	G4int fParticleType;
	G4bool fIsNewHistory;
	G4double fHitTimestamp;
	G4double fx;
	G4double fy;
	G4double fz;

	G4int fRunID;
	G4int fEventID;
	G4int fPrevRunID;
	G4int fPrevEventID;
};
#endif
