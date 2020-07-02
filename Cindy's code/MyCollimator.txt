// Component for MyCollimator
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

#include "MyCollimator.hh"

#include "TsParameterManager.hh"

#include "G4LogicalVolume.hh"
#include "G4UIcommand.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SystemOfUnits.hh"

MyCollimator::MyCollimator(TsParameterManager* pM, TsExtensionManager* eM, TsMaterialManager* mM, TsGeometryManager* gM,
									   TsVGeometryComponent* parentComponent, G4VPhysicalVolume* parentVolume, G4String& name):
TsVGeometryComponent(pM, eM, mM, gM, parentComponent, parentVolume, name)
{;}


MyCollimator::~MyCollimator() {;}

G4VPhysicalVolume* MyCollimator::Construct()
{
	BeginConstruction();

	const G4int NbOfRows      = fPm->GetIntegerParameter(GetFullParmName("NbOfRows"));

	//Box
	G4Box* gCollimatorBox = new G4Box(fName,
									fPm->GetDoubleParameter(GetFullParmName("HLX"), "Length"),
									fPm->GetDoubleParameter(GetFullParmName("HLY"), "Length"),
									fPm->GetDoubleParameter(GetFullParmName("HLZ"), "Length"));
	fEnvelopeLog = CreateLogicalVolume(gCollimatorBox);
	fEnvelopePhys = CreatePhysicalVolume(fEnvelopeLog);

	for (G4int i = 0; i < NbOfRows; ++i) {
		G4String  iRow  	   = "Row" + G4UIcommand::ConvertToString(i+1);
		G4int     NbHoles 	   = fPm->GetVectorLength(GetFullParmName(iRow, "Displacement"));
		G4double* Displacement = fPm->GetDoubleVector(GetFullParmName(iRow, "Displacement"), "Length");
		G4double  PosX 		   = 0.0;
		G4double  PosY 		   = 0.0;
		G4double  PosZ	       = fPm->GetDoubleParameter(GetFullParmName(iRow, "PosZ"), "Length");
		G4RotationMatrix* AlignMatrix = new G4RotationMatrix;
		G4bool    IsAlignX 			  = false;
		G4String  Align 			  = fPm->GetStringParameter(GetFullParmName(iRow, "Align"));
		G4double  Rmin = fPm->GetDoubleParameter( GetFullParmName(iRow, "RMin"), "Length");
		G4double  Rmax = fPm->GetDoubleParameter( GetFullParmName(iRow, "RMax"), "Length");
        G4double  HL   = fPm->GetDoubleParameter( GetFullParmName(iRow, "HL"), "Length");
		if ( fPm->ParameterExists(GetFullParmName("PrintInformation")) && fPm->GetBooleanParameter(GetFullParmName("PrintInformation")) ) {
			G4cout<<" Row: \"" << i <<"\" , # of Holes: "<< NbHoles << ", Alignment: "<< Align
			<<", Hole (Rmin= "<< Rmin/cm <<" cm, Rmax= "<< Rmax/cm <<" cm, HL= "<< HL/cm<<" cm)"
			<<" , Z Position in the Chamber: "<<PosZ/cm<< " (cm)" <<G4endl;
		}
		//Hole specification
		G4Tubs* ghole = new G4Tubs(iRow, Rmin, Rmax, HL, 0.0*deg, 360.0*deg);
		G4LogicalVolume* lhole = CreateLogicalVolume( iRow, ghole);

		//Alignment flags of holes
		if ( Align == "X" || Align == "x") {
			AlignMatrix->rotateX(0.0);
			AlignMatrix->rotateY(90.0*deg);
			AlignMatrix->rotateZ(0.0);
			IsAlignX = true;
		} else if ( Align == "Y" || Align == "y") {
			AlignMatrix->rotateX(90.0*deg);
			AlignMatrix->rotateY(0.0);
			AlignMatrix->rotateZ(0.0);
			IsAlignX = false;
		} else {
			G4cout<<"ERROR: Align value should be X or Y axis: see "<< GetFullParmName(iRow, "Align") << G4endl;
			exit(1);
		}

		//Actual wire displacement
		for (int j = 0; j < NbHoles; ++j ) {
			if (IsAlignX)
				PosX = 0.0;
			else
				PosX = Displacement[j];

			if (IsAlignX)
				PosY = Displacement[j];
			else
				PosY = 0.0;

			if ( fPm->ParameterExists(GetFullParmName("PrintInformation")) && fPm->GetBooleanParameter(GetFullParmName("PrintInformation")) )
				G4cout<<"    Hole \""<< j <<"\", Position (" << PosX/cm<<" cm, "<< PosY/cm <<" cm)" <<G4endl;

			CreatePhysicalVolume("Hole", j, true, lhole, AlignMatrix, new G4ThreeVector(PosX,PosY,PosZ), fEnvelopePhys);
		}//for (NbWires, displacement)
	}//for (Wire,Layers)
	return fEnvelopePhys;
}
