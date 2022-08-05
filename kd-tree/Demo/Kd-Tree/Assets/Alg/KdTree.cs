using System;
using System.Collections.Generic;
using UnityEngine;

namespace Alg
{
    public struct ResultNode : IComparable<ResultNode>
    {
        public KdTreeNode node;
        public float distance;

        public int CompareTo(ResultNode other)
        {
            if (distance == other.distance) return 0;
            return (distance < other.distance) ? -1 : 1;
        }
    }

    public class KdTree
    {
        private KdTreeNode _root;
        private int _size;
        private KdTreeUtli _util;
        private bool _includeY;


        public KdTree(Transform[] position, bool includeY = false)
        {
            _includeY = includeY;
            _size = position.Length;
            _util = new KdTreeUtli(_includeY);

            _root = new KdTreeNode();
            BuildTree(null, _root, 0, new List<Transform>(position));
        }

        private void ResetVisit(KdTreeNode root)
        {
            if (root != null)
            {
                root.visit = false;
                ResetVisit(root.left);
                ResetVisit(root.right);
            }
        }

        public KdTreeNode[] Find(in Vector3 position, int count, out List<ResultNode> results)
        {
            var leaf = _util.FindLeafNode(in position, _root);
            results = new List<ResultNode>(count);
            Retrospective(in position, leaf, results);
            ResetVisit(_root);
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
            if (left != null && left.Count != 0)
            {
                root.left = new KdTreeNode();
                BuildTree(root, root.left, _includeY ? (short)((split + 1) % 3) : (short)((split + 1) % 2), left);
            }
            if (right != null && right.Count != 0)
            {
                root.right = new KdTreeNode();
                BuildTree(root, root.right, _includeY ? (short)((split + 1) % 3) : (short)((split + 1) % 2), right);
            }
        }

        private void Retrospective(in Vector3 position, KdTreeNode cur, List<ResultNode> results)
        {
            if (cur == null) return;
            if (cur.visit)
            {
                Retrospective(in position, cur.parent, results);
                return;
            }
            cur.visit = true;
            Campare(in position, cur, results);
            if (cur.IsLeaf)
            {
                Retrospective(in position, cur.parent, results);
                return;
            }

            float distance = _util.CalculateDistanceBySplit(in position, cur);

            if (results.Count == results.Capacity && distance > results[results.Count - 1].distance)
            {
                Retrospective(in position, cur.parent, results);
                return;
            }
            if (cur.right != null && !cur.right.visit)
            {
                KdTreeNode node = _util.FindLeafNode(in position, cur.right);
                Retrospective(in position, node, results);
            }
            else if (cur.left != null && !cur.left.visit)
            {
                KdTreeNode node = _util.FindLeafNode(in position, cur.left);
                Retrospective(in position, node, results);
            }
            else
            {
                Retrospective(in position, cur.parent, results);
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
