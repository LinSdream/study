

using System.Diagnostics;
using System.Runtime.CompilerServices;

public class Sxvar<T> where T : struct
{
    internal ExvarOpType _xop;
    internal T _xval;
    internal bool _xIsDirty;
    internal Sxvar<T> _xSource_x;
    internal T _xSource_c;
    internal ConditionalWeakTable<Sxvar<T>, WeakReference> _xLink;

    internal Func<T, T> _xf1;
    internal Func<T, T, T> _xf2;

    public T value
    {
        get
        {
            if (_xIsDirty)
            {
                var sw = new Stopwatch();
                sw.Restart();
                Sxvar_util.Validate(this);
                sw.Stop();
                Program.Logger.Info("Validate  用时: {0} ", sw.Elapsed);
            }
            return _xval;
        }
    }

    public void Reset(T val)
    {
        Debug.Assert(_xop == ExvarOpType.XOP0);
        Sxvar_util.Reset(this, val);
    }
}

public static class Sxvar_util
{
    public static Sxvar<T> F0<T>(T val) where T : struct
    {
        Sxvar<T> x = new Sxvar<T>();
        x._xop = ExvarOpType.XOP0;
        x._xIsDirty = true;
        x._xval = val;
        return x;
    }

    public static Sxvar<T> F1<T>(Func<T, T> func, Sxvar<T> x) where T : struct
    {
        Sxvar<T> y = new Sxvar<T>();
        y._xop = ExvarOpType.XOP1;
        AddLink(x, y);
        y._xf1 = func;
        y._xSource_x = x;
        y._xIsDirty = true;
        return y;
    }
    public static Sxvar<T> F2<T>(Func<T, T, T> func, Sxvar<T> x, T y) where T : struct
    {
        Sxvar<T> z = new Sxvar<T>();
        z._xop = ExvarOpType.XOP2_XC;
        AddLink(x, z);
        z._xf2 = func;
        z._xSource_x = x;
        z._xSource_c = y;
        z._xIsDirty = true;
        return z;
    }

    public static void Validate<T>(Sxvar<T> x) where T : struct
    {

        if (x._xop == ExvarOpType.XOP0)
        {
            x._xIsDirty = false;
            return;
        }
        T oldVal = x._xval;
        T newVal = default;

        switch (x._xop)
        {
            case ExvarOpType.XOP1:
                var source = x._xSource_x;
                newVal = x._xf1(source.value);
                break;
            case ExvarOpType.XOP2_XC:
                var source_x= x._xSource_x;
                var source_c = x._xSource_c;
                newVal = x._xf2(source_x.value, source_c);
                break;
                //case ExvarOpType.XOP2_XC:
                //    args2[0] = GetArg(ref x._xSource_x[0]);
                //    args2[1] = GetArg(ref x._xSource_x[1]);
                //    newVal = CallXf(ref x._xf, args2);
                //    break;
        }
        x._xval = newVal;
        x._xIsDirty = false;
    }

    private static void SetDirty<T>(Sxvar<T> x) where T : struct
    {
        if (x._xIsDirty)
        {
            return;
        }
        x._xIsDirty = true;
        if (x._xLink != null)
        {
            var sw = new Stopwatch();
            sw.Restart();
            foreach (var dep in x._xLink)
            {
                SetDirty(dep.Key);
            }
            sw.Stop();
            Program.Logger.Info("SetDirty 用时: {0} ", sw.Elapsed);
        }
    }

    private static void AddLink<T>(Sxvar<T> x, Sxvar<T> link) where T : struct
    {
        if (x._xLink == null)
        {
            x._xLink = new ConditionalWeakTable<Sxvar<T>, WeakReference>();
        }
        x._xLink.Add(link, new WeakReference(x));
    }

    internal static void Reset<T>(Sxvar<T> x, T val) where T : struct
    {
        x._xval = val;
        SetDirty(x);
    }
}