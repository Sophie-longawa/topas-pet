// Extension for implementing the collimator like used in SPECT

#ifndef MyCollimator_hh
#define MyCollimator_hh

#include "TsVGeometryComponent.hh"

class MyCollimatorPVPlacement : public TsVGeometryComponent {
public:
    MyCollimatorPVPlacement(TsParameterManager* pM, TsExtensionManager* eM, TsMaterialManager* mM, TsGeometryManager* gM,
					   TsVGeometryComponent* parentComponent, G4VPhysicalVolume* parentVolume, G4String& name);
    ~MyCollimatorPVPlacement();
    G4VPhysicalVolume* Construct();
};

#endif

