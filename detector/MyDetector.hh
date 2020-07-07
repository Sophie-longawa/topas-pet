#pragma once

#include "TsVGeometryComponent.hh"

class MyDetector : public TsVGeometryComponent {
public:
    MyDetector(TsParameterManager* pM, TsExtensionManager* eM, TsMaterialManager* mM, TsGeometryManager* gM,
					   TsVGeometryComponent* parentComponent, G4VPhysicalVolume* parentVolume, G4String& name);
    ~MyDetector();
    G4VPhysicalVolume* Construct();
};


