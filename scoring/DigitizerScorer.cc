// Scorer for DigitizerScorer
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

#include "DigitizerScorer.hh"

#include "G4PSDirectionFlag.hh"
#include "globals.hh"
#include "G4Threading.hh"
#include "G4AutoLock.hh"

DigitizerScorer::DigitizerScorer(TsParameterManager* pM, TsMaterialManager* mM, TsGeometryManager* gM, TsScoringManager* scM, TsExtensionManager* eM,
						  G4String scorerName, G4String quantity, G4String outFileName, G4bool isSubScorer)
						 : TsVNtupleScorer(pM, mM, gM, scM, eM, scorerName, quantity, outFileName, isSubScorer)
{
	//SetSurfaceScorer();

	pulses = std::map<G4String,std::map<G4int,Pulse*>*>();

	fNtuple->RegisterColumnD(&fEnergy, "Energy", "MeV");
	fNtuple->RegisterColumnD(&fStartGlobalTime, "Pulse Start Time Global", "ns");
	fNtuple->RegisterColumnD(&fEndGlobalTime, "Pulse End Time Global", "ns");
	fNtuple->RegisterColumnD(&fTopasTime, "Pulse Time TOPAS", "s");
	fNtuple->RegisterColumnD(&fXPos, "X Position", "cm");
	fNtuple->RegisterColumnD(&fYPos, "Y Position", "cm");
	fNtuple->RegisterColumnD(&fZPos, "Z Position", "cm");
	//fNtuple->RegisterColumnD(&fEnergy, "Energy", "MeV"); name of volume?
}


DigitizerScorer::~DigitizerScorer() {;}




//all runs are isolated from one another, so data between runs isn't put together even if they're within the time bounds
//I'm not sure if this is the right decision because it seems that runs are generally isolated from one another, but then you
//end up with time bounds that aren't 100% realistic.
//If this is an issue, I can make some sort of queue like data structure that stores pulses that get popped off whenever they get past
//a certain time threshhold
//actually I'll just maintain the list and do it the long way to keep stuff accurate
void DigitizerScorer::UserHookForEndOfEvent() 
{

	//G4cout << "ending run start" << G4endl;

    std::map<G4String,std::map<G4int,Pulse*>*>::iterator volumeIterator = pulses.begin();

    //G4cout << "pulses size: " << pulses.size() << G4endl;
    
    while (volumeIterator != pulses.end())
    {
    	std::map<G4int,Pulse*>::iterator eventIterator = volumeIterator->second->begin();

    	//G4cout << "vol iter" << G4endl;

    	while (eventIterator != volumeIterator->second->end()) {

    		//G4cout << "pulse iter" << G4endl;

    		Pulse* pulse = eventIterator->second;

    		G4ThreeVector position = pulse->GetPosition();
    		fXPos = position.x();
    		fYPos = position.y();
    		fZPos = position.z();
    		fEnergy = pulse->GetEnergy();
    		fStartGlobalTime = pulse->GetStartTimeGlobal();
    		fEndGlobalTime = pulse->GetEndTimeGlobal();
    		fTopasTime = pulse->GetEventStartTimeTopas();

    		fNtuple->Fill();

    		//clear out recorded pulse
    		delete pulse;

    		eventIterator++;
    	}

    	volumeIterator->second->erase( volumeIterator->second->begin(), volumeIterator->second->end());

        volumeIterator++;
    }

    //G4cout << "ending run done" << G4endl;
	
}


G4bool DigitizerScorer::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
	if (!fIsActive) {
		fSkippedWhileInactive++;
		return false;
	}

	fEventID = GetEventID();

	ResolveSolid(aStep);

	G4String volumeName = aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName();


	//G4cout << "Step volume: " << volumeName << G4endl;

	Pulse* pulse;

	std::map<G4int,Pulse*>* pulsesInVolume;

	//there's possibly a faster way to do this where you use the results of find to get the value without searching again

	//find dict for volume
	if(pulses.find(volumeName) == pulses.end()) {
		//G4cout << "making dict" << G4endl;
		pulsesInVolume = new std::map<G4int,Pulse*>();
		pulses[volumeName] = pulsesInVolume;
		//G4cout << "pulses size: " << pulses.size() << G4endl;
	} else {
		pulsesInVolume = pulses[volumeName];
	}


	//find dict for getting pulse for an event
	if(pulsesInVolume->find(fEventID) == pulsesInVolume->end()) {
		//G4cout << "making pulse" << G4endl;
		pulse = new Pulse(aStep, GetTime());
		(*pulsesInVolume)[fEventID] = pulse;
	} else {
		pulse = (*pulsesInVolume)[fEventID];
		//put the hit into the adder digitizer
		pulse->Add(aStep);
	}
	
	




	/*
	if (true){//IsSelectedSurface(aStep)) {
		G4StepPoint* theStepPoint=0;
		G4int direction = GetDirection();

		G4cout << aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName() << G4endl;

		if (direction == fFlux_In) theStepPoint = aStep->GetPreStepPoint();
		else if (direction == fFlux_Out) theStepPoint = aStep->GetPostStepPoint();
		else return false;

		fEnergy       = theStepPoint->GetKineticEnergy();
		fWeight       = theStepPoint->GetWeight();
		fParticleType = aStep->GetTrack()->GetDefinition()->GetPDGEncoding();

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
	*/



	return true;
}
