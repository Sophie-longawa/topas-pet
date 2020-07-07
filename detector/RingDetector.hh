#pragma once

#include "TsVGeometryComponent.hh"

class RingDetector : public TsVGeometryComponent {
public:
    RingDetector(TsParameterManager* pM, TsExtensionManager* eM, TsMaterialManager* mM, TsGeometryManager* gM,
					   TsVGeometryComponent* parentComponent, G4VPhysicalVolume* parentVolume, G4String& name);

    ~RingDetector();
    G4VPhysicalVolume* Construct();
};

