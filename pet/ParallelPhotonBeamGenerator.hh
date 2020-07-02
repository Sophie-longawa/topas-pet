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

#ifndef ParallelPhotonBeamGenerator_hh
#define ParallelPhotonBeamGenerator_hh

#include "TsVGenerator.hh"

class ParallelPhotonBeamGenerator : public TsVGenerator
{
public:
	ParallelPhotonBeamGenerator(TsParameterManager* pM, TsGeometryManager* gM, TsGeneratorManager* pgM, G4String sourceName);
	~ParallelPhotonBeamGenerator();

	void ResolveParameters();
	
	void GeneratePrimaries(G4Event* );
	
private:
	//G4int fSplitNumber;
	//G4double fdcos1;
	//G4double fdcos2;
	//G4double fdcos3;

	G4double fxpos;
	G4double fypos;
	G4double fzpos;

	G4double fangleError;
	G4double fmeanPositronRange;
};
#endif
