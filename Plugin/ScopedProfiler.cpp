#include "ScopedProfiler.h"

ScopedProfiler::ScopedProfiler(IUnityProfiler* profiler, const UnityProfilerMarkerDesc& desc)
    : profiler_(profiler)
    , m_desc(&desc)
{
    if (profiler_->IsAvailable())
        profiler_->BeginSample(m_desc);
}

ScopedProfiler::~ScopedProfiler()
{
    if (profiler_->IsAvailable())
        profiler_->EndSample(m_desc);
}

ScopedProfilerThread::ScopedProfilerThread(IUnityProfiler* profiler, const char* groupName, const char* name)
    : profiler_(profiler)
{
    if (profiler_->IsAvailable())
    {
        int result = profiler_->RegisterThread(&threadId_, groupName, name);
        if (result)
        {
            throw;
        }
    }
}
ScopedProfilerThread ::~ScopedProfilerThread()
{
    if (profiler_->IsAvailable())
        profiler_->UnregisterThread(threadId_);
}
