import math
import pyperclip

num_blocks = 200

crystals_horiz = 8
crystals_vert = 8

margin_horiz = .5 #mm
margin_vert  = .5 #mm

crystal_lx = 4.05 #mm
crystal_ly = 4.39 #mm
crystal_lz = 30 #mm

time_resolution = 10 #ns
experiment_length = 10**5 #ns







num_time_bins = math.ceil(experiment_length/time_resolution)

angle_increment = math.pi * (num_blocks - 2) / num_blocks

result = f""" 
s:Tf/TimeSliceNum/Function      = "Step"
dv:Tf/TimeSliceNum/Times        = {num_time_bins} {" ".join([str(i*time_resolution) for i in range(0,num_time_bins)])} ns
iv:Tf/TimeSliceNum/Values       = {num_time_bins} {" ".join([str(i) for i in range(0,num_time_bins)])}

s:Ge/PETScannerRing/Type="Group"
s:Ge/PETScannerRing/Parent = "World"
d:Ge/PETScannerRing/TransX=0. m
d:Ge/PETScannerRing/TransY=0. m
d:Ge/PETScannerRing/TransZ=0. m
d:Ge/PETScannerRing/RotX=0. deg
d:Ge/PETScannerRing/RotY=0. deg
d:Ge/PETScannerRing/RotZ=0. deg
i:Ge/PETScannerRing/NumBlocks = {num_blocks}
d:Ge/PETScannerRing/BlockLX               = {crystals_vert * crystal_lx + (crystals_vert+1) * margin_vert} mm
d:Ge/PETScannerRing/R                     = Ge/PETScannerRing/BlockLX mm * {.5 / math.sin(math.pi / num_blocks)}
"""

for block_num in range(0,num_blocks):

	result += f"""
s:Ge/CrystalGroup{block_num}/Type                  = "Group"
s:Ge/CrystalGroup{block_num}/Parent                = "PETScannerRing"
d:Ge/CrystalGroup{block_num}/TransX                = Ge/PETScannerRing/R mm * {math.sin(math.pi * 2 * (block_num/num_blocks))}
d:Ge/CrystalGroup{block_num}/TransY                = 0. m
d:Ge/CrystalGroup{block_num}/TransZ                = Ge/PETScannerRing/R mm * {math.cos(math.pi * 2 * (block_num/num_blocks))}
d:Ge/CrystalGroup{block_num}/RotX                  = 0. deg
d:Ge/CrystalGroup{block_num}/RotY                  = {angle_increment * block_num} rad
d:Ge/CrystalGroup{block_num}/RotZ                  = 0. deg
d:Ge/CrystalGroup{block_num}/MarginHoriz           = .5 mm #made up number
d:Ge/CrystalGroup{block_num}/MarginVert            = .5 mm #made up number
i:Ge/CrystalGroup{block_num}/CrystalsHoriz         = {crystals_horiz}
i:Ge/CrystalGroup{block_num}/CrystalsVert          = {crystals_vert}
u:Ge/CrystalGroup{block_num}/NegHalfCrystalsHoriz  = -.5 * Ge/CrystalGroup{block_num}/CrystalsHoriz
u:Ge/CrystalGroup{block_num}/NegHalfCrystalsVert   = -.5 * Ge/CrystalGroup{block_num}/CrystalsHoriz
u:Ge/CrystalGroup{block_num}/FirstPosHoriz         = Ge/CrystalGroup/NegHalfCrystalsHoriz + .5
u:Ge/CrystalGroup{block_num}/FirstPosVert          = Ge/CrystalGroup/NegHalfCrystalsVert  + .5
d:Ge/CrystalGroup{block_num}/CrystalHLX            = .5 * Ge/CrystalGroup{block_num}/CrystalLX mm
d:Ge/CrystalGroup{block_num}/CrystalHLY            = .5 * Ge/CrystalGroup{block_num}/CrystalLY mm
d:Ge/CrystalGroup{block_num}/CrystalHLZ            = .5 * Ge/CrystalGroup{block_num}/CrystalLZ mm
d:Ge/CrystalGroup{block_num}/CrystalLX             = {crystal_lx} mm
d:Ge/CrystalGroup{block_num}/CrystalLY             = {crystal_ly} mm
d:Ge/CrystalGroup{block_num}/CrystalLZ             = {crystal_lz} mm
d:Ge/CrystalGroup{block_num}/DeltaX                = Ge/CrystalGroup{block_num}/CrystalLX + Ge/CrystalGroup/MarginVert mm
d:Ge/CrystalGroup{block_num}/DeltaY                = Ge/CrystalGroup{block_num}/CrystalLY + Ge/CrystalGroup/MarginHoriz mm
s:Ge/CrystalGroup{block_num}/CrystalMaterial       = "BGO"
	"""


	for row in range(0,crystals_vert):
		for col in range(0,crystals_horiz):

			result += f"""
s:Ge/Crystal{block_num}r{row}c{col}/Type                    = "TsBox"
s:Ge/Crystal{block_num}r{row}c{col}/Parent                  = "CrystalGroup{block_num}"
s:Ge/Crystal{block_num}r{row}c{col}/Material                = Ge/CrystalGroup{block_num}/CrystalMaterial
d:Ge/Crystal{block_num}r{row}c{col}/TransX                  = Ge/CrystalGroup{block_num}/DeltaX mm * Ge/Crystal1r0c0/VertOffset
d:Ge/Crystal{block_num}r{row}c{col}/TransY                  = Ge/CrystalGroup{block_num}/DeltaY mm * Ge/Crystal1r0c0/HorizOffset
d:Ge/Crystal{block_num}r{row}c{col}/TransZ                  = Ge/CrystalGroup{block_num}/CrystalHLZ mm
d:Ge/Crystal{block_num}r{row}c{col}/RotX                    = 0 deg
d:Ge/Crystal{block_num}r{row}c{col}/RotY                    = 0 deg
d:Ge/Crystal{block_num}r{row}c{col}/RotZ                    = 0 deg
d:Ge/Crystal{block_num}r{row}c{col}/HLX                     = Ge/CrystalGroup{block_num}/CrystalHLX mm
d:Ge/Crystal{block_num}r{row}c{col}/HLY                     = Ge/CrystalGroup{block_num}/CrystalHLY mm
d:Ge/Crystal{block_num}r{row}c{col}/HLZ                     = Ge/CrystalGroup{block_num}/CrystalHLZ mm
u:Ge/Crystal{block_num}r{row}c{col}/HorizOffset             = Ge/CrystalGroup{block_num}/FirstPosHoriz + {col}
u:Ge/Crystal{block_num}r{row}c{col}/VertOffset              = Ge/CrystalGroup{block_num}/FirstPosVert + {row}


s:Sc/CrystalScore{block_num}r{row}c{col}/Quantity                  = "EnergyDeposit"
s:Sc/CrystalScore{block_num}r{row}c{col}/Component                 = "Crystal{block_num}r{row}c{col}"
s:Sc/CrystalScore{block_num}r{row}c{col}/OutputFile                = "CrystalScore{block_num}r{row}c{col}"
s:Sc/CrystalScore{block_num}r{row}c{col}/OutputType                = "csv"
b:Sc/CrystalScore{block_num}r{row}c{col}/OutputToConsole           = "False"
s:Sc/CrystalScore{block_num}r{row}c{col}/IfOutputFileAlreadyExists = "Overwrite"
s:Sc/CrystalScore{block_num}r{row}c{col}/SplitByTimeFeature        = "TimeSliceNum"
		    """
print("Copying...")
pyperclip.copy(result)
print("Done!")


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