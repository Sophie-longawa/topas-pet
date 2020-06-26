// Extension for implementing the collimator like used in SPECT

#pragma once

#include "TsVGeometryComponent.hh"

class MyCollimatorBooleanSolids : public TsVGeometryComponent {
public:
    MyCollimatorBooleanSolids(TsParameterManager* pM, TsExtensionManager* eM, TsMaterialManager* mM, TsGeometryManager* gM,
					   TsVGeometryComponent* parentComponent, G4VPhysicalVolume* parentVolume, G4String& name);
    ~MyCollimatorBooleanSolids();
    G4VPhysicalVolume* Construct();
};

