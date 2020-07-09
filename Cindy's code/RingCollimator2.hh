#ifndef RingCollimator2_hh
#define RingCollimator2_hh

#include "TsVGeometryComponent.hh"

class RingCollimator2 : public TsVGeometryComponent
{
public:
    RingCollimator2(TsParameterManager* pM, TsExtensionManager* eM, TsMaterialManager* mM, TsGeometryManager* gM,
                TsVGeometryComponent* parentComponent, G4VPhysicalVolume* parentVolume, G4String& name);
    ~RingCollimator2();

    G4VPhysicalVolume* Construct();

};

#endif

