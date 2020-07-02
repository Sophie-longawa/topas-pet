// Component for PETScanner
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

#include "PETScanner.hh"

#include "TsParameterManager.hh"

#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4UIcommand.hh"
#include "G4SystemOfUnits.hh"
#include <math.h>
#include "G4VisAttributes.hh"

PETScanner::PETScanner(TsParameterManager* pM, TsExtensionManager* eM, TsMaterialManager* mM, TsGeometryManager* gM,
								 TsVGeometryComponent* parentComponent, G4VPhysicalVolume* parentVolume, G4String& name)
	: TsVGeometryComponent(pM, eM, mM, gM, parentComponent, parentVolume, name)
{;}

PETScanner::~PETScanner()
{;}


void PETScanner::UpdateForSpecificParameterChange(G4String parameter)
{
	TsVGeometryComponent::UpdateForSpecificParameterChange(parameter);

	/*

	if (parameter == GetFullParmNameLower("XPlusLeavesOpen") || parameter == GetFullParmNameLower("XMinusLeavesOpen")) {
		XPlusLeavesOpen  = fPm->GetDoubleVector(GetFullParmName("XPlusLeavesOpen"), "Length");
		XMinusLeavesOpen = fPm->GetDoubleVector(GetFullParmName("XMinusLeavesOpen"), "Length");
		G4ThreeVector xpl;
		G4ThreeVector xml;
		for (int i= 0; i < NbOfLeavesPerSide; ++i) {
			if ( (XPlusLeavesOpen[i] - XMinusLeavesOpen[i]) < 0.0 ) {
				G4cerr << "Detected Leaf collision at "<< i <<" th leaf: XPlusLeaf "
				<< XPlusLeavesOpen[i]/cm << " (cm), XMinusLeaf "
				<< XMinusLeavesOpen[i]/cm << " (cm) "<< G4endl;
				exit(1);
			}

			xpl = (XPlusLeaves[i])->GetTranslation();
			xml = (XMinusLeaves[i])->GetTranslation();
			xpl.setX(LeafHalfLength + XPlusLeavesOpen[i]);
			xml.setX((-1)*LeafHalfLength + XMinusLeavesOpen[i]);
			(XPlusLeaves[i])->SetTranslation(xpl);
			(XMinusLeaves[i])->SetTranslation(xml);
		}
		AddToReoptimizeList(fEnvelopeLog);
	} else {
		// For any other parameters, fall back to the base class Update method
		TsVGeometryComponent::UpdateForSpecificParameterChange(parameter);
	}
	*/
}


