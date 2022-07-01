using System.Runtime.InteropServices;
using UnityEngine;

public class Minimum : MonoBehaviour
{
#if UNITY_IOS
        internal const string Lib = "__Internal";
#else
    internal const string Lib = "minimum";
#endif
    
    [DllImport(Lib)]
    public static extern void StartProfiling(int threadCount);
    [DllImport(Lib)]
    public static extern void StopProfiling();
    
    private void Start()
    {
        StartProfiling(5);
    }

    private void OnDestroy()
    {
        StopProfiling();
    }
}
