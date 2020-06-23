num_blocks = 200








result = f"""
s:Ge/PETScannerRing/Type="Group"
s:Ge/PETScannerRing/Parent = "World"
d:Ge/PETScannerRing/TransX=0. m
d:Ge/PETScannerRing/TransY=0. m
d:Ge/PETScannerRing/TransZ=0. m
d:Ge/PETScannerRing/RotX=0. deg
d:Ge/PETScannerRing/RotY=0. deg
d:Ge/PETScannerRing/RotZ=0. deg
i:Ge/PETScannerRing/NumBlocks = {num_blocks}
"""



"""
s:Ge/PETScannerRing/Type="Group"
s:Ge/PETScannerRing/Parent = "World"
d:Ge/PETScannerRing/TransX=2. m
d:Ge/PETScannerRing/TransY=2. m
d:Ge/PETScannerRing/TransZ=0. m
d:Ge/PETScannerRing/RotX=0. deg
d:Ge/PETScannerRing/RotY=0. deg
d:Ge/PETScannerRing/RotZ=0. deg
i:Ge/PETScannerRing/NumBlocks = 200


s:Ge/CrystalGroup1/Type="Group"
s:Ge/CrystalGroup1/Parent = "PETScannerRing"
d:Ge/CrystalGroup1/TransX=2. m
d:Ge/CrystalGroup1/TransY=2. m
d:Ge/CrystalGroup1/TransZ=0. m
d:Ge/CrystalGroup1/RotX=0. deg
d:Ge/CrystalGroup1/RotY=0. deg
d:Ge/CrystalGroup1/RotZ=0. deg
d:Ge/CrystalGroup1/MarginHoriz = .5 mm #made up number
d:Ge/CrystalGroup1/MarginVert = .5 mm #made up number
i:Ge/CrystalGroup1/CrystalsHoriz = 8
i:Ge/CrystalGroup1/CrystalsVert  = 8
u:Ge/CrystalGroup1/NegHalfCrystalsHoriz  = -.5 * Ge/CrystalGroup/CrystalsHoriz
u:Ge/CrystalGroup1/NegHalfCrystalsVert   = -.5 * Ge/CrystalGroup/CrystalsHoriz
u:Ge/CrystalGroup1/FirstPosHoriz  = Ge/CrystalGroup/NegHalfCrystalsHoriz + .5
u:Ge/CrystalGroup1/FirstPosVert   = Ge/CrystalGroup/NegHalfCrystalsVert  + .5
d:Ge/CrystalGroup1/CrystalHLX = .5 * Ge/CrystalGroup/CrystalLX mm
d:Ge/CrystalGroup1/CrystalHLY = .5 * Ge/CrystalGroup/CrystalLY mm
d:Ge/CrystalGroup1/CrystalHLZ = .5 * Ge/CrystalGroup/CrystalLZ mm
d:Ge/CrystalGroup1/CrystalLX = 4.05 mm
d:Ge/CrystalGroup1/CrystalLY = 4.39 mm
d:Ge/CrystalGroup1/CrystalLZ = 30 mm
d:Ge/CrystalGroup1/DeltaX = Ge/CrystalGroup/CrystalLX + Ge/CrystalGroup/MarginVert mm
d:Ge/CrystalGroup1/DeltaY = Ge/CrystalGroup/CrystalLY + Ge/CrystalGroup/MarginHoriz mm
s:Ge/CrystalGroup1/CrystalMaterial = "BGO"


s:Ge/Crystal1r0c0/Type                    = "TsBox"
s:Ge/Crystal1r0c0/Parent                  = "World"
s:Ge/Crystal1r0c0/Material                = Ge/CrystalGroup/CrystalMaterial
d:Ge/Crystal1r0c0/TransX                  = Ge/CrystalGroup/DeltaX mm * Ge/Crystal1r0c0/VertOffset
d:Ge/Crystal1r0c0/TransY                  = Ge/CrystalGroup/DeltaY mm * Ge/Crystal1r0c0/HorizOffset
d:Ge/Crystal1r0c0/TransZ                  = 0 cm
d:Ge/Crystal1r0c0/RotX                    = 0 deg
d:Ge/Crystal1r0c0/RotY                    = 0 deg
d:Ge/Crystal1r0c0/RotZ                    = 0 deg
d:Ge/Crystal1r0c0/HLX                     = Ge/CrystalGroup/CrystalHLX mm
d:Ge/Crystal1r0c0/HLY                     = Ge/CrystalGroup/CrystalHLY mm
d:Ge/Crystal1r0c0/HLZ                     = Ge/CrystalGroup/CrystalHLZ mm
u:Ge/Crystal1r0c0/HorizOffset             = Ge/CrystalGroup/FirstPosHoriz + 0
u:Ge/Crystal1r0c0/VertOffset              = Ge/CrystalGroup/FirstPosVert + 0
"""