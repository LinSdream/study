using CommonModule.Logger;
using System.Diagnostics;
using System.Reflection;

public static class Program
{
    public static ILogger Logger = LogFactory.CreateLogger(LogFactory.ECreateLoggerType.CONSOLE);

    public static void Main()
    {
        Case1();
        Logger.Info("                        ");
        Case2();
        Logger.Info("                        ");
        Case3();
        Logger.Info("                        ");
        Case4();
    }

    private static void Case1()
    {
        Stopwatch sw = new Stopwatch();
        sw.Restart();
        Xvar a = Xvar_util.F0(1);
        Debug.Assert(a.GetValueVal<int>() == 1);
        sw.Stop();

        Logger.Info("Success Case 1 1 用时(ms): {0}", sw.Elapsed);

        sw.Restart();
        Xvar b = Xvar_util.F1<int, int>((x) => x * 100, a);
        Debug.Assert(b.GetValueVal<int>() == 100);
        sw.Stop();
        Logger.Info("Success Case 1 2 用时(ms): {0}", sw.Elapsed);

        sw.Restart();
        a.Reset<int>(2);
        Debug.Assert(b.GetValueVal<int>() == 200);
        sw.Stop();

        Logger.Info("Success Case 1 3 用时(ms): {0}", sw.Elapsed);

        sw.Restart();
        Debug.Assert(b.GetValueVal<int>() == 200);
        sw.Stop();

        Logger.Info("Success Case 1 4 用时(ms): {0}", sw.Elapsed);


        sw.Restart();
        int aaaa = 10 * 1;
        Logger.Info("{0}", aaaa);
        sw.Stop();

        Logger.Info("Success Case 1 5 用时(ms): {0}", sw.Elapsed);

        b = null;
        GC.Collect();
        Logger.Info("{0}", a);

    }

    private static void Case2()
    {
        Stopwatch sw = new Stopwatch();
        sw.Restart();
        Sxvar<int> a = Sxvar_util.F0(1);
        Debug.Assert(a.value == 1);
        sw.Stop();

        Logger.Info("Success Case 2 1 用时(ms): {0}", sw.Elapsed);

        sw.Restart();
        Sxvar<int> b = Sxvar_util.F1((x) => x * 100, a);
        Debug.Assert(b.value == 100);
        sw.Stop();
        Logger.Info("Success Case 2 2 用时(ms): {0}", sw.Elapsed);

        sw.Restart();
        a.Reset(2);
        Debug.Assert(b.value == 200);
        sw.Stop();

        Logger.Info("Success Case 2 3 用时(ms): {0}", sw.Elapsed);

        sw.Restart();
        Debug.Assert(b.value == 200);
        sw.Stop();

        Logger.Info("Success Case 2 4 用时(ms): {0}", sw.Elapsed);

        sw.Restart();
        Sxvar<int> c = Sxvar_util.F2((x, y) => x + y, b, 5);
        Debug.Assert(c.value == 205);
        sw.Stop();
        Logger.Info("Success Case 2 5 用时(ms): {0}", sw.Elapsed);
    }

    private static void Case3()
    {
        var a1 = Xint.F0(1);
        Debug.Assert(a1.Val == 1);

        a1.Val = 2;
        Debug.Assert(a1.Val == 2);

        var a2 = Xint.F1((a) => Xvar_util.Conver<int>(a) * 100, a1);
        Debug.Assert(a2.Val == 200);

        a1.Val = 3;
        Debug.Assert(a2.Val == 300);

        var a3 = Xint.F2((a, b) =>
        {
            return Xvar_util.Conver<int>(a) + Xvar_util.Conver<int>(b);
        }, a1, a2);

        Debug.Assert(a3.Val == 303);

        var b1 = Xbool.F2((a, b) =>
        {
            return Xvar_util.Conver<int>(a) > Xvar_util.Conver<int>(b);
        }, a1, a2);

        Debug.Assert(b1.Val == false);

        a1.Val = -1;
        Debug.Assert(b1.Val != false);
    }


    private static void Case4()
    {
        var a1 = Xint.F0(1);
        var a2 = Xint.F1((a) => Xvar_util.Conver<int>(a) * 100, a1);

        Type t = a1.GetType();
        FieldInfo fileInfo = t.BaseType.GetField("_xlinks", BindingFlags.NonPublic | BindingFlags.Instance);
        var set = fileInfo.GetValue(a1) as HashSet<IXvar>;
        Debug.Assert(set != null && (set.Count == 1));

        a2.Unused();
        a2 = null;

        GC.Collect();

        Debug.Assert(set != null && (set.Count == 0));

        Debug.Assert(Xvar_None.Instance.Val == Xvar_None.Instance);
    }
}