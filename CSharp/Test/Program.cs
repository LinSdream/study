using CommonModule.Logger;
public static class Program
{
    public static ILogger Logger = LogFactory.CreateLogger(LogFactory.ECreateLoggerType.CONSOLE);

    public class IdCountVal
    {
        public int id { get; set; }
        public int count { get; set; }
        public int val { get; set; }

        public IdCountVal()
        {

        }

        public IdCountVal(int id, int count, int val)
        {
            this.id = id;
            this.count = count;
            this.val = val;
        }
    }

    private static Dictionary<int, (int idx, int curVal, int nextVal)> _attriDiffTmp = new();
    private static List<(int id, int curVal, int nextVal)> _attriScrollTmpList = new();

    public static Task AAAAA()
    {
        return Task.CompletedTask;
    }

    public static void DiffAttri(IdCountVal[] curAttriArr, IdCountVal[] nextAttriArr,
        ref List<(int id, int curVal, int nextVal)> ret)
    {
        AAAAA();
        _attriDiffTmp.Clear();
        var index = 0;
        foreach (var attri in curAttriArr)
        {
            _attriDiffTmp[attri.id] = (index++, attri.count, 0);
        }

        foreach (var attri in nextAttriArr)
        {
            if (_attriDiffTmp.ContainsKey(attri.id))
            {
                _attriDiffTmp[attri.id] = (_attriDiffTmp[attri.id].idx, _attriDiffTmp[attri.id].curVal, attri.count);
            }
            else
            {
                _attriDiffTmp[attri.id] = (index++, 0, attri.count);
            }
        }

        ret.AddRange(
            _attriDiffTmp.Select(pair => (pair.Key, pair.Value.curVal, pair.Value.nextVal))
        );
    }

    public static int Main(params string[] args)
    {

        List<(int id, int curVal, int nextVal)> list = new List<(int id, int curVal, int nextVal)>();
        IdCountVal[] curAttriArr =
            new IdCountVal[] {
                new IdCountVal(1, 10, 0),
                new IdCountVal(2, 20, 0),
                new IdCountVal(3, 30, 0),
            };

        IdCountVal[] nextAttriArr =
            new IdCountVal[] {
                new IdCountVal(1, 15, 0),
                new IdCountVal(2, 20, 0),
                new IdCountVal(4, 40, 0),
            };

        DiffAttri(curAttriArr, nextAttriArr, ref list);

        foreach (var item in list)
        {
            Logger.Info(item.id + " " + item.curVal + " " + item.nextVal);
        }

        return 0;

    }
}