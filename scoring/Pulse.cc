// Extra Class for use by DigitizerScorer
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

#include "Pulse.hh"

#include "globals.hh"
#include "G4NavigationHistory.hh"

Pulse::Pulse(G4Step* aStep, G4double startTopasTime)
{
  	//getting position of crystal
  	//code taken from here:
  	//https://geant4-forum.web.cern.ch/t/retrieve-the-position-in-global-x-y-z-coordinates-of-a-hit-sensitive-volume/1537
  	G4TouchableHandle theTouchable = aStep->GetPostStepPoint()->GetTouchableHandle();
  	G4ThreeVector origin(0.,0.,0.);
  	G4ThreeVector globalOrigin = theTouchable->GetHistory()->GetTopTransform().Inverse().TransformPoint(origin);

    fPosition = globalOrigin;

    fEventStartTimeTopas = startTopasTime;

    fStartTimeGlobal = aStep->GetPreStepPoint()->GetGlobalTime();

    fStartTimeTopas = fStartTimeGlobal + fEventStartTimeTopas;

    fEnergy = 0;

    this->Add(aStep);
}


Pulse::~Pulse()
{
}

void Pulse::Add(G4Step* aStep)
{
	//set new end time
	fEndTimeGlobal = aStep->GetPostStepPoint()->GetGlobalTime();
    fEndTimeTopas = fEndTimeGlobal + fEventStartTimeTopas;

    //add energy
    fEnergy += aStep->GetTotalEnergyDeposit();
}

G4ThreeVector Pulse::GetPosition() {
	return fPosition;
}
G4double Pulse::GetEnergy() {
	return fEnergy;
}
G4double Pulse::GetStartTimeGlobal() {
	return fStartTimeGlobal;
}
G4double Pulse::GetEndTimeGlobal() {
	return fEndTimeGlobal;
}
G4double Pulse::GetEventStartTimeTopas() {
	return fEventStartTimeTopas;
}
G4double Pulse::GetStartTimeTopas() {
	return fStartTimeTopas;
}
G4double Pulse::GetEndTimeTopas() {
	return fEndTimeTopas;
}
