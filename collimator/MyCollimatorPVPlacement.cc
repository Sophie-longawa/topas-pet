// Component for MyCollimatorPVPlacement


#include "MyCollimatorPVPlacement.hh"

#include "TsParameterManager.hh"

#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4SystemOfUnits.hh"
#include "G4PVPlacement.hh"

#include <string>
#include <thread>

MyCollimatorPVPlacement::MyCollimatorPVPlacement(TsParameterManager* pM, TsExtensionManager* eM, TsMaterialManager* mM, TsGeometryManager* gM,
                                       TsVGeometryComponent* parentComponent, G4VPhysicalVolume* parentVolume, G4String& name):
TsVGeometryComponent(pM, eM, mM, gM, parentComponent, parentVolume, name)
{;}

MyCollimatorPVPlacement::~MyCollimatorPVPlacement() {;}

G4String GetOpeningID(int i, int j, int k, G4String fName) {
    return G4String("OpeningOfCollimator_") + fName + G4String("_"  + std::to_string(i) + "_" + std::to_string(j) + "_" + std::to_string(k));
}

G4VPhysicalVolume* MyCollimatorPVPlacement::Construct() {

    G4cerr << "Starting constructing collimator " << fName << G4endl;

    BeginConstruction();

    G4String OpeningMaterial = fPm->ParameterExists(GetFullParmName("OpeningMaterial")) ? 
                                     fPm->GetStringParameter(GetFullParmName("OpeningMaterial")) : "G4_AIR";
    
    G4String CollimatorMaterial = fPm->GetStringParameter(GetFullParmName("Material"));

    const G4int AxisXCuts = fPm->ParameterExists(GetFullParmName("AxisXCuts")) ? 
                            fPm->GetIntegerParameter(GetFullParmName("AxisXCuts")) : 1;
    
    const G4int AxisYCuts = fPm->ParameterExists(GetFullParmName("AxisYCuts")) ? 
                            fPm->GetIntegerParameter(GetFullParmName("AxisYCuts")) : 1;
    
    const G4int AxisZCuts = fPm->ParameterExists(GetFullParmName("AxisZCuts")) ? 
                            fPm->GetIntegerParameter(GetFullParmName("AxisZCuts")) : 1;

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

    const G4double HLX = fPm->GetDoubleParameter(GetFullParmName("HLX"),"Length");
    const G4double HLY = fPm->GetDoubleParameter(GetFullParmName("HLY"),"Length");
    const G4double HLZ = fPm->GetDoubleParameter(GetFullParmName("HLZ"),"Length");

    const G4double ThicknessAxisXPercentage = fPm->ParameterExists(GetFullParmName("ThicknessAxisXPercentage")) ? 
                                              fPm->IGetUnitlessParameter(GetFullParmName("ThicknessAxisXPercentage")) : 0;
    
    const G4double ThicknessAxisYPercentage = fPm->ParameterExists(GetFullParmName("ThicknessAxisYPercentage")) ? 
                                              fPm->IGetUnitlessParameter(GetFullParmName("ThicknessAxisYPercentage")) : 0;
    
    const G4double ThicknessAxisZPercentage = fPm->ParameterExists(GetFullParmName("ThicknessAxisZPercentage")) ? 
                                              fPm->IGetUnitlessParameter(GetFullParmName("ThicknessAxisZPercentage")) : 0;

    const G4double HLXOfDummyBox = (1 - ThicknessAxisXPercentage) * HLX / AxisXCuts;
    const G4double HLYOfDummyBox = (1 - ThicknessAxisYPercentage) * HLY / AxisYCuts;
    const G4double HLZOfDummyBox = (1 - ThicknessAxisZPercentage) * HLZ / AxisZCuts;

    G4cerr << "Found that HLX, HLY, HLZ = " << HLX << ' ' << HLY << ' ' << HLZ << '\n';

    fEnvelopeLog = CreateLogicalVolume(fName, CollimatorMaterial, new G4Box(fName, HLX, HLY, HLZ));
    fEnvelopePhys = CreatePhysicalVolume(fEnvelopeLog);
    
    /** Now we will cut the openings from the physical volume
     *  using G4PVPlacement. The openings can be of any solid 
     *  shape and will have by default Air Material (as Jan suggested).
     */

    for (int i = 0;i < AxisXCuts;i++) {
        const G4double XCenter = (2 * i + 1) * (HLX / AxisXCuts) - HLX;
        for (int j = 0;j < AxisYCuts;j++) {    
            const G4double YCenter = (2 * j + 1) * (HLY / AxisYCuts) - HLY;
            for (int k = 0;k < AxisZCuts;k++) {
                const G4double ZCenter = (2 * k + 1) * (HLZ / AxisZCuts) - HLZ;
                const G4ThreeVector BoxOffsets = G4ThreeVector(XCenter, YCenter, ZCenter);

                G4VSolid* DeletedBox = new G4Box("Dummy Box", HLXOfDummyBox, HLYOfDummyBox, HLZOfDummyBox);

                G4LogicalVolume*  DeletedBoxLogicalVolume = CreateLogicalVolume(GetOpeningID(i, j, k, fName), OpeningMaterial, DeletedBox);
                
                //We don't need to pointer to the opening
                new G4PVPlacement(0, BoxOffsets, fName, DeletedBoxLogicalVolume, fEnvelopePhys, false, 1);
            }
        }
    }
    
    G4cerr << "Finished calculating collimator " << fName << G4endl;

    return fEnvelopePhys;
}
