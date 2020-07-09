#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>


using namespace std;

// // DLS Parameters
#define  N_RINGS               18 // DLS: Number of rings
#define  N_SEG                 35 // DLS: Number of segments (2 N_RINGS - 1)
#define  N_DET                672 // DLS: Detectors per ring
#define  S_WIDTH              283 // DLS: Number of radial sinogram bins
#define  N_RSEC               56 // DLS: Number of resector
#define  N_MODULE             6 // DLS: Number of modules (2x3)
#define  N_MOD_xy             2 // DLS: Number of tangential modules
#define  N_MOD_z              3 // DLS: Number of axial modules
#define  N_SUBMOD             1 // DLS: Number of submodules (1x1)
#define  N_SMOD_xy            1 // DLS: Number of tangential submodules
#define  N_SMOD_z             1 // DLS: Number of axial submodules
#define  N_CRYSTAL            36 // DLS: Number of crystals (6x6)
#define  N_CRY_xy             6 // DLS: Number of tangential crystals
#define  N_CRY_z              6 // DLS: Number of axial crystals
#define  MAX_D_RING           11 // DLS: Maximum ring difference
#define  N_PLANES             265 // DLS: Total number of sinograms
#define  FOV                  550 // DLS: Width of the FOV (mm)
#define  SLICES_PER_FOV       35 // DLS: Number of slices per FOV
#define  USE_OFFSET             1 // DLS: On/Off use of offset
#define  OFFSET               499 // DLS: Sets initial sinogram angle


unsigned short ans,ans1;
unsigned short ans_SC, ans_RC, ans_SRC, ans_S, ans_R;

float Mich_r1r2fu[N_RINGS][N_RINGS][N_DET/2][S_WIDTH]={0}; 





