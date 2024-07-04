
using System.Runtime.CompilerServices;

public class Xvar_None : IXvar
{
    private static Xvar_None _instance = new Xvar_None();
    public static Xvar_None Instance => _instance;

    public void AddLink(IXvar link)
    {
        throw new InvalidOperationException();
    }

    public void RemoveLink(IXvar link)
    {
        throw new InvalidOperationException();
    }

    public void SetDirty()
    {
        throw new InvalidOperationException();
    }

    public void Unused()
    {
        throw new InvalidOperationException();
    }

    public Xvar_None Val
    {
        get
        {
            return this;
        }
        set
        {
            throw new InvalidOperationException();
        }
    }

    public bool Invaild => true;
}

public interface IXvar
{
    void SetDirty();
    void AddLink(IXvar link);
    void RemoveLink(IXvar link);
    void Unused();
    bool Invaild { get; }
}

public enum ExvarOpType : short
{
    XOP0 = 0,
    XOP1,
    XOP2_XC,
    XOP2_XX,
}

public enum ExargType : short
{
    None = 0,
    Xvar,
    Struct,
    Class,
    Primitive,
}

internal unsafe struct XArg
{
    public ExargType eArgType;
    public Type type;
    public object value;
    public void* pointer;
}

internal unsafe struct XFunc
{
    public Delegate func;
    public Type resType;
}

public class Xvar : IDisposable
{
    internal ExvarOpType _xop;
    internal XArg _xval;
    internal XArg[] _xSource;
    internal XFunc _xf;
    internal bool _xIsDirty;
    internal ConditionalWeakTable<Xvar, WeakReference> _xLink;

    public T GetCalssVal<T>() where T : new()
    {
        if (_xIsDirty)
        {
            Xvar_util.Validate(this);
        }
        return (T)_xval.value;
    }

    public T GetValueVal<T>() where T : struct
    {
        if (_xIsDirty)
        {
            Xvar_util.Validate(this);
        }
        return Xvar_util.ConvertArg<T>(ref _xval);
    }

    internal object GetValRaw()
    {
        if (_xIsDirty)
        {
            Xvar_util.Validate(this);
        }
        return _xval.value;
    }

    public void Reset<T>(T val)
    {
        Xvar_util.Reset(this, val);
    }

    public void Dispose()
    {
        _xval = default;
        _xSource = null;
        _xf = default;
        _xLink = null;
    }

    public void SetDirty()
    {

    }
}


public static class Xvar_util
{
    public static T Conver<T>(IXvar x)
    {
        if (x.Invaild)
        {
            return default;
        }
        var res = x as Xvar<T>;
        if (res != null)
        {
            return res.Val;
        }
        return default;
    }

    private static object[] args1 = new object[1];
    private static object[] args2 = new object[2];

    public static Xvar F0<T>(T val)
    {
        Xvar x = new Xvar();
        x._xop = ExvarOpType.XOP0;
        x._xIsDirty = true;
        ParseArg(ref x._xval, val);
        return x;
    }

    public static Xvar F1<T, K>(Func<T, K> func, Xvar x)
    {
        Xvar y = new Xvar();
        y._xop = ExvarOpType.XOP1;
        AddLink(x, y);
        y._xSource = new XArg[1];
        ParseArg(ref y._xSource[0], x);
        y._xIsDirty = true;
        ParseFunc(ref y._xf, func);
        return y;
    }

    public static void Validate(Xvar x)
    {

        if (x._xop == ExvarOpType.XOP0)
        {
            x._xIsDirty = false;
            return;
        }
        object oldVal = x._xval;
        object newVal = null;

        switch (x._xop)
        {
            case ExvarOpType.XOP1:
                var source = x._xSource[0];
                args1[0] = GetArg(ref source);
                newVal = CallXf(ref x._xf, args1);
                break;
            case ExvarOpType.XOP2_XC:
                args2[0] = GetArg(ref x._xSource[0]);
                args2[1] = GetArg(ref x._xSource[1]);
                newVal = CallXf(ref x._xf, args2);
                break;
        }
        ParseArg(ref x._xval, newVal, x._xf.resType);
        x._xIsDirty = false;
    }


