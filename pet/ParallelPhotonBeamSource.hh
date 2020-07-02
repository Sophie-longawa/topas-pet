//
// ********************************************************************
// *                                                                  *
// *                                                                  *
// * This file was obtained from Topas MC Inc under the license       *
// * agreement set forth at http://www.topasmc.org/registration       *
// * Any use of this file constitutes full acceptance of              *
// * this TOPAS MC license agreement.                                 *
// *                                                                  *
// ********************************************************************
//

#ifndef ParallelPhotonBeamSource_hh
#define ParallelPhotonBeamSource_hh

#include "TsSource.hh"

class ParallelPhotonBeamSource : public TsSource
{
public:
	ParallelPhotonBeamSource(TsParameterManager* pM, TsSourceManager* psM, G4String sourceName);
	~ParallelPhotonBeamSource();

	void ResolveParameters();
};
#endif
