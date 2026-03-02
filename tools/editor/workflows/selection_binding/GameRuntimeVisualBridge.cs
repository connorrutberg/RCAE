using System;
using System.Runtime.InteropServices;

public sealed class GameRuntimeVisualBridge : IDisposable
{
    private IntPtr _runtime;

    public GameRuntimeVisualBridge()
    {
        _runtime = rcae_game_runtime_create();
        if (_runtime == IntPtr.Zero)
        {
            throw new InvalidOperationException("Failed to create native game runtime");
        }
    }

    public bool LoadLevel(string path) => rcae_game_runtime_load_level(_runtime, path) == 1;

    public void Move(int vx, int vy) => rcae_game_runtime_move(_runtime, vx, vy);
    public void Tick() => rcae_game_runtime_tick(_runtime);
    public void RunTicks(int ticks) => rcae_game_runtime_run_ticks(_runtime, ticks);
    public void ClickButton(string buttonId) => rcae_game_runtime_click_button(_runtime, buttonId);

    public string Frame()
    {
        var p = rcae_game_runtime_frame(_runtime);
        return Marshal.PtrToStringAnsi(p) ?? string.Empty;
    }

    public string DrainEvents()
    {
        var p = rcae_game_runtime_drain_events(_runtime);
        return Marshal.PtrToStringAnsi(p) ?? string.Empty;
    }

    public void Dispose()
    {
        if (_runtime != IntPtr.Zero)
        {
            rcae_game_runtime_destroy(_runtime);
            _runtime = IntPtr.Zero;
        }
        GC.SuppressFinalize(this);
    }

    ~GameRuntimeVisualBridge()
    {
        Dispose();
    }

    [DllImport("rcae_game_runtime_capi", CallingConvention = CallingConvention.Cdecl)]
    private static extern IntPtr rcae_game_runtime_create();

    [DllImport("rcae_game_runtime_capi", CallingConvention = CallingConvention.Cdecl)]
    private static extern void rcae_game_runtime_destroy(IntPtr runtime);

    [DllImport("rcae_game_runtime_capi", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern int rcae_game_runtime_load_level(IntPtr runtime, string path);

    [DllImport("rcae_game_runtime_capi", CallingConvention = CallingConvention.Cdecl)]
    private static extern void rcae_game_runtime_move(IntPtr runtime, int vx, int vy);

    [DllImport("rcae_game_runtime_capi", CallingConvention = CallingConvention.Cdecl)]
    private static extern void rcae_game_runtime_tick(IntPtr runtime);

    [DllImport("rcae_game_runtime_capi", CallingConvention = CallingConvention.Cdecl)]
    private static extern void rcae_game_runtime_run_ticks(IntPtr runtime, int ticks);

    [DllImport("rcae_game_runtime_capi", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    private static extern void rcae_game_runtime_click_button(IntPtr runtime, string buttonId);

    [DllImport("rcae_game_runtime_capi", CallingConvention = CallingConvention.Cdecl)]
    private static extern IntPtr rcae_game_runtime_frame(IntPtr runtime);

    [DllImport("rcae_game_runtime_capi", CallingConvention = CallingConvention.Cdecl)]
    private static extern IntPtr rcae_game_runtime_drain_events(IntPtr runtime);
}
