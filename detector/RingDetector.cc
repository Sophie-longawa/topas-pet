// Component for RingDetector


#include "RingDetector.hh"

#include "TsParameterManager.hh"

#include "G4Box.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4Colour.hh"
#include "G4Visible.hh"

#include <cmath>
#include <cstring>

RingDetector::RingDetector(TsParameterManager* pM, TsExtensionManager* eM, TsMaterialManager* mM, TsGeometryManager* gM,
                                       TsVGeometryComponent* parentComponent, G4VPhysicalVolume* parentVolume, G4String& name):
TsVGeometryComponent(pM, eM, mM, gM, parentComponent, parentVolume, name)
{;}

RingDetector::~RingDetector() {;}

G4VPhysicalVolume* RingDetector::Construct(void) {
    G4cerr << "Starting constructing ring detector" << fName << G4endl;

    BeginConstruction();

    const G4int XBins = fPm->ParameterExists(GetFullParmName("XBins")) ?
                            fPm->GetIntegerParameter(GetFullParmName("XBins")) : 1;
    const G4int YBins = fPm->ParameterExists(GetFullParmName("YBins")) ?
                            fPm->GetIntegerParameter(GetFullParmName("YBins")) : 1;
    const G4int ZBins = fPm->ParameterExists(GetFullParmName("ZBins")) ?
                            fPm->GetIntegerParameter(GetFullParmName("ZBins")) : 1;

    G4cerr << "The cuts are: " << XBins << ' ' << YBins << ' ' << ZBins << '\n';

    if (XBins <= 0) {
        G4cout << "Error: AxisXCuts should be a positive integer, see: " << GetFullParmName("XBins") << G4endl;
        exit(1);
    }

    if (YBins <= 0) {
        G4cout << "Error: AxisYCuts should be a positive integer, see: " << GetFullParmName("YBins") << G4endl;
        exit(1);
    }

    if (ZBins <= 0) {
        G4cout << "Error: AxisZCuts should be a positive integer, see: " << GetFullParmName("ZBins") << G4endl;
        exit(1);
    }

    const G4double RingRadius = fPm->GetDoubleParameter(GetFullParmName("RingRadius"), "Length");
    
    /**
     * RingRadius is the distance between the center of the ring to
     * the closest possible solid.
     *
     * The Z axis of the detector will represent the wideness of the ring
     * The Y axis of the detector will represent the length of the detector
     * The X axis of the detector will be the axis tangent to the ring if
     * we ignoress the wideness of the tube so that the we can
     * pass a line perpendicular of radiu RingRadiu to the plane formed 
     * by axis X & Z
     *
     * The total length of the detector will be represent by both the length
     * of crystall and collimator (if present), i.e. it's equal to 
     * (CrystallHLY + CollimatorHLY) * 2
     *
     * Since both the crystall and detector will share surface of X & Z axis
     * both of them will share axis X & Z length
     * 
     * To obtain a different rotation configuration use the topas
     * RotX, RotY & RotZ
     *
     * If the number of detectors NbOfDetectors is set to -1, it will automatically
     * infer the maximum number of detectors possible to align in the ring
     * with fixed HLX & HLZ
     */
    
    const G4double HLX = fPm->GetDoubleParameter(GetFullParmName("HLX"), "Length");
    const G4double CrystallHLY = fPm->GetDoubleParameter(GetFullParmName("CrystallHLY"), "Length");
    const G4double HLZ = fPm->GetDoubleParameter(GetFullParmName("HLZ"), "Length");

    const G4String DetectorType = fPm->GetStringParameter(GetFullParmName("DetectorType"));

    G4cerr << "The detector type is: " << DetectorType << G4endl;

    const bool CollimatorExists = DetectorType == "PET" ? false : true;

    G4int NbOfDetectors = fPm->GetIntegerParameter(GetFullParmName("NbOfDetectors"));

    if (NbOfDetectors < 0) {
        NbOfDetectors = floor(M_PI / asin(HLX / RingRadius));
        
        G4cerr << "Infered that NbOfDetectors = " << NbOfDetectors << G4endl;
    }

    if (NbOfDetectors == 0) {
        G4cout << "Error: NbOfDetectors should not be equal to 0 either implicitely or explicitely, see: " << GetFullParmName("NbOfDetectors") << G4endl;
        exit(1);
    }

    const G4double CollimatorHLY = CollimatorExists ? fPm->GetDoubleParameter(GetFullParmName("CollimatorHLY"), "Length") : 0;

    const G4double ThicknessAxisXPercentage = fPm->ParameterExists(GetFullParmName("ThicknessAxisXPercentage")) ? 
                                              fPm->IGetUnitlessParameter(GetFullParmName("ThicknessAxisXPercentage")) : 0;
        
    const G4double ThicknessAxisYPercentage = fPm->ParameterExists(GetFullParmName("ThicknessAxisYPercentage")) ? 
                                              fPm->IGetUnitlessParameter(GetFullParmName("ThicknessAxisYPercentage")) : 0;
    
    const G4double ThicknessAxisZPercentage = fPm->ParameterExists(GetFullParmName("ThicknessAxisZPercentage")) ? 
                                              fPm->IGetUnitlessParameter(GetFullParmName("ThicknessAxisZPercentage")) : 0;

    G4String OpeningMaterial = fPm->ParameterExists(GetFullParmName("OpeningMaterial")) ?
                               fPm->GetStringParameter(GetFullParmName("OpeningMaterial")) : fParentComponent->GetResolvedMaterialName();

    G4String CollimatorMaterial = CollimatorExists ? fPm->GetStringParameter(GetFullParmName("CollimatorMaterial")) : ""; 
                        //If collimator does not exist the we put a whatever material we want since we're not going to use the collimator

    G4String CrystallMaterial = fPm->GetStringParameter(GetFullParmName("CrystallMaterial"));
    G4String TubeMaterial = fPm->GetStringParameter(GetFullParmName("Material"));
        
    const G4double HLXOfOpening = (1 - ThicknessAxisXPercentage) * HLX / XBins;
    const G4double HLYOfOpening = (1 - ThicknessAxisYPercentage) * CollimatorHLY / YBins;
    const G4double HLZOfOpening = (1 - ThicknessAxisZPercentage) * HLZ / ZBins;

    const G4double CrystallBoxHLX = HLX / XBins;
    const G4double CrystallBoxHLY = CrystallHLY;
    const G4double CrystallBoxHLZ = HLZ / ZBins;

    G4VSolid* CollimatorBox = CollimatorExists ? new G4Box("Collimator Box Solid", HLX, CollimatorHLY, HLZ) : NULL;
    G4VSolid* OpeningBox = CollimatorExists ? new G4Box("Opening Box Solid", HLXOfOpening, HLYOfOpening, HLZOfOpening) : NULL;

    G4VSolid* Crystall = new G4Box("Crystall Solid", HLX, CrystallHLY, HLZ);
    G4LogicalVolume* CrystallLogicalVolume = CreateLogicalVolume(Crystall);

    G4VisAttributes* yokeColor = new G4VisAttributes(G4Colour(0, 0, 0));
    RegisterVisAtt(yokeColor); 
    CrystallLogicalVolume->SetVisAttributes(yokeColor);

    G4VSolid* CrystallBox = new G4Box("Crystall Box Solid", CrystallBoxHLX, CrystallBoxHLY, CrystallBoxHLZ);
    G4LogicalVolume* CrystallBoxLogicalVolume = CreateLogicalVolume("Crystall Box Logical Volume", CrystallMaterial, CrystallBox);
    yokeColor = new G4VisAttributes(G4Colour::Cyan());
    RegisterVisAtt(yokeColor);
    CrystallBoxLogicalVolume->SetVisAttributes(yokeColor);

    G4LogicalVolume* CollimatorBoxLogicalVolume = CollimatorExists ? CreateLogicalVolume("Collimator Box Logical Volume", CollimatorMaterial, CollimatorBox) : NULL;
    if (CollimatorExists) {
        yokeColor = new G4VisAttributes(G4Colour::Yellow());
        RegisterVisAtt(yokeColor);
        CollimatorBoxLogicalVolume->SetVisAttributes(yokeColor);
    }

    G4LogicalVolume* OpeningBoxLogicalVolume = CollimatorExists ? CreateLogicalVolume("Opening Box Logical Volume", OpeningMaterial, OpeningBox) : NULL;
    if (CollimatorExists) {
        yokeColor = new G4VisAttributes(G4Colour(255.0 / 255, 192.0 / 255, 203.0 / 255));
        RegisterVisAtt(yokeColor);
        OpeningBoxLogicalVolume->SetVisAttributes(yokeColor);
    }
    
    G4VSolid* EmptyTube = new G4Tubs("Ring Solid", 0, (RingRadius + 2 * CrystallHLY + 2 * CollimatorHLY), 2 * HLZ, 0, 2 * M_PI);
    fEnvelopeLog = CreateLogicalVolume("Ring Logical Volume", TubeMaterial, EmptyTube);
    yokeColor = new G4VisAttributes(G4Colour(0, 0, 0));
    RegisterVisAtt(yokeColor); 
    fEnvelopeLog->SetVisAttributes(yokeColor);

    fEnvelopePhys = CreatePhysicalVolume(fEnvelopeLog);

    if (CollimatorExists) {
        for (int collimator = 0;collimator < NbOfDetectors;++collimator) {
            const double Angle = 2 * M_PI * collimator / NbOfDetectors;

            const double TransX = std::cos(Angle) * (RingRadius + CollimatorHLY);
            const double TransY = std::sin(Angle) * (RingRadius + CollimatorHLY);

            G4ThreeVector* TransVector = new G4ThreeVector(TransX, TransY, 0);

            const G4double RotationAngle = 0.5 * M_PI - Angle;

            G4RotationMatrix* RotMatrix = new G4RotationMatrix;
            RotMatrix->rotateX(0);
            RotMatrix->rotateY(0);
            RotMatrix->rotateZ(RotationAngle);


            int openingID = 0;

            G4VPhysicalVolume* CurrentCollimator = CreatePhysicalVolume("Collimator Box Physical Volume", 
                                collimator, true, CollimatorBoxLogicalVolume, RotMatrix, TransVector, fEnvelopePhys);

            for (int i = 0;i < XBins;++i) {
                const G4double XCenter = (2 * i + 1) * (HLX / XBins) - HLX;
                for (int j = 0;j < YBins;++j) {
                    const G4double YCenter = (2 * j + 1) * (CollimatorHLY / YBins) - CollimatorHLY;
                    for (int k = 0;k < ZBins;++k) {
                        const G4double ZCenter = (2 * k + 1) * (HLZ / ZBins) - HLZ;
                        TransVector = new G4ThreeVector(XCenter, YCenter, ZCenter);
                        CreatePhysicalVolume("Opening Box Physical Volume", 
                                openingID++, true, OpeningBoxLogicalVolume, 0, TransVector, CurrentCollimator);
                    }
                }
            }
        }
    }

    for (int crystall = 0;crystall < NbOfDetectors;++crystall) {
        const double Angle = 2 * M_PI * crystall / NbOfDetectors;

        const double TransX = std::cos(Angle) * (RingRadius + 2 * CollimatorHLY + CrystallHLY);
        const double TransY = std::sin(Angle) * (RingRadius + 2 * CollimatorHLY + CrystallHLY);

        G4ThreeVector* TransVector = new G4ThreeVector(TransX, TransY, 0);

        const G4double RotationAngle = 0.5 * M_PI - Angle;

        G4RotationMatrix* RotMatrix = new G4RotationMatrix;
        RotMatrix->rotateX(0);
        RotMatrix->rotateY(0);
        RotMatrix->rotateZ(RotationAngle);

        G4VPhysicalVolume* CurrentCrystall = CreatePhysicalVolume("Crystall Physical Volume", 
                            crystall, true, CrystallLogicalVolume, RotMatrix, TransVector, fEnvelopePhys);

        int crystallBoxID = 0;
        for (int i = 0;i < XBins;++i) {
            const G4double XCenter = (2 * i + 1) * (HLX / XBins) - HLX;
            for (int j = 0;j < ZBins;++j) {
                const G4double ZCenter = (2 * j + 1) * (HLZ / ZBins) - HLZ;
                TransVector = new G4ThreeVector(XCenter, 0, ZCenter);
                CreatePhysicalVolume("Crystall Box Physical Volume",
                            crystallBoxID++, true, CrystallBoxLogicalVolume, 0, TransVector, CurrentCrystall);
            }
        }
    }

    return fEnvelopePhys;
}
