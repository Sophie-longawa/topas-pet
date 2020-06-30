#pragma once

#include "TsVGeometryComponent.hh"

class RingCollimator : public TsVGeometryComponent {
public:
    RingCollimator(TsParameterManager* pM, TsExtensionManager* eM, TsMaterialManager* mM, TsGeometryManager* gM,
					   TsVGeometryComponent* parentComponent, G4VPhysicalVolume* parentVolume, G4String& name);

    ~RingCollimator();
    G4VPhysicalVolume* Construct();
};