    private static object CallXf(ref XFunc xf, object[] args)
    {
        try
        {
            var method = xf.func.Method;
            return method.Invoke(xf.func.Target, args1);
        }
        catch (Exception ex)
        {
            Program.Logger.Fatal(ex);
        }
        return null;
    }

    private static object GetArg(ref XArg arg)
    {
        switch (arg.eArgType)
        {
            case ExargType.Xvar:
                return (arg.value as Xvar).GetValRaw();
            default:
                return arg.value;
        }
    }

    internal unsafe static T ConvertArg<T>(ref XArg arg) where T : struct
    {
        switch (arg.eArgType)
        {
            case ExargType.Primitive:
            case ExargType.Struct:
                var res = Unsafe.Unbox<T>(arg.value);
                return res;
            default:
                throw new ArgumentException("类型错误");
        }
    }

    internal static void Reset<T>(Xvar x, T val)
    {
        ParseArg(ref x._xval, val);
        SetDirty(x);
    }


    private static void SetDirty(Xvar x)
    {
        if (x._xIsDirty)
        {
            return;
        }
        x._xIsDirty = true;
        if (x._xLink != null)
        {
            foreach (var dep in x._xLink)
            {
                SetDirty(dep.Key);
            }
        }
    }

    private unsafe static void ParseArg<T>(ref XArg arg, T val, Type t = null)
    {
        arg.value = val;
        arg.type = t == null ? typeof(T) : t;
        arg.pointer = Unsafe.AsPointer<object>(ref arg.value);
        if (arg.type == typeof(Xvar))
        {
            arg.eArgType = ExargType.Xvar;
        }
        else if (arg.type.IsClass)
        {
            arg.eArgType = ExargType.Class;
        }
        else if (arg.type.IsValueType && !arg.type.IsPrimitive && !arg.type.IsEnum)
        {
            arg.eArgType = ExargType.Struct;
        }
        else if (arg.type.IsPrimitive)
        {
            arg.eArgType = ExargType.Primitive;
        }
    }

    private static void ParseFunc<T, K>(ref XFunc refFunc, Func<T, K> func)
    {
        refFunc.func = func;
        refFunc.resType = typeof(K);
    }

    private static void AddLink(Xvar x, Xvar link)
    {
        if (x._xLink == null)
        {
            x._xLink = new ConditionalWeakTable<Xvar, WeakReference>();
        }
        x._xLink.Add(link, new WeakReference(x));
    }

}


public class Xvar<T> : IXvar
{
    protected ExvarOpType _xop;
    protected T _xval;
    protected IXvar _xSource_1;
    protected IXvar _xSource_2;
    protected Func<IXvar, T> _xf1;
    protected Func<IXvar, IXvar, T> _xf2;
    protected bool _xIsDirty;
    protected bool _invaild = false;
    protected HashSet<IXvar> _xlinks;
    protected Xvar() { }

    public T Val
    {
        get
        {
            if (_xIsDirty)
            {
                Validate();
            }
            return _xval;
        }
        set
        {
            if (_xop != ExvarOpType.XOP0)
            {
                throw new InvalidOperationException();
            }

            _xval = value;
            SetDirty();
        }
    }

    public bool Invaild
    {
        get { return _invaild; }
        protected set
        {
            if (_xop == ExvarOpType.XOP0)
            {
                _invaild = false;
            }
            else
            {
                _invaild = value;
            }
        }
    }

    protected void Validate()
    {
        if (_xop == ExvarOpType.XOP0)
        {
            _xIsDirty = false;
            return;
        }

        T oldVal = _xval;
        T newVal = default;

        switch (_xop)
        {
            case ExvarOpType.XOP1:
                var xf = _xf1;
                var source = _xSource_1;
                if (source.Invaild)
                {
                    Invaild = true;
                    _xIsDirty = false;
                    return;
                }

                newVal = xf(source);
                break;
            case ExvarOpType.XOP2_XX:
                var xf2 = _xf2;
                var source1 = _xSource_1;
                var source2 = _xSource_2;
                if (source1.Invaild || source2.Invaild)
                {
                    Invaild = true;
                    _xIsDirty = false;
                    return;
                }
                newVal = xf2(source1, source2);
                break;
        }

        _invaild = false;
        _xval = newVal;
        _xIsDirty = false;
    }

