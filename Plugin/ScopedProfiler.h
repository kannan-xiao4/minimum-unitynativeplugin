#pragma once

#include <IUnityProfiler.h>

class ScopedProfiler
{
public:
    ScopedProfiler(IUnityProfiler* profiler, const UnityProfilerMarkerDesc& desc);
    ~ScopedProfiler();

private:
    void operator=(const ScopedProfiler& src) const { }
    ScopedProfiler(const ScopedProfiler& src) { }

    IUnityProfiler* profiler_;
    const UnityProfilerMarkerDesc* m_desc;
};

class ScopedProfilerThread
{
public:
    ScopedProfilerThread(IUnityProfiler* profiler, const char* groupName, const char* name);
    ~ScopedProfilerThread();

private:
    void operator=(const ScopedProfilerThread& src) const { }
    ScopedProfilerThread(const ScopedProfilerThread& src) { }

    IUnityProfiler* profiler_;
    UnityProfilerThreadId threadId_;
};
