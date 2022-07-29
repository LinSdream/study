using Alg;
using System;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;
using Random = UnityEngine.Random;

public class Test : MonoBehaviour
{

    public GameObject prefab;
    private List<Transform> _prefabs;
    private KdTree _tree;

    public int count;
    public Vector2 minRange;
    public bool isRangeY;

    public PlayerController playerController;

    void Start()
    {
        _prefabs = new List<Transform>(count);
        for (int i = 0; i < count; i++)
        {
            Transform t = Instantiate<GameObject>(prefab).transform;
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
        var result = _tree.Find(position, count);
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
        return rets;
    }

    private void OnDrawGizmos()
    {
        foreach(var trans in _prefabs)
        {
            Gizmos.DrawLine(playerController.transform.position, trans.position);
            float distance = Vector3.Distance(playerController.transform.position, trans.position);
            Handles.Label(trans.position, distance.ToString());
        }
    }
}
