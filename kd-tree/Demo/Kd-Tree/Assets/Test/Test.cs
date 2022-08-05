using Alg;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using UnityEditor;
using UnityEngine;
using Debug = UnityEngine.Debug;
using Random = UnityEngine.Random;

namespace TestDemo
{
    public class Test : MonoBehaviour
    {
        private class TransInfo : IComparable<TransInfo>
        {
            public Transform node;
            public float distance;
            public int CompareTo(TransInfo other)
            {
                if (distance == other.distance) return 0;
                return distance < other.distance ? -1 : 1;
            }
        }

        public GameObject prefab;
        private List<Transform> _prefabs;
        private KdTree _tree;

        public int count;
        public Vector2 minRange;
        public bool isRangeY;
        public bool useKdFind = true;
        public PlayerController playerController;

        void Start()
        {
            _prefabs = new List<Transform>(count);
            for (int i = 0; i < count; i++)
            {
                Transform t = Instantiate(prefab).transform;
                t.name = i.ToString();
                _prefabs.Add(t);
                float x = Random.Range(minRange.x, minRange.y);
                float y = 1;
                if (isRangeY) y = Random.Range(minRange.x, minRange.y);
                float z = Random.Range(minRange.x, minRange.y);
                t.position = new Vector3(x, y, z);
            }
            _tree = new KdTree(_prefabs.ToArray());
        }

        public Transform[] Find(Vector3 position, int count)
        {
            if (useKdFind) return KdFind(position, count);
            return InternalFind(position, count);
        }

        private Transform[] InternalFind(Vector3 position, int count)
        {
            Stopwatch stopwatch = new Stopwatch();
            stopwatch.Start();

            List<TransInfo> rets = new List<TransInfo>(count);

            foreach (var tran in _prefabs)
            {
                float dis = (tran.position - position).sqrMagnitude;
                if (rets.Count < rets.Capacity)
                {
                    rets.Add(new TransInfo
                    {
                        distance = dis,
                        node = tran,
                    });
                    rets.Sort();
                }
                else
                {
                    rets[rets.Count - 1].distance = dis;
                    rets[rets.Count - 1].node = tran;
                    rets.Sort();
                }
            }
            stopwatch.Stop();
            Debug.Log($"Find£º{stopwatch.Elapsed.TotalMilliseconds}");
            return rets.ConvertAll(a => a.node).ToArray();
        }

        public Transform[] KdFind(Vector3 position, int count)
        {
            Stopwatch stopwatch = new Stopwatch();
            stopwatch.Start();
            var result = _tree.Find(position, count, out _);
            Transform[] rets = new Transform[count];
            for (int i = 0; i < count; i++)
            {
                rets[i] = result[i].value;
            }
            foreach (var trans in _prefabs)
            {
                if (!Array.Exists(rets, v => v == trans))
                {
                    var mat = trans.GetComponent<MeshRenderer>().material;
                    mat.color = Color.white;
                }
            }
            stopwatch.Stop();
            Debug.Log($"KdTree Find£º{stopwatch.Elapsed.TotalMilliseconds}");
            return rets;

        }

        private void OnDrawGizmos()
        {
            foreach (var trans in _prefabs)
            {
                Gizmos.DrawLine(playerController.transform.position, trans.position);
                float distance = Vector3.Distance(playerController.transform.position, trans.position);
                Handles.Label(trans.position, distance.ToString());
            }
        }
    }
}