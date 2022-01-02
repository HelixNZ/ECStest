//Local Includes
#include <windows.h>

//This Include
#include "profiler.h"

//Static Variables
CProfiler* CProfiler::s_pProfiler = NULL;

//Implementation
CProfiler::CProfiler()
{
	//Constructor
	LARGE_INTEGER liFrequency;
	QueryPerformanceFrequency(&liFrequency);
	m_dPCFrequency = double(liFrequency.QuadPart) / 1000.0;
}

CProfiler::~CProfiler()
{
	//Destructor
}

CProfiler&
CProfiler::GetInstance()
{
	if (!s_pProfiler) s_pProfiler = new CProfiler();
	return(*s_pProfiler);
}

void
CProfiler::DestroyInstance()
{
	if (s_pProfiler) delete s_pProfiler;
	s_pProfiler = nullptr;
}

int
CProfiler::StartProfile(std::string _strReference, std::string _strFile, int _iLine)
{
	//Push back an empty profile first (Consumes a lot of time)
	m_vecProfiles.push_back(TProfilerProfile());

	//Store the ID and profile pointer to reduce time
	int iProfileID = (int)m_vecProfiles.size() - 1;
	TProfilerProfile* tNewProfile = &m_vecProfiles[iProfileID];

	//Store data
	tNewProfile->strReference = _strReference;
	tNewProfile->strStartFile = _strFile;
	tNewProfile->iStartLine = _iLine;

	//Store start time
	LARGE_INTEGER liFrequency;
	QueryPerformanceCounter(&liFrequency);
	tNewProfile->uiProfileStart = liFrequency.QuadPart;

	return(iProfileID);
}

void
CProfiler::EndProfile(int _iProfileID, std::string _strEndFile, int _iEndLine)
{
	//Store time immediately for most accurate results
	LARGE_INTEGER liFrequency;
	QueryPerformanceCounter(&liFrequency);

	//Grab a reference to the profile
	TProfilerProfile& tProfile = m_vecProfiles[_iProfileID];
	tProfile.uiProfileEnd = liFrequency.QuadPart;
	tProfile.fProfileDuration = (float)(double(tProfile.uiProfileEnd - tProfile.uiProfileStart) / m_dPCFrequency); //Retrieve time taken

	//Record final information
	tProfile.strEndFile = _strEndFile;
	tProfile.iEndLine = _iEndLine;

	//Debug (Consumes 0.8ms)
	OutputDebugStringA(("PROFILER: " + tProfile.strReference + " consumed " + std::to_string(tProfile.fProfileDuration) + "ms\n").c_str());
}

TProfilerProfile
CProfiler::GetProfileData(int _iProfileID) const
{
	return(m_vecProfiles[_iProfileID]);
}