// Particle Generator for ParallelPhotonBeamGenerator
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

#include "ParallelPhotonBeamGenerator.hh"

#include "TsParameterManager.hh"
#include "Randomize.hh"
#include "G4RandomDirection.hh"
#include "G4RandomTools.hh"
#include <cmath>
#include <CLHEP/Units/PhysicalConstants.h>
#include "G4Point3D.hh"
#include "TsVGeometryComponent.hh"


ParallelPhotonBeamGenerator::ParallelPhotonBeamGenerator(TsParameterManager* pM, TsGeometryManager* gM, TsGeneratorManager* pgM, G4String sourceName) :
TsVGenerator(pM, gM, pgM, sourceName)
{
	ResolveParameters();
}


ParallelPhotonBeamGenerator::~ParallelPhotonBeamGenerator()
{
}


// This is just a silly demonstration particle source.
// It creates one or more identical primary particles, based on the parameter SplitNumber.
// The rest of the parameters are already provided by the base class's ResolveParameters.
void ParallelPhotonBeamGenerator::ResolveParameters() {
	TsVGenerator::ResolveParameters();
	
	//fSplitNumber = fPm->GetIntegerParameter(GetFullParmName("SplitNumber"));
	//fdcos1 = fPm->GetUnitlessParameter(GetFullParmName("DCos1"));
	//fdcos2 = fPm->GetUnitlessParameter(GetFullParmName("DCos2"));
	//fdcos3 = fPm->GetUnitlessParameter(GetFullParmName("DCos3"));

	//fxpos = fPm->GetDoubleParameter(GetFullParmName("TransX"), "Length");
	//fypos = fPm->GetDoubleParameter(GetFullParmName("TransY"), "Length");
	//fzpos = fPm->GetDoubleParameter(GetFullParmName("TransZ"), "Length");

	fangleError = fPm->GetDoubleParameter(GetFullParmName("AngleError"), "Angle");
	fmeanPositronRange = fPm->GetDoubleParameter(GetFullParmName("MeanPositronRange"), "Length");

}

//Creates a random unit vector error radians from the input vector
G4ThreeVector GetErrorVector(G4ThreeVector v, G4double error) {

	G4double deltaMag = tan(error);

	G4ThreeVector deltaVec = G4PlaneVectorRand(v).unit() * deltaMag;

	//G4cout << "One?: " << G4PlaneVectorRand(v).unit().mag() << G4endl;
	//G4cout << "Desired mag: " << deltaMag << " Actual mag: " << deltaVec.mag() << G4endl;

	G4ThreeVector newDirection = (v + deltaVec).unit();

	//G4cout << "Desired error: " << error << " Actual error: " << v.angle(newDirection) << G4endl;

	return newDirection;
}


void ParallelPhotonBeamGenerator::GeneratePrimaries(G4Event* anEvent)
{
	//G4cout << "Generating primay called for custom dude" << G4endl;

	if (CurrentSourceHasGeneratedEnough())
		return;
	
	//G4cout << "Got to next line ayyy" << G4endl;


	// we define the direction of p1 and then base p2 off of that
	TsPrimaryParticle p1;
	TsPrimaryParticle p2;

	G4Point3D componentPosition = *(fComponent->GetTransRelToWorld()); //.x()

	//randomize starting positions based on mean positron range
	G4ThreeVector position = G4ThreeVector(componentPosition.x(), componentPosition.y(), componentPosition.z()) + G4RandomDirection() * fmeanPositronRange;

	//G4cout << "Chosen position: " << position << G4endl;

	p1.posX = position.x();
	p1.posY = position.y();
	p1.posZ = position.z();

	p2.posX = position.x();
	p2.posY = position.y();
	p2.posZ = position.z();

	//direction of first photon
	G4ThreeVector firstDirection = G4RandomDirection();

	G4ThreeVector secondDirection = GetErrorVector(-firstDirection, fangleError);
	
	p1.dCos1 = firstDirection.x();//fdcos1;
	p1.dCos2 = firstDirection.y();//fdcos2;
	p1.dCos3 = firstDirection.z();//fdcos3;

	p2.dCos1 = secondDirection.x();
	p2.dCos2 = secondDirection.y();
	p2.dCos3 = secondDirection.z();

	//G4cout << "V1: " << firstDirection << G4endl;
	//G4cout << "V2: " << secondDirection << G4endl;


	//p1

	p1.kEnergy = fEnergy;
	p1.weight = 1.;
	
	p1.particleDefinition = fParticleDefinition;
	
	p1.isNewHistory = true;
	
	p1.isOpticalPhoton = fIsOpticalPhoton;
	p1.isGenericIon = fIsGenericIon;
	p1.ionCharge = fIonCharge;

	//p2

	p2.kEnergy = fEnergy;
	p2.weight = 1.;
	
	p2.particleDefinition = fParticleDefinition;
	
	p2.isNewHistory = false;
	
	p2.isOpticalPhoton = fIsOpticalPhoton;
	p2.isGenericIon = fIsGenericIon;
	p2.ionCharge = fIonCharge;

	GenerateOnePrimary(anEvent, p1);
	GenerateOnePrimary(anEvent, p2);
	
	AddPrimariesToEvent(anEvent);
}
