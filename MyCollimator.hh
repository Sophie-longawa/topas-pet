#ifndef MyCollimator_hh
#define MyCollimator_hh

#include "TsVGeometryComponent.hh"

class MyCollimator : public TsVGeometryComponent
{
public:
    MyCollimator(TsParameterManager* pM, TsExtensionManager* eM, TsMaterialManager* mM, TsGeometryManager* gM,
                TsVGeometryComponent* parentComponent, G4VPhysicalVolume* parentVolume, G4String& name);
    ~MyCollimator();

    G4VPhysicalVolume* Construct();

};

#endif