/**
 * @file ThreadEvent.h
 * @author created by: Peter Hlavaty
 */

#ifndef __THREADEVENT_H__
#define __THREADEVENT_H__

#include "../../Common/base/Common.h"
#include "../../Common/utils/ProcessCtx.h"
#include "../../Common/utils/SyscallCallbacks.hpp"

struct EVENT_THREAD_INFO 
{
	HANDLE ProcessId;
	void* EventSemaphor;
	ULONG_PTR GeneralPurposeContext[REG_COUNT];

	EVENT_THREAD_INFO(
		__in HANDLE processId
		) : ProcessId(processId)
	{
	}

	void SetContext(
		__in ULONG_PTR reg[REG_COUNT]
		)
	{
		*GeneralPurposeContext = *reg;
		EventSemaphor = reinterpret_cast<void*>(reg[EDX]);//enum RDX != EDX
	}
};

class CThreadEvent :
	public THREAD_INFO
{
public:
	MEMORY_INFO LastMemoryInfo;
	ULONG_PTR GeneralPurposeContext[REG_COUNT];

	CThreadEvent();

	CThreadEvent(
		__in HANDLE threadId,
		__in HANDLE parentProcessId = NULL
		);

// VIRTUAL MEMORY HANDLER support routines
	__checkReturn
	bool WaitForSyscallEpilogue();

	void SetCallbackEpilogue(
		__in ULONG_PTR reg[REG_COUNT],
		__in void* memory,
		__in size_t size,
		__in bool write,
		__in_opt void* pageFault = NULL
		);

	void EpilogueProceeded();

// FUZZ MONITOR HANDLER support routines
	__checkReturn
	bool MonitorFastCall(
		__in ULONG_PTR reg[REG_COUNT]
		);

	__checkReturn
	bool EventCallback(
		__in ULONG_PTR reg[REG_COUNT]
	);

protected:
	__checkReturn 
	bool FlipSemaphore(
		__in const EVENT_THREAD_INFO& eventThreadInfo
		);

protected:
	bool WaitForSyscallCallback;

	EVENT_THREAD_INFO m_monitorThreadInfo;
	EVENT_THREAD_INFO m_currentThreadInfo;
};

#endif //__THREADEVENT_H__
