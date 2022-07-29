using System.Collections.Generic;
using UnityEngine;

namespace Alg
{
    public struct ResultNode : IComparer<ResultNode>
    {
        public KdTreeNode node;
        public float distance;

        public int Compare(ResultNode x, ResultNode y)
        {
            if (x.distance == y.distance) return 0;
            return (x.distance < y.distance) ? -1 : 1;
        }
    }

    public class KdTree
    {
        private KdTreeNode _root;
        private int _size;
        private KdTreeUtli _util;


        public KdTree(Transform[] position)
        {
            _size = position.Length;
            _util = new KdTreeUtli();

            _root = new KdTreeNode();
            BuildTree(null, _root, 0, new List<Transform>(position));
        }

        public KdTreeNode[] Find(in Vector3 position, int count)
        {
            var leaf = _util.FindLeafNode(in position, _root);
            List<KdTreeNode> callOn = new List<KdTreeNode>(_size);
            List<ResultNode> results = new List<ResultNode>(count);
            Retrospective(in position, leaf, callOn, results);
            KdTreeNode[] result = new KdTreeNode[results.Count];
            for (int i = 0; i < result.Length; i++)
            {
                result[i] = results[i].node;
            }
            return result;
        }

        private void BuildTree(KdTreeNode parent, KdTreeNode root, short split, List<Transform> arr)
        {
            root.parent = parent;
            root.split = split;
            root.value = _util.Filtrate(split, arr, out var left, out var right);
            if (left == null && right == null)
            {
                root.IsLeaf = true;
                return;
            }
            if (left.Count != 0)
            {
                root.left = new KdTreeNode();
                BuildTree(root, root.left, (short)((split + 1) % 3), left);
            }
            if (right.Count != 0)
            {
                root.right = new KdTreeNode();
                BuildTree(root, root.right, (short)((split + 1) % 3), right);
            }
        }

        private void Retrospective(in Vector3 position, KdTreeNode cur, List<KdTreeNode> callOn, List<ResultNode> results)
        {
            if (cur == null) return;
            if (callOn.Exists(v => v == cur))
            {
                Retrospective(in position, cur.parent, callOn, results);
                return;
            }
            callOn.Add(cur);
            Campare(in position, cur, results);
            if (cur.IsLeaf)
            {
                Retrospective(in position, cur.parent, callOn, results);
                return;
            }

            float distance = _util.CalculateDistanceBySplit(in position, cur);

            if (results.Count == results.Capacity && distance > results[results.Count - 1].distance)
            {
                Retrospective(in position, cur.parent, callOn, results);
                return;
            }
            if (cur.right != null && callOn.Exists(v => v == cur.right))
            {
                KdTreeNode node = _util.FindLeafNode(in position, cur.right);
                Retrospective(in position, node, callOn, results);
            }
            else if (cur.left != null && callOn.Exists(v => v == cur.left))
            {
                KdTreeNode node = _util.FindLeafNode(in position, cur.left);
                Retrospective(in position, node, callOn, results);
            }
            else
            {
                Retrospective(in position, cur.parent, callOn, results);
            }
        }

        private bool Campare(in Vector3 position, KdTreeNode node, List<ResultNode> results)
        {
            if (results.Count < results.Capacity)
            {
                ResultNode n0 = new ResultNode();
                n0.node = node;
                n0.distance = _util.Distance(in position, node.value.position);
                results.Add(n0);
                results.Sort();
                return true;
            }
            float distance = _util.Distance(in position, node.value.position);
            if (results[results.Count - 1].distance < distance) return false;

            ResultNode n = new ResultNode
            {
                node = node,
                distance = distance
            };
            results[results.Count - 1] = n;
            results.Sort();
            return true;
        }
    }
}
