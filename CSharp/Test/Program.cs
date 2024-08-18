using CommonModule.Logger;

public static class Program
{
    public static ILogger Logger = LogFactory.CreateLogger(LogFactory.ECreateLoggerType.CONSOLE);

    public class IdCountVal
    {
        public int id;
        public int count;
        public int value;
    }

    public static int Main(params string[] args)
    {
        List<IdCountVal> list = new List<IdCountVal>();
        list.Add(new IdCountVal()
        {
            id = 1,
            count = 100,
            value = 1000,
        });

        list.Add(new IdCountVal()
        {
            id = 2,
            count = 50,
            value = 100,
        });

        list.Add(new IdCountVal()
        {
            id = 3,
            count = 10,
            value = 10,
        });
        
        list.Add(new IdCountVal()
        {
            id = 4,
            count = 100,
            value = 3,
        });

        list.Add(new IdCountVal()
        {
            id = 5,
            count = 0,
            value = 1,
        });


        var (sum, res) = GetUseList(list, 2002);

        Logger.Info("Sum:{0}", sum);
        foreach (var pair in res)
        {
            Logger.Info("Id:{0}, count:{1}", pair.Key, pair.Value);
        }

        return 0;
    }





    public static (int sum, Dictionary<int, int> list) GetUseList(List<IdCountVal> list, int target)
    {
        var sum = 0;
        for (int i = 0, len = list.Count; i < len; i++)
        {
            var item = list[i];
            sum += item.value * item.count;
        }
        if (sum < target)
        {
            return (sum, new Dictionary<int, int>());
        }

        list.Sort((a, b) => a.value.CompareTo(b.value));

        int gbsNum = list[0].value;
        int gysNum = list[0].value;
        List<int> gbsList = new List<int>();
        List<int> gysList = new List<int>();
        for (int i = 0; i < list.Count; i++)
        {
            gbsNum = CalcMinCommonMultiple(list[i].value, gbsNum);
            gysNum = CalcGreatestCommonDivisor(list[i].value, gysNum);
            gbsList.Add(gbsNum);
            gysList.Add(gysNum);
        }

        (bool isValid, Dictionary<int, int> useDic, int subVal) = BeastChoiseList(target, list, gbsList, list.Count - 1, new Dictionary<int, int>());

        int total = 0;
        var useList = new List<IdCountVal>();

        foreach (var pair in useDic)
        {
            var item = list.Find((a) => a.id == pair.Key);
            total += pair.Value * item.value;
            useList.Add(new IdCountVal()
            {
                id = item.id,
                count = pair.Value,
                value = item.value,
            });
        }

        useList.Sort((a, b) => a.value.CompareTo(b.value));

        return (total, useDic);
    }


    public static int CalcGreatestCommonDivisor(int a, int b)
    {
        if (a < b)
        {
            (a, b) = (b, a);
        }
        return (a % b == 0) ? b : CalcGreatestCommonDivisor(a % b, b);
    }

    public static int CalcMinCommonMultiple(int a, int b)
    {
        return a * b / CalcGreatestCommonDivisor(a, b);
    }


    public static (bool isValid, Dictionary<int, int> useList, int subValue) BeastChoiseList(int target, List<IdCountVal> itemList, List<int> gbsList,  int index, Dictionary<int, int> useList)
    {
        if (index <= 0 || target <= 0)
            return (target <= 0, useList, Math.Abs(target));

        int gbs = gbsList[index];
        IdCountVal item = itemList[index];

        int maxCount = Math.Min(target / gbs, item.count * item.value / gbs);
        if (maxCount > 0)
        {
            useList[item.id] = maxCount * gbs / item.value;
            target -= maxCount * gbs;
        }
        else if (!useList.ContainsKey(item.id))
        {
            useList[item.id] = 0;
        }

        int minCount = -Math.Min(gbs / item.value, useList[item.id]);
        maxCount = Math.Min(item.count - useList[item.id], (int)Math.Ceiling((float)target / item.value));

        Dictionary<int, int> bestList = new Dictionary<int, int>();
        bestList[item.id] = maxCount;

        int bestSub = target - item.value * maxCount;

        for (int i = maxCount; i >= minCount; i--)
        {
            int subVal = target - item.value * i;
            if (subVal > 0)
            {
                (bool isValid, Dictionary<int, int> list, int subValue) = BeastChoiseList(subVal, itemList, gbsList, index - 1, new Dictionary<int, int>());
                if (isValid && (bestSub > 0 || subVal > bestSub))
                {
                    bestList = list;
                    bestList[item.id] = i;
                    bestSub = subValue;
                    if (bestSub >= 0)
                        break;
                }
            }
            else if (subVal == 0)
            {
                break;
            }
        }


        foreach (var pair in bestList)
        {
            if (useList.ContainsKey(pair.Key))
            {
                useList[pair.Key] += pair.Value;
            }
            else
            {
                useList[pair.Key] = pair.Value;
            }
        }

        return (bestSub <= 0, useList, bestSub);
    }
}