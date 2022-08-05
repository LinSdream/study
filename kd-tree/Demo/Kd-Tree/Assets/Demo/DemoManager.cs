using Alg;
using Demo;
using Demo.Effect;
using System.Collections.Generic;
using System.Diagnostics;
using UnityEngine;
using Debug = UnityEngine.Debug;

public class DemoManager : MonoBehaviour
{
    public GameObject prefab;
    public GameObject playerModel;
    public float moveSpeed;
    public int count;
    public Vector2 minRange;

    private List<Transform> _prefabs;
    private Transform _prefabsParentTrans;
    private KdTree _tree;

    private Player _player;
    private EffectManager _effectMgr;

    public Material chainLightn;

    private void Awake()
    {
        _effectMgr = new EffectManager();

        _prefabsParentTrans = new GameObject("PrefabsParent").transform;
        _prefabs = new List<Transform>(count);
        for (int i = 0; i < count; i++)
        {
            Transform t = Instantiate<GameObject>(prefab).transform;
            t.SetParent(_prefabsParentTrans);
            t.name = i.ToString();
            _prefabs.Add(t);
            float x = Random.Range(minRange.x, minRange.y);
            float y = 1;
            //if (isRangeY) y = Random.Range(minRange.x, minRange.y);
            float z = Random.Range(minRange.x, minRange.y);
            t.position = new Vector3(x, y, z);
        }
        Stopwatch stopwatch = new Stopwatch();
        stopwatch.Start();
        _tree = new KdTree(_prefabs.ToArray());
        stopwatch.Stop();
        Debug.LogFormat("BuildTree Time:{0}", stopwatch.Elapsed.TotalMilliseconds);
        _player = new Player(playerModel);
        _player.moveSpeed = moveSpeed;
    }

    private void Update()
    {
        float deltaTime = Time.deltaTime;
        _player.Update(deltaTime);
        _effectMgr.Update(deltaTime);

        if (Input.GetMouseButtonDown(0))
        {
            Stopwatch stopwatch = new Stopwatch();
            stopwatch.Start();
            var res = _tree.Find(_player.Transform.position, 1, out var node);
            if (res != null && node[0].distance <= _player.attackRange * _player.attackRange)
            {
                _player.Attack(res[0].value);
                ChainLightningArgs args = new ChainLightningArgs
                {
                    startPoint = _player.Transform,
                    endPoint = res[0].value
                };
                BaseEffect effect;
                effect = _effectMgr.PlayEffect(args);
                if (effect == null)
                    _effectMgr.CreateEffect<ChainLightningArgs>(new ChainLightningEffect(chainLightn));
                effect = _effectMgr.PlayEffect(args);
                effect.playCompletedCallback += () =>
                {
                    _tree.Find(res[0].value.position, 4, out var nodes);
                    for (int i = 1; i < nodes.Count; i++)
                    {
                        args = new ChainLightningArgs
                        {
                            startPoint = res[0].value,
                            endPoint = nodes[i].node.value,
                        };

                        if (_effectMgr.PlayEffect(args) == null)
                        {
                            _effectMgr.CreateEffect<ChainLightningArgs>(new ChainLightningEffect(chainLightn));
                            var effect = _effectMgr.PlayEffect(args);
                        }

                    }
                };
            }

            stopwatch.Stop();

            Debug.LogFormat("Time: {0}", stopwatch.Elapsed.TotalMilliseconds);
        }
    }
}
