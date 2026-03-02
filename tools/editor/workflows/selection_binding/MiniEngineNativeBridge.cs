using System;
using System.Runtime.InteropServices;

public sealed class MiniEngineNativeBridge : IDisposable
{
    private IntPtr _engine;

    public MiniEngineNativeBridge()
    {
        _engine = rcae_mini_engine_create();
        if (_engine == IntPtr.Zero)
        {
            throw new InvalidOperationException("Failed to create native MiniEngine instance");
        }
    }

    public int CreateEntity() => rcae_mini_engine_create_entity(_engine);

    public bool SetTransform(int entityId, int x, int y)
        => rcae_mini_engine_set_transform(_engine, entityId, new RCAE_Vec2 { x = x, y = y }) != 0;

    public void SetVelocity(int entityId, int x, int y)
        => rcae_mini_engine_set_velocity(_engine, entityId, new RCAE_Vec2 { x = x, y = y });

    public void SetSprite(int entityId, char glyph)
        => rcae_mini_engine_set_sprite(_engine, entityId, glyph);

    public void Tick() => rcae_mini_engine_tick(_engine);

    public string Render()
    {
        var ptr = rcae_mini_engine_render(_engine);
        return Marshal.PtrToStringAnsi(ptr) ?? string.Empty;
    }

    public void Dispose()
    {
        if (_engine != IntPtr.Zero)
        {
            rcae_mini_engine_destroy(_engine);
            _engine = IntPtr.Zero;
        }
        GC.SuppressFinalize(this);
    }

    ~MiniEngineNativeBridge()
    {
        Dispose();
    }

    [StructLayout(LayoutKind.Sequential)]
    private struct RCAE_Vec2
    {
        public int x;
        public int y;
    }

    [DllImport("rcae_runtime_capi", CallingConvention = CallingConvention.Cdecl)]
    private static extern IntPtr rcae_mini_engine_create();

    [DllImport("rcae_runtime_capi", CallingConvention = CallingConvention.Cdecl)]
    private static extern void rcae_mini_engine_destroy(IntPtr engine);

    [DllImport("rcae_runtime_capi", CallingConvention = CallingConvention.Cdecl)]
    private static extern int rcae_mini_engine_create_entity(IntPtr engine);

    [DllImport("rcae_runtime_capi", CallingConvention = CallingConvention.Cdecl)]
    private static extern int rcae_mini_engine_set_transform(IntPtr engine, int entity_id, RCAE_Vec2 position);

    [DllImport("rcae_runtime_capi", CallingConvention = CallingConvention.Cdecl)]
    private static extern void rcae_mini_engine_set_velocity(IntPtr engine, int entity_id, RCAE_Vec2 velocity_per_tick);

    [DllImport("rcae_runtime_capi", CallingConvention = CallingConvention.Cdecl)]
    private static extern void rcae_mini_engine_set_sprite(IntPtr engine, int entity_id, char glyph);

    [DllImport("rcae_runtime_capi", CallingConvention = CallingConvention.Cdecl)]
    private static extern void rcae_mini_engine_tick(IntPtr engine);

    [DllImport("rcae_runtime_capi", CallingConvention = CallingConvention.Cdecl)]
    private static extern IntPtr rcae_mini_engine_render(IntPtr engine);
}
