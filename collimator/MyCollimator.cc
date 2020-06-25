// Component for MyCollimator

#include "MyCollimator.hh"

#include "TsParameterManager.hh"

#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4SystemOfUnits.hh"
#include "G4PVPlacement.hh"

MyCollimator::MyCollimator(TsParameterManager* pM, TsExtensionManager* eM, TsMaterialManager* mM, TsGeometryManager* gM,
                                       TsVGeometryComponent* parentComponent, G4VPhysicalVolume* parentVolume, G4String& name):
TsVGeometryComponent(pM, eM, mM, gM, parentComponent, parentVolume, name)
{;}

MyCollimator::~MyCollimator() {;}

G4VPhysicalVolume* MyCollimator::Construct() {
    BeginConstruction();

//  const G4String OpeningMaterial = fPm->ParameterExists(GetFullParmName("OpeningMaterial")) ? fPm->GetStrkngParameter(GetFullParmName("OpeningMaterial")) : "G4_AIR";
    const G4String CollimatorMaterial = fPm->GetStringParameter(GetFullParmName("Material"));

    const G4int AxisXCuts = fPm->ParameterExists(GetFullParmName("AxisXCuts")) ? fPm->GetIntegerParameter(GetFullParmName("AxisXCuts")) : 1;
    const G4int AxisYCuts = fPm->ParameterExists(GetFullParmName("AxisYCuts")) ? fPm->GetIntegerParameter(GetFullParmName("AxisYCuts")) : 1;
    const G4int AxisZCuts = fPm->ParameterExists(GetFullParmName("AxisZCuts")) ? fPm->GetIntegerParameter(GetFullParmName("AxisZCuts")) : 1;

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

    const G4double ThicknessAxisXPercentage = fPm->ParameterExists(GetFullParmName("ThicknessAxisXPercentage")) ? fPm->IGetUnitlessParameter(GetFullParmName("ThicknessAxisXPercentage")) : 0;
    const G4double ThicknessAxisYPercentage = fPm->ParameterExists(GetFullParmName("ThicknessAxisYPercentage")) ? fPm->IGetUnitlessParameter(GetFullParmName("ThicknessAxisYPercentage")) : 0;
    const G4double ThicknessAxisZPercentage = fPm->ParameterExists(GetFullParmName("ThicknessAxisZPercentage")) ? fPm->IGetUnitlessParameter(GetFullParmName("ThicknessAxisZPercentage")) : 0;

    const G4double HLXOfDummyBox = (1 - ThicknessAxisXPercentage) * HLX / AxisXCuts;
    const G4double HLYOfDummyBox = (1 - ThicknessAxisYPercentage) * HLY / AxisYCuts;
    const G4double HLZOfDummyBox = (1 - ThicknessAxisZPercentage) * HLZ / AxisZCuts;

    G4cerr << "Found that HLX, HLY, HLZ = " << HLX << ' ' << HLY << ' ' << HLZ << '\n';

    G4cerr << "For the dummy box HLX, HLY, HLZ = " << HLXOfDummyBox << ' ' << HLYOfDummyBox << ' ' << HLZOfDummyBox << '\n';

    G4VSolid* AnsBox = new G4Box(fName, HLX, HLY, HLZ);

    for (int i = 0;i < AxisXCuts;i++) {
        const G4double XCenter = (2 * i + 1) * (HLX / AxisXCuts) - HLX;
        for (int j = 0;j < AxisYCuts;j++) {
            const G4double YCenter = (2 * j + 1) * (HLY / AxisYCuts) - HLY;
            for (int k = 0;k < AxisZCuts;k++) {
                const G4double ZCenter = (2 * k + 1) * (HLZ / AxisZCuts) - HLZ;
                const G4ThreeVector BoxOffsets = G4ThreeVector(XCenter, YCenter, ZCenter);

                G4VSolid* DeletedBox = new G4Box("Dummy Box", HLXOfDummyBox, HLYOfDummyBox, HLZOfDummyBox);
                AnsBox = new G4SubtractionSolid(fName, AnsBox, DeletedBox, 0, BoxOffsets);

                G4cerr << "AnsBox is = " << *AnsBox << G4endl;
            }
        }
    }
    
    fEnvelopeLog = CreateLogicalVolume(fName, CollimatorMaterial, AnsBox);
    fEnvelopePhys = CreatePhysicalVolume(fEnvelopeLog);

    return fEnvelopePhys;
}