    public void SetDirty()
    {
        if (_xIsDirty) return;
        _xIsDirty = true;

        if (_xlinks != null)
        {
            foreach (var link in _xlinks)
            {
                link.SetDirty();
            }
        }
    }

    public void AddLink(IXvar link)
    {
        if (_xlinks == null)
        {
            _xlinks = new HashSet<IXvar>();
        }
        _xlinks.Add(link);
    }

    public void RemoveLink(IXvar link)
    {
        if (_xlinks != null && _xlinks.Contains(link))
        {
            _xlinks.Remove(link);
        }
    }

    public static Xvar<T> F0(T val)
    {
        var xvar = new Xvar<T>();
        xvar._xop = ExvarOpType.XOP0;
        xvar._xval = val;
        xvar._xIsDirty = true;
        return xvar;
    }

    public static Xvar<T> F1(Func<IXvar, T> func, IXvar x)
    {
        var y = new Xvar<T>();
        y._xop = ExvarOpType.XOP1;
        y._xIsDirty = true;
        y._xf1 = func;
        y._xSource_1 = x;
        x.AddLink(y);
        return y;
    }

    public static Xvar<T> F2(Func<IXvar, IXvar, T> func, IXvar x, IXvar y)
    {
        var z = new Xvar<T>();
        z._xop = ExvarOpType.XOP2_XX;
        z._xIsDirty = true;
        z._xf2 = func;
        z._xSource_1 = x;
        z._xSource_2 = y;
        x.AddLink(z);
        y.AddLink(z);
        return z;
    }

    public void Unused()
    {
        if (_xSource_1 != null)
        {
            _xSource_1.RemoveLink(this);
        }
        if (_xSource_2 != null)
        {
            _xSource_2.RemoveLink(this);
        }
    }

    internal void SetInvaild()
    {
        Invaild = true;
    }
}


public class Xint : Xvar<int>
{
    public new static Xint F0(int val)
    {
        var xvar = new Xint();
        xvar._xop = ExvarOpType.XOP0;
        xvar._xval = val;
        xvar._xIsDirty = true;
        return xvar;
    }

    public new static Xint F1(Func<IXvar, int> func, IXvar x)
    {
        var y = new Xint();
        y._xop = ExvarOpType.XOP1;
        y._xIsDirty = true;
        y._xf1 = func;
        y._xSource_1 = x;
        x.AddLink(y);
        return y;
    }

    public new static Xint F2(Func<IXvar, IXvar, int> func, IXvar x, IXvar y)
    {
        var z = new Xint();
        z._xop = ExvarOpType.XOP2_XX;
        z._xIsDirty = true;
        z._xf2 = func;
        z._xSource_1 = x;
        z._xSource_2 = y;
        x.AddLink(z);
        y.AddLink(z);
        return z;
    }

}

public class Xbool : Xvar<bool>
{
    public new static Xbool F0(bool val)
    {
        var xvar = new Xbool();
        xvar._xop = ExvarOpType.XOP0;
        xvar._xval = val;
        xvar._xIsDirty = true;
        return xvar;
    }

    public new static Xbool F1(Func<IXvar, bool> func, IXvar x)
    {
        var y = new Xbool();
        y._xop = ExvarOpType.XOP1;
        y._xIsDirty = true;
        y._xf1 = func;
        y._xSource_1 = x;
        x.AddLink(y);
        return y;
    }

    public new static Xbool F2(Func<IXvar, IXvar, bool> func, IXvar x, IXvar y)
    {
        var z = new Xbool();
        z._xop = ExvarOpType.XOP2_XX;
        z._xIsDirty = true;
        z._xf2 = func;
        z._xSource_1 = x;
        z._xSource_2 = y;
        x.AddLink(z);
        y.AddLink(z);
        return z;
    }

}