G4VPhysicalVolume* PETScanner::Construct()
{
	BeginConstruction();


	//Crystals.clear();

	//load in parameters

	NbDetectorBlocksPerRing = fPm->GetIntegerParameter(GetFullParmName("DetectorBlocksPerRing"));
	NbRings                 = fPm->GetIntegerParameter(GetFullParmName("Rings"));

	NbCrystalsHorizontal    = fPm->GetIntegerParameter(GetFullParmName("CrystalsHorizontal"));
	NbCrystalsVertical      = fPm->GetIntegerParameter(GetFullParmName("CrystalsVertical"));

	MarginHorizontal        = fPm->GetDoubleParameter(GetFullParmName("MarginHorizontal"), "Length");
	MarginVertical          = fPm->GetDoubleParameter(GetFullParmName("MarginVertical"), "Length");

	MarginRings             = fPm->GetDoubleParameter(GetFullParmName("MarginRings"), "Length");

	MarginBlocks            = fPm->GetDoubleParameter(GetFullParmName("MarginBlocks"), "Length");

	Crystallx               = fPm->GetDoubleParameter(GetFullParmName("CrystalLX"), "Length");
	Crystally               = fPm->GetDoubleParameter(GetFullParmName("CrystalLY"), "Length");
	Crystallz               = fPm->GetDoubleParameter(GetFullParmName("CrystalLZ"), "Length");

	//Crystallz               = fPm->GetDoubleParameter(GetFullParmName("CrystalLZ"), "Length");

	


	G4double detectorHLX = (Crystallx * NbCrystalsHorizontal + MarginHorizontal * (NbCrystalsHorizontal - 1)) * .5;
	G4double detectorHLY = Crystally * .5;
	G4double detectorHLZ = (Crystallz * NbCrystalsVertical   + MarginVertical   * (NbCrystalsVertical   - 1)) * .5;

	G4String envelopeMaterialName = fParentComponent->GetResolvedMaterialName();

	G4double ringRadius = (detectorHLX + MarginBlocks) / std::sin(M_PI/NbDetectorBlocksPerRing);



	G4String crystalMaterialName = GetResolvedMaterialName();

	G4VSolid* crystalSolid =  new G4Box("crystal", Crystallx*.5, Crystally*.5, Crystallz*.5);

	G4LogicalVolume* crystalLV = CreateLogicalVolume("crystal", crystalMaterialName, crystalSolid);





	G4VSolid* detectorSolid =  new G4Box("detector", detectorHLX, detectorHLY, detectorHLZ);

	G4LogicalVolume* detectorLV = CreateLogicalVolume("detector", envelopeMaterialName, detectorSolid);

	G4VisAttributes* yokeColor = new G4VisAttributes(G4Colour(0.2, 1.0, 0.2)); // Sets RGB color
	RegisterVisAtt(yokeColor); // Necessary so that TOPAS can delete the attribute if the component is rebuild during 4D behavior
	detectorLV->SetVisAttributes(yokeColor);

	G4double angleIncrement = M_PI * 2 / NbDetectorBlocksPerRing;

	G4VSolid* encapuslatingTube = new G4Tubs("petScanner", ringRadius,    // inner radius
											 (ringRadius + Crystally) * std::cos(M_PI/NbDetectorBlocksPerRing),    // outer radius
											 (NbRings*.5 + .5) * (detectorHLZ * 2 + MarginRings) * 2,    // height
											 0.0,  2 * M_PI);  // segment angles

	fEnvelopeLog = CreateLogicalVolume(fName, envelopeMaterialName, encapuslatingTube);
	fEnvelopePhys = CreatePhysicalVolume(fEnvelopeLog);



	/*
	G4VSolid* crystalTube = new G4Tubs("frick", (ringRadius + (ringRadius + Crystallz) * std::cos(M_PI/NbDetectorBlocksPerRing))/2,    // inner radius
											 (ringRadius + Crystallz) * std::cos(M_PI/NbDetectorBlocksPerRing),    // outer radius
											 (NbRings*.5 + .5) * (detectorHLY * 2 + MarginRings),    // height
											 0.0,  2 * M_PI);  // segment angles

	G4LogicalVolume* tubeLV = CreateLogicalVolume("frick", crystalMaterialName, crystalTube);

	CreatePhysicalVolume("frick", tubeLV, fEnvelopePhys);
	*/

	G4int ringNum = 0;
	G4int detectorNum = 0;
	G4int crystalNum = 0;

	for(int ring_index = 0; ring_index < NbRings; ring_index++) {

		G4double detectorTransZ = (-NbRings*.5 + .5 + ring_index) * (detectorHLZ * 2 + MarginRings);

		for(int detector_index = 0; detector_index < 1/*NbDetectorBlocksPerRing*/; detector_index++) {

			G4double detectorTransX = (ringRadius + detectorHLY) * (std::sin(-(M_PI * 2 * (detector_index / NbDetectorBlocksPerRing) + M_PI / 2)));//Ge/PETScannerRing/R mm * {math.sin(-(math.pi * 2 * (block_num/num_blocks) + math.pi / 2))}
			//y defined above
			G4double detectorTransY = (ringRadius + detectorHLY) * (std::cos(-(M_PI * 2 * (detector_index / NbDetectorBlocksPerRing) + M_PI / 2)));//Ge/PETScannerRing/R mm * {math.cos(-(math.pi * 2 * (block_num/num_blocks) + math.pi / 2))}

			G4double rotationAngle = M_PI * .5 + angleIncrement * detector_index;

			G4RotationMatrix detectorRotation = G4RotationMatrix();
			detectorRotation.rotateY(rotationAngle);

			G4ThreeVector trans(detectorTransX, detectorTransY, detectorTransZ);

			G4cout << "Making detector: " << detectorNum << G4endl;

			G4String detector_string = "detector"+G4UIcommand::ConvertToString(detectorNum);

			G4VPhysicalVolume* detectorPV = CreatePhysicalVolume(detector_string, detectorNum++, true, detectorLV, &detectorRotation, &trans, fEnvelopePhys);


			for(int row = 0; row < 1/*NbCrystalsVertical*/; row++) {
				for(int col = 0; col < 1/*NbCrystalsVertical*/; col++) {

					//int n = sprintf (crystalNameBuffer, "%sr%dd%dr%dc%d", crystalName, ring_index, detector_index, row, col);

					G4RotationMatrix crystalRotation = G4RotationMatrix();//no rotation

					G4double xOffset = Crystallx + MarginHorizontal;
					G4double zOffset = Crystallz + MarginVertical;

					G4double numXOffsets = (-0.5 * NbCrystalsHorizontal + .5) + col;
					G4double numZOffsets = (-0.5 * NbCrystalsVertical   + .5) + row;

					G4double xPos = numXOffsets * xOffset;
					G4double yPos = 0;
					G4double zPos = numZOffsets * zOffset;//Crystallz * .5;

					G4ThreeVector crystalPos(xPos, yPos, zPos);

					G4cout << "Making crystal: " << crystalNum << G4endl;

					G4String crystalVolName = "crystal"+G4UIcommand::ConvertToString(crystalNum);

					CreatePhysicalVolume(crystalVolName, crystalNum++, true, crystalLV, &crystalRotation, &crystalPos, detectorPV);


				}
			}



		}
	}

	return fEnvelopePhys;


































	/*

	XPlusLeaves.clear();
	XMinusLeaves.clear();


	//Count leaves
	NbOfLeavesPerSide = fPm->GetVectorLength(GetFullParmName("Widths"));
	const G4int n_xl = fPm->GetVectorLength(GetFullParmName("XPlusLeavesOpen"));
	const G4int n_xr = fPm->GetVectorLength(GetFullParmName("XMinusLeavesOpen"));
	if (NbOfLeavesPerSide != n_xl) {
		G4cerr << "Number of Width does not Match to XPlusLeaves: Widths = " << NbOfLeavesPerSide << ", XPlusLeavesOpen = "<< n_xl << G4endl;
		exit(1);
	} else {
		if (NbOfLeavesPerSide != n_xr) {
				G4cerr << "Number of Width does not Match to XMinusLeaves: Widths = " << NbOfLeavesPerSide << ", XPlusLeavesOpen = "<< n_xr << G4endl;
				exit(1);
		}
	}

	//LeafHalfLength: x of leaf, leaf_thickness: z of leaf, and vt: various y of leaf
	LeafHalfLength          = (0.5)*fPm->GetDoubleParameter(GetFullParmName("Length"), "Length"); //x
	G4double leaf_thickness = fPm->GetDoubleParameter(GetFullParmName("Thickness"), "Length"); //z
	G4double*     vt        = fPm->GetDoubleVector(GetFullParmName("Widths"), "Length");  //y
	if ( fPm->ParameterExists(GetFullParmName("MaximumLeafOpen")) ) {
	MaximumLeafOpen = fPm->GetDoubleParameter(GetFullParmName("MaximumLeafOpen"), "Length");
	} else {
	MaximumLeafOpen = 2.0*LeafHalfLength;
	}

	XPlusLeavesOpen  = fPm->GetDoubleVector(GetFullParmName("XPlusLeavesOpen"), "Length");
	XMinusLeavesOpen = fPm->GetDoubleVector(GetFullParmName("XMinusLeavesOpen"), "Length");

	G4double y_sum = 0.0;
	for (G4int i = 0; i < NbOfLeavesPerSide; ++i) {
	y_sum += vt[i];
	//Prevent leaf open over its limit.
	if ( fabs(XPlusLeavesOpen[i]) > MaximumLeafOpen || fabs(XMinusLeavesOpen[i]) > MaximumLeafOpen) {
			G4cerr << i << " th Leaf tried to open more than the limit: " << MaximumLeafOpen/cm << " (cm)" << G4endl;
			exit(1);
	}
	//Leaf collision detection.
	if ( (XPlusLeavesOpen[i] - XMinusLeavesOpen[i]) < 0.0 ) {
			G4cerr << "Detected Leaf collision at "<< i <<" th leaf: XPlusLeaf "
						 << XPlusLeavesOpen[i]/cm << " (cm), XMinusLeaf "
						 << XMinusLeavesOpen[i]/cm << " (cm) "<< G4endl;
			exit(1);
	}
	}

	G4String envelopeMaterialName = fParentComponent->GetResolvedMaterialName();
	G4Box* svWholeBox = new G4Box(fName, LeafHalfLength*2 + MaximumLeafOpen, 0.5*y_sum, 0.5*leaf_thickness);
	fEnvelopeLog = CreateLogicalVolume(fName, envelopeMaterialName, svWholeBox);
	fEnvelopePhys = CreatePhysicalVolume(fEnvelopeLog);

	G4double y_cpos = (0.5*y_sum);
	for (G4int i = 0; i < NbOfLeavesPerSide; ++i)
	{
	if (i == 0) y_cpos = y_cpos - 0.5 * vt[i];
	else        y_cpos = y_cpos - 0.5 * (vt[i-1] + vt[i]);

	G4String id_string = G4UIcommand::ConvertToString(i);
	G4Box* svLeaf = new G4Box("gLeaf", LeafHalfLength, 0.5 * vt[i], leaf_thickness*0.5);
	G4LogicalVolume* lLeaf  = CreateLogicalVolume(svLeaf);
		G4String volName = "X+Leaf"+id_string;
		G4ThreeVector* threeVecPlus = new G4ThreeVector(LeafHalfLength + XPlusLeavesOpen[i] , y_cpos, 0.0);
		G4VPhysicalVolume* pPlusLeaf = CreatePhysicalVolume(volName, lLeaf, 0, threeVecPlus, fEnvelopePhys);
		volName = "X-Leaf"+id_string;
		G4ThreeVector* threeVecMinus = new G4ThreeVector((-1)*LeafHalfLength + XMinusLeavesOpen[i] , y_cpos, 0.0);
		G4VPhysicalVolume* pMinusLeaf = CreatePhysicalVolume(volName, lLeaf, 0, threeVecMinus, fEnvelopePhys);
		XPlusLeaves.push_back(pPlusLeaf);
	XMinusLeaves.push_back(pMinusLeaf);
	}
	return fEnvelopePhys;
	*/
}
