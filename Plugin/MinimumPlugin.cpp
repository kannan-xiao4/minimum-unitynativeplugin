#include <array>
#include <chrono>
#include <memory>
#include <mutex>
#include <stddef.h>
#include <thread>
#include <vector>

#include <IUnityGraphics.h>
#include <IUnityInterface.h>
#include <IUnityProfiler.h>

#include "ScopedProfiler.h"

static IUnityInterfaces* s_UnityInterfaces = nullptr;
static IUnityGraphics* s_Graphics = nullptr;
static UnityGfxRenderer s_RendererType = kUnityGfxRendererNull;
static IUnityProfiler* s_UnityProfiler = nullptr;

static void UNITY_INTERFACE_API OnGraphicsDeviceEvent(UnityGfxDeviceEventType eventType)
{
    switch (eventType)
    {
    case kUnityGfxDeviceEventInitialize:
    {
        s_RendererType = s_Graphics->GetRenderer();
        // TODO: Initialization
        break;
    }
    case kUnityGfxDeviceEventShutdown:
    {
        s_RendererType = kUnityGfxRendererNull;
        // TODO: Shutdown
        break;
    }
    case kUnityGfxDeviceEventBeforeReset:
    case kUnityGfxDeviceEventAfterReset:
    {
        break;
    }
    };
}

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityPluginLoad(IUnityInterfaces* unityInterfaces)
{
    s_UnityInterfaces = unityInterfaces;
    s_UnityProfiler = unityInterfaces->Get<IUnityProfiler>();

    s_Graphics = unityInterfaces->Get<IUnityGraphics>();
    if (s_Graphics)
    {
        s_Graphics->RegisterDeviceEventCallback(OnGraphicsDeviceEvent);
        OnGraphicsDeviceEvent(kUnityGfxDeviceEventInitialize);
    }
}

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityPluginUnload()
{
    s_Graphics->UnregisterDeviceEventCallback(OnGraphicsDeviceEvent);
}

constexpr char threadGroupName[] = "MinimumProfile";
static std::vector<std::unique_ptr<std::thread>> s_threadList;
bool s_runnning = false;

void ThreadFunction(const char* label, const char* makerLabel)
{
    auto profilerThread = std::make_unique<ScopedProfilerThread>(s_UnityProfiler, threadGroupName, label);
    const UnityProfilerMarkerDesc* profileMark = nullptr;
    int result = s_UnityProfiler->CreateMarker(
        &profileMark, makerLabel, kUnityProfilerCategoryScripts, kUnityProfilerMarkerFlagDefault, 0);
    while (s_runnning)
    {
        std::unique_ptr<const ScopedProfiler> scopedProfiler;
        if (s_UnityProfiler)
        {
            scopedProfiler = std::make_unique<const ScopedProfiler>(s_UnityProfiler, *profileMark);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void StartProfilingThread()
{
    s_runnning = true;
    auto th1 = std::make_unique<std::thread>(ThreadFunction, "Thread1", "Profile1");
    s_threadList.emplace_back(std::move(th1));

    auto th2 = std::make_unique<std::thread>(ThreadFunction, "Thread2", "Profile2");
    s_threadList.emplace_back(std::move(th2));
}

void StopProfilingThread()
{
    s_runnning = false;
    for (auto& thread : s_threadList)
    {
        thread->join();
    }

    s_threadList.clear();
}

extern "C"
{
    UNITY_INTERFACE_EXPORT void StartProfiling() { StartProfilingThread(); }
    UNITY_INTERFACE_EXPORT void StopProfiling() { StopProfilingThread(); }
}