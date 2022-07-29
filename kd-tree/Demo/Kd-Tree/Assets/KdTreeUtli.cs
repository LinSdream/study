using System.Collections.Generic;
using UnityEngine;

namespace Alg
{
    internal class KdTreeUtli
    {
        public Transform Filtrate(short split, List<Transform> arr, out List<Transform> left, out List<Transform> right)
        {
            left = null;
            right = null;

            if (arr == null) return null;
            if (arr.Count == 1)
            {
                return arr[0];
            }
            List<Transform> tmp = new List<Transform>(arr);
            tmp.Sort((left, right) =>
            {
                var v1 = GetValueBySplit(left.position, split);
                var v2 = GetValueBySplit(right.position, split);
                if (v1 < v2) return -1;
                else if (v1 == v2) return 0;
                else return 1;
            });


            int index = (tmp.Count - 1) / 2;
            if (index != 0)
            {
                left = new List<Transform>(index);
                for (int i = 0; i < index; i++)
                {
                    left.Add(tmp[i]);
                }
            }
            int length = tmp.Count - 1 - index;
            if (length != 0)
            {
                right = new List<Transform>(length);
                for (int i = 0; i < length; i++)
                {
                    right.Add(tmp[index + 1 + i]);
                }
            }
            return tmp[index];
        }

        public float GetValueBySplit(Vector3 value, short split)
        {
            if (split == 0)
            {
                return value.x;
            }
            else if (split == 1)
            {
                return value.y;
            }
            else if (split == 2)
            {
                return value.z;
            }
            return 0f;
        }

        public KdTreeNode FindLeafNode(in Vector3 position, KdTreeNode node)
        {
            if (node.IsLeaf) return node;
            if (GetValueBySplit(position, node.split) < GetValueBySplit(position, node.split))
            {
                return FindLeafNode(position, node.left);
            }
            return FindLeafNode(position, node.right);
        }

        public float Distance(in Vector3 p1, in Vector3 p2)
        {
            return (p1 - p2).sqrMagnitude;
        }

        public float CalculateDistanceBySplit(in Vector3 position, KdTreeNode node)
        {
            Vector3 pos = node.value.position;
            if(node.split == 0)
            {
                pos.y = 0f;
                pos.z = 0f;
            }
            else if(node.split == 1)
            {
                pos.x = 0f;
                pos.z = 0f;
            }
            else if (node.split == 2)
            {
                pos.x = 0f;
                pos.y = 0f;
            }
            return Distance(in pos, in position);
        }
    }
}
