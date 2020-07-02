// Particle Source for ParallelPhotonBeamSource
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

#include "ParallelPhotonBeamSource.hh"

#include "TsParameterManager.hh"

ParallelPhotonBeamSource::ParallelPhotonBeamSource(TsParameterManager* pM, TsSourceManager* psM, G4String sourceName) :
TsSource(pM, psM, sourceName)
{
	ResolveParameters();
}


ParallelPhotonBeamSource::~ParallelPhotonBeamSource()
{
}


void ParallelPhotonBeamSource::ResolveParameters() {
	TsSource::ResolveParameters();
}