int main()
{
  //---------------------------------------------------------------------------------
  // the first  argument (argv[1]) is the sub-directory of the input file
  // the second argument (argv[2]) is the name of the output file
  //---------------------------------------------------------------------------------
  const double pi = acos(-1.0);
  string filedir, inputfilename ;
  string filename, Moutputfilename, Poutputfilename ;

  //####################################################################
  //#             Declaration of leaves types - TTree Coincidences     #
  //####################################################################
  
 //  float axialPos, rotationAngle, sinogramS, sinogramTheta;
//   //Char_t          comptVolName1[40], comptVolName2[40];
//   ///Int_t           compton1, compton2;
//   Int_t           runID, sourceID1, sourceID2, eventID1, eventID2; 
//   Int_t           layerID1, layerID2, crystalID1, crystalID2;
//   Int_t           submoduleID1, submoduleID2, moduleID1, moduleID2, rsectorID1, rsectorID2;
//   Int_t           comptonPhantom1, comptonPhantom2;
//   Float_t         energy1, energy2;   
//   Float_t         globalPosX1, globalPosX2, globalPosY1, globalPosY2, globalPosZ1, globalPosZ2;
//   Float_t         sourcePosX1, sourcePosX2, sourcePosY1, sourcePosY2, sourcePosZ1, sourcePosZ2;
//   Double_t        time1, time2;
  

  //Utahscat600k_ca_seg4.s
   
  filename = "Utahscat600k_ca_seg4";//"PolP50per05ns";

  string line;
 
  ifstream inFile;
 
  // outFile.precision(12);
  //outFile.setf(ios::fixed);
  //outFile.setf(ios::showpoint);

  //outFile2.precision(12);
  //outFile2.setf(ios::fixed);
  //outFile2.setf(ios::showpoint);

  // filename = argv[1] ; 
  Moutputfilename = "Mich_"+ filename + ".s" ;
  cout << "Michelogram file name is = " << Moutputfilename << endl ;
  Poutputfilename = "Proj_"+ filename + ".s" ;
  cout << "Projection file name is = " << Poutputfilename << endl ;

  FILE  *Mich_r1r2fuFile, *Proj_File;      
  Mich_r1r2fuFile = fopen(Moutputfilename.c_str(),"wb");
  Proj_File       = fopen(Poutputfilename.c_str(),"wb");
   

  

  //inFile.open("~/topas_extensions/PET_git/topas-pet/scoring/example_sinogram/PolP50per05ns.txt");
  inFile.open("TextFiles/Utahscat600k_ca_seg4");//PolP50per05ns.txt");
  //outFile.open("Sinogram.txt");
  //
  

  if (!inFile)
    {
      cout << "Unable to open file" << endl;
      exit(1);
    }
  
                   
  //Read file into an array to analysis
  int eventID1 = 0;
  int eventID2 = 0;
  int crystalID1 = 0;
  int crystalID2 = 0;
  int blockID1 = 0;
  int blockID2 = 0;
  int moduleID1 = 0;
  int moduleID2 = 0;
  float energy1, energy2;

  int ring1, ring2;
  int crystal1, crystal2;
  int    phi, u;
  int    Counts = 0;  
  int flip, swap, zi, c1, c2;
 
    
  while (!inFile.eof())
    {
      //Read coincidence pair details:
      getline(inFile,line);
      inFile >> eventID1 >> crystalID1 >> blockID1 >> moduleID1 >> energy1 >> eventID2 >> crystalID2 >> blockID2 >> moduleID2 >> energy2; 

      
      //-----------------------------------
      //  Identify the ring#...
      //-----------------------
      ring1 = (int)(crystalID1/N_CRY_xy) 
	//+ (Int_t)(submoduleID1/N_SMOD_xy)*N_CRY_z
	    + (int)(blockID1/N_MOD_xy)*N_SMOD_z*N_CRY_z;
      ring2 = (int)(crystalID2/N_CRY_xy) 
	//  + (Int_t)(submoduleID2/N_SMOD_xy)*N_CRY_z
	    + (int)(blockID2/N_MOD_xy)*N_SMOD_z*N_CRY_z;

      if ( abs(ring1 - ring2) > MAX_D_RING )  continue;  
    

      //-----------------------
      //  Identify the crystal#...
      //-----------------------------------
      crystal1 = moduleID1 * N_MOD_xy * N_SMOD_xy * N_CRY_xy
	       + (blockID1%N_MOD_xy) * N_SMOD_xy * N_CRY_xy
	//+ (moduleID1%N_SMOD_xy) * N_CRY_xy
	       + (crystalID1%N_CRY_xy);
      crystal2 = moduleID2 * N_MOD_xy * N_SMOD_xy * N_CRY_xy
	       + (blockID2%N_MOD_xy) * N_SMOD_xy * N_CRY_xy
	//+ (submoduleID2%N_SMOD_xy) * N_CRY_xy
	       + (crystalID2%N_CRY_xy);
 
     
      //cout << "rings " << ring1 << " " << ring2 << endl;

     //-----------------------------------------------------
      //  Rotate the image correctly#...
      //--------------------------------
     //  if (USE_OFFSET == 1)
// 	{
// 	  crystal1 = crystal1 + OFFSET;
// 	  crystal2 = crystal2 + OFFSET;    
// 	  if (crystal1 >= N_DET)  crystal1 = crystal1 - N_DET;
// 	  if (crystal2 >= N_DET)  crystal2 = crystal2 - N_DET;
// 	}
      //--------------------------------
      //  Bin the crystal ring pairs into Michelograms
      //  u - radial sinogram component
      //  phi - azimuthal sinogram component
      //  ring pairs are sorted according to c1 < c2 else flip
      //  where c1 and c2 are crystals at phi(u = S_WIDTH/2)
      //--------------------------------
      phi = ((crystal1 + crystal2 + N_DET/2)%N_DET)/2;

      //  cout << "phi " << phi << " " <<  endl;
  
      if (((crystal1 + crystal2) < (3*N_DET/2)) && ((crystal1 + crystal2) >= (N_DET/2)))
	u    =  abs(crystal1 - crystal2) -  N_DET/2 + S_WIDTH/2;
      else u = -abs(crystal1 - crystal2) +  N_DET/2 + S_WIDTH/2;         

      if ( u >= S_WIDTH || u < 0 ) continue;

      if (u%2 == 0) 
	{
	  zi = (N_DET/2 - (crystal1 - crystal2) - 1)/2;
	  if (zi >=  N_DET/4) zi = zi - N_DET/2 + 1;
	  if (zi <= -N_DET/4) zi = zi + N_DET/2 - 1;
	}
      else          
	{
	  zi = (N_DET/2 - (crystal1 - crystal2))/2;
	  if (zi >=  N_DET/4) zi = zi - N_DET/2;
	  if (zi <= -N_DET/4) zi = zi + N_DET/2;
	}

      c1 = crystal1 + zi;
      c2 = crystal2 - zi;
      if (c1 >= N_DET) c1 = c1 - N_DET;
      if (c1 < 0)      c1 = c1 + N_DET;
      if (c2 >= N_DET) c2 = c2 - N_DET;
      if (c2 < 0)      c2 = c2 + N_DET;
      
      if (c1 < c2) flip = 0;
      else         flip = 1;

      if (flip) 
	{
	  swap  = ring1;
	  ring1 = ring2;
	  ring2 = swap;
	}
      
      // cout << "phi " << phi << " " << u << endl;

     // Update the different arrays...
     //-------------------------------
     //***ALL EVENTS
      Mich_r1r2fu[ring2][ring1][phi][u] += 1.;

     // if (eventID1 == eventID2)
//        {
// 	 //***true+scatter
// 	 if (comptonPhantom1 == 0 && comptonPhantom1 == 0)
// 	   {
// 	     //***true
// 	   }
// 	 else
// 	   {
// 	     //***scatter
// 	   }
//        }
//      else
//        {
// 	 //***random
//        }
      Counts++;
	                 
    }
  printf("\n");

  // Write the data to disk, and then close Michelogram file...
  //----------------------------------------------------------------
  ans = fwrite(Mich_r1r2fu,4,(N_RINGS*N_RINGS*N_DET/2*S_WIDTH),Mich_r1r2fuFile);
  fclose(Mich_r1r2fuFile);  
 
  // Generate projection files...
  //-----------------------------
  // From Segment number -MAX_D_RING to +MAX_D_RING
  // After phi
  // After z
  // After r
  int S_NUM;
  for (int i = 0 ; i < 2*MAX_D_RING + 1 ; i++)
    {
      if (i <= MAX_D_RING) S_NUM = N_RINGS - MAX_D_RING + i;
      else                 S_NUM = N_RINGS + MAX_D_RING - i;
  
      for (int j = 0 ; j < N_DET/2 ; j++)
	{ 
	  float Proj[S_NUM][S_WIDTH];
	  for (int k = 0 ; k < S_NUM ; k++)
	    {
	      if (i <= MAX_D_RING) ring1 = k;
	      else                 ring2 = k;

	      if (i <= MAX_D_RING) ring2 = ring1 + MAX_D_RING - i;
	      else                 ring1 = ring2 - MAX_D_RING + i;

	      for (int l = 0 ; l < S_WIDTH ; l++)
		Proj[k][l] = Mich_r1r2fu[ring2][ring1][j][l];
	    }
	  ans = fwrite(Proj,4,(S_NUM*S_WIDTH),Proj_File); 
	}
    }
  
    
 
  fclose(Proj_File);

  //-----------------------------

  return(0);

     
}
      
