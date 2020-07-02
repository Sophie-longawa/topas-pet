// Scorer for PETScorer
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

#include "PETScorer.hh"

#include "G4PSDirectionFlag.hh"

PETScorer::PETScorer(TsParameterManager* pM, TsMaterialManager* mM, TsGeometryManager* gM, TsScoringManager* scM, TsExtensionManager* eM,
						  G4String scorerName, G4String quantity, G4String outFileName, G4bool isSubScorer)
						 : TsVNtupleScorer(pM, mM, gM, scM, eM, scorerName, quantity, outFileName, isSubScorer)
{
	SetSurfaceScorer();

	fNtuple->RegisterColumnD(&fEnergy, "Energy", "MeV");
	fNtuple->RegisterColumnF(&fWeight, "Weight", "");
	fNtuple->RegisterColumnI(&fParticleType, "Particle Type (in PDG Format)");
	fNtuple->RegisterColumnB(&fIsNewHistory, "Flag to tell if this is the First Scored Particle from this History (1 means true)");
	fNtuple->RegisterColumnD(&fHitTimestamp, "Hit Time (ns)", "ns");
	fNtuple->RegisterColumnD(&fx, "x position (mm)", "mm");
	fNtuple->RegisterColumnD(&fy, "x position (mm)", "mm");
	fNtuple->RegisterColumnD(&fz, "x position (mm)", "mm");
	
}


PETScorer::~PETScorer() {;}

/*
void PETScorer::ResolveParameters() {
	TsVNtupleScorer::ResolveParameters();
	
	//fSplitNumber = fPm->GetIntegerParameter(GetFullParmName("SplitNumber"));
	fdcos1 = fPm->GetUnitlessParameter(GetFullParmName("DCos1"));
	fdcos2 = fPm->GetUnitlessParameter(GetFullParmName("DCos2"));
	fdcos3 = fPm->GetUnitlessParameter(GetFullParmName("DCos3"));



}
*/


G4bool PETScorer::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
	if (!fIsActive) {
		fSkippedWhileInactive++;
		return false;
	}

	ResolveSolid(aStep);

	if (IsSelectedSurface(aStep)) {
		G4StepPoint* theStepPoint=0;
		G4int direction = GetDirection();
		if (direction == fFlux_In) theStepPoint = aStep->GetPreStepPoint();
		else if (direction == fFlux_Out) theStepPoint = aStep->GetPostStepPoint();
		else return false;

		G4ThreeVector position = theStepPoint->GetPosition();
		fx = position.x();
		fy = position.y();
		fz = position.z();

		fEnergy       = theStepPoint->GetKineticEnergy();
		fWeight       = theStepPoint->GetWeight();
		fParticleType = aStep->GetTrack()->GetDefinition()->GetPDGEncoding();
		fHitTimestamp = GetTime();

		// Check if this is a new history
		fRunID   = GetRunID();
		fEventID = GetEventID();
		if (fEventID != fPrevEventID || fRunID != fPrevRunID) {
			fIsNewHistory = true;
			fPrevEventID = fEventID;
			fPrevRunID = fRunID;
		} else {
			fIsNewHistory = false;
		}

		fNtuple->Fill();
		return true;
	}
	return false;
}
