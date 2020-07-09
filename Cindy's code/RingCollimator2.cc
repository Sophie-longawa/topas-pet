// Component for RingCollimator2
//
//  RingCollimator2.cc
//  v2
//
//  Created by Cindy Zhang on 7/6/20.
//

#include "RingCollimator2.hh"

#include "TsParameterManager.hh"

#include "G4Box.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4RotationMatrix.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

#include <cmath>
#include <cstring>

RingCollimator2::RingCollimator2(TsParameterManager* pM, TsExtensionManager* eM, TsMaterialManager* mM, TsGeometryManager* gM,
             TsVGeometryComponent* parentComponent, G4VPhysicalVolume* parentVolume, G4String& name) :
TsVGeometryComponent(pM, eM, mM, gM, parentComponent, parentVolume, name)
{;
}


RingCollimator2::~RingCollimator2()
{;
}


G4VPhysicalVolume* RingCollimator2::Construct()
{
    BeginConstruction();
    
    // Getting a bunch of parameters for collimator specifications
    G4String OpeningMaterial = fPm->ParameterExists(GetFullParmName("OpeningMaterial")) ?
                               fPm->GetStringParameter(GetFullParmName("OpeningMaterial")) : fParentComponent->GetResolvedMaterialName();
    
    G4String BoxMaterial = fPm->GetStringParameter(GetFullParmName("Material"));
    
    const G4int AxisXCuts = fPm->ParameterExists(GetFullParmName("AxisXCuts")) ?
                            fPm->GetIntegerParameter(GetFullParmName("AxisXCuts")) : 1;

    const G4int AxisYCuts = fPm->ParameterExists(GetFullParmName("AxisYCuts")) ?
                            fPm->GetIntegerParameter(GetFullParmName("AxisYCuts")) : 1;

    const G4int AxisZCuts = fPm->ParameterExists(GetFullParmName("AxisZCuts")) ?
                            fPm->GetIntegerParameter(GetFullParmName("AxisZCuts")) : 1;
    
    const G4String RingAxis = fPm->GetStringParameter(GetFullParmName("RingAxis"));

    G4cerr << "The cuts are: " << AxisXCuts << ' ' << AxisYCuts << ' ' << AxisZCuts << '\n';

    if (AxisXCuts <= 0) {
        G4cout << "Error: AxisXCuts should be a positive integer, see: " << GetFullParmName("AxisXCuts") << G4endl;
        exit(1);
    }

    if (AxisYCuts <= 0) {
        G4cout << "Error: AxisYCuts should be a positive integer, see: " << GetFullParmName("AxisYCuts") << G4endl;
        exit(1);
    }

    if (AxisZCuts <= 0) {
        G4cout << "Error: AxisZCuts should be a positive integer, see: " << GetFullParmName("AxisZCuts") << G4endl;
        exit(1);
    }

    if (AxisXCuts > 1 && AxisYCuts > 1 && AxisZCuts > 1) {
        G4cout << "Error: At least one of AxisXCuts, AxisYCuts, AxisZCuts should be equal to 1" << G4endl;
        exit(1);
    }
    

    
    if (RingAxis != "X" && RingAxis != "Y" && RingAxis != "Z") {
        G4cout << "Error: RingAxis must be X, Y or Z, see: " << GetFullParmName("RingAxis") << G4endl;
        exit(1);
    }
    
    const G4double RingRadius = fPm->GetDoubleParameter(GetFullParmName("RingRadius"), "Length");
    
    const G4int NbOfCollimators = fPm->GetIntegerParameter(GetFullParmName("NbOfCollimators"));
    
    

    const G4double HLX = fPm->GetDoubleParameter(GetFullParmName("HLX"), "Length");
    const G4double HLY = fPm->GetDoubleParameter(GetFullParmName("HLY"), "Length");
    const G4double HLZ = fPm->GetDoubleParameter(GetFullParmName("HLZ"), "Length");
    

    const G4double ThicknessAxisXPercentage = fPm->ParameterExists(GetFullParmName("ThicknessAxisXPercentage")) ?
                                              fPm->IGetUnitlessParameter(GetFullParmName("ThicknessAxisXPercentage")) : 0;
    
    const G4double ThicknessAxisYPercentage = fPm->ParameterExists(GetFullParmName("ThicknessAxisYPercentage")) ?
                                              fPm->IGetUnitlessParameter(GetFullParmName("ThicknessAxisYPercentage")) : 0;
    
    const G4double ThicknessAxisZPercentage = fPm->ParameterExists(GetFullParmName("ThicknessAxisZPercentage")) ?
                                              fPm->IGetUnitlessParameter(GetFullParmName("ThicknessAxisZPercentage")) : 0;
    
    // Create a "Dummy box" of cuts
    const G4double HLXOfDummyBox = (1 - ThicknessAxisXPercentage) * HLX / AxisXCuts;
    const G4double HLYOfDummyBox = (1 - ThicknessAxisYPercentage) * HLY / AxisYCuts;
    const G4double HLZOfDummyBox = (1 - ThicknessAxisZPercentage) * HLZ / AxisZCuts;

    G4VSolid* DeletedBox = new G4Box("Dummy Box", HLXOfDummyBox, HLYOfDummyBox, HLZOfDummyBox);
    
    G4VSolid* gWholeBox = new G4Box("Whole Box",HLX,HLY,HLZ);
    
    G4LogicalVolume* WholeBoxLogicalVolume = CreateLogicalVolume("Whole Box Logical Volume", BoxMaterial, gWholeBox);
    G4LogicalVolume* DeletedBoxLogicalVolume = CreateLogicalVolume("Deletex Box Logical Volume", OpeningMaterial, DeletedBox);
    
    // Making an envelope of mother's material
    G4String subComponentName     = "Box";
    G4String envelopeMaterialName = GetResolvedMaterialName(subComponentName);
    G4Tubs* gEmpty = new G4Tubs(fName, 0.0*cm, RingRadius*2, 2*HLZ, 0.0*deg, 360.0*deg);
    fEnvelopeLog = CreateLogicalVolume(fName, envelopeMaterialName, gEmpty);
        fEnvelopeLog->SetVisAttributes(fPm->GetInvisible());
        fEnvelopePhys = CreatePhysicalVolume(fEnvelopeLog);
    
    G4LogicalVolume* lWholeBox = CreateLogicalVolume(subComponentName, BoxMaterial, gWholeBox);
    
    // Setting up the ring formation
    for (int box = 0;box < NbOfCollimators;++box) {
        const double Angle = 2 * M_PI * box / NbOfCollimators;
        const G4double Trans1 = RingRadius * cos(Angle);
        const G4double Trans2 = RingRadius * sin(Angle);

        G4double TransX(Trans1), TransY(Trans2), TransZ(0);
        
    const G4double RotationAngle = 0.5 * M_PI - Angle;
        
        G4RotationMatrix* RotMatrix = new G4RotationMatrix;
        RotMatrix->rotateX(0);
        RotMatrix->rotateY(0);
        RotMatrix->rotateZ(RotationAngle);
        
        G4ThreeVector* TransVector = new G4ThreeVector(TransX, TransY, TransZ);

        G4VPhysicalVolume* Current_Box = CreatePhysicalVolume(subComponentName, box, true, lWholeBox, RotMatrix, TransVector, fEnvelopePhys);
        
        // Setting up Collimator cuts
        for (int i = 0;i < AxisXCuts;++i) {
            const G4double XCenter = (2 * i + 1) * (HLX / AxisXCuts) - HLX;
            for (int j = 0;j < AxisYCuts;++j) {
                const G4double YCenter = (2 * j + 1) * (HLY / AxisYCuts) - HLY;
                for (int k = 0;k < AxisZCuts;++k) {
                    const G4double ZCenter = (2 * k + 1) * (HLZ / AxisZCuts) - HLZ;
                    TransVector = new G4ThreeVector(XCenter, YCenter, ZCenter);
                    CreatePhysicalVolume("Deleted Box Physical Volume",
                            box * AxisXCuts * AxisYCuts * AxisZCuts + i * AxisYCuts * AxisZCuts + j * AxisZCuts + k,
                            true, DeletedBoxLogicalVolume, 0, TransVector, Current_Box);
            }
        }
    }
}
    return fEnvelopePhys;
    
}
