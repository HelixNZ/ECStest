#pragma once
#ifndef __PROFILER_H__
#define __PROFILER_H__

//Library Includes
#include <vector>
#include <string>

//Macros
#define ProfileCurrentScope(strRef) CProfilerScoped __tempProfiler_inscope(strRef, __FILE__, __LINE__);

//Types
struct TProfilerProfile
{
	//Member Variables
	std::string strReference;
	std::string strStartFile;
	std::string strEndFile;
	int iStartLine;
	int iEndLine;
	float fProfileDuration;

	//Counter data
	__int64 uiProfileStart;
	__int64 uiProfileEnd;
};

//Prototypes
class CProfiler
{
	//Member Functions
public:
	static CProfiler& GetInstance();
	static void DestroyInstance();

	int StartProfile(std::string _strReference, std::string _strFile, int _iLine);
	void EndProfile(int _iProfileID, std::string _strEndFile, int _iEndLine);

	TProfilerProfile GetProfileData(int _iProfileID) const;

private:
	CProfiler();
	~CProfiler();

	//Member Variables
protected:
	static CProfiler* s_pProfiler;

	std::vector<TProfilerProfile> m_vecProfiles;
	double m_dPCFrequency;

};

//Do not new, will only run in scope (AUTOMATED)
class CProfilerScoped
{
	//Member Functions
public:
	CProfilerScoped(std::string _strReference, std::string _strFile, int _iLine)
	{
		m_iLine = _iLine;
		m_iProfileID = CProfiler::GetInstance().StartProfile(_strReference, _strFile, _iLine);
	}

	~CProfilerScoped()
	{
		CProfiler::GetInstance().EndProfile(m_iProfileID, "", m_iLine);
	}

	//Member Variables
protected:
	int m_iProfileID;
	int m_iLine;

};

#endif //__PROFILER_H__