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

#ifndef Pulse_hh
#define Pulse_hh

#include "G4ThreeVector.hh"
#include "G4Step.hh"
#include "globals.hh"

class Pulse
{    
public:
	Pulse();
	Pulse(G4Step*, G4double);
	~Pulse();

	void Add(G4Step*);

	G4ThreeVector GetPosition();
	G4double GetEnergy();
	G4double GetStartTimeGlobal();
	G4double GetEndTimeGlobal();
	G4double GetEventStartTimeTopas();
	G4double GetStartTimeTopas();
	G4double GetEndTimeTopas();

protected:


	//in future, maybe average positions of particles.
	//It's unclear to me if i should take the crystal position or some position inside the crystal
	G4ThreeVector fPosition;
	G4double fEnergy;
	G4double fStartTimeGlobal;
	G4double fEndTimeGlobal;

	G4double fEventStartTimeTopas;
	G4double fStartTimeTopas;
	G4double fEndTimeTopas;


};

#endif
