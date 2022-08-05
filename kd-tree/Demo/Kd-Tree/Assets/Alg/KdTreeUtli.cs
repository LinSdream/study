using System;
using System.Collections.Generic;
using UnityEngine;

namespace Alg
{
    internal class KdTreeUtli
    {
        private bool _includeY = true;

        public KdTreeUtli(bool includeY)
        {
            _includeY = includeY;
        }

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

            //int index = BFPRT(tmp, 0, tmp.Count - 1, (tmp.Count - 1) / 2, (left, right) =>
            //   {
            //       var v1 = GetValueBySplit(left.position, split);
            //       var v2 = GetValueBySplit(right.position, split);
            //       if (v1 < v2) return -1;
            //       else if (v1 == v2) return 0;
            //       else return 1;
            //   });
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
            if (_includeY)
                return value[split];
            if (split == 1) return value.z;
            return value.x;
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
            //点面距离
            Plane plane = new Plane(node.value.up, node.value.position);
            float distance = plane.GetDistanceToPoint(position);
            return Mathf.Pow(distance, 2);
        }

        private int InsertSort<T>(List<T> arr, int left, int right, Comparison<T> comparison)
        {
            for (int i = left + 1; i <= right; i++)
            {
                T temp = arr[i];
                int j;
                for (j = i; j > left && comparison(arr[j - 1], temp) >= 0; j--)
                {
                    arr[j] = arr[j - 1];
                }
                arr[j] = temp;
            }
            return (left + right) >> 1;
        }

        private int GetPivotIndex<T>(List<T> arr, int left, int right, Comparison<T> comparison)
        {
            if (right - left < 5) return InsertSort<T>(arr, left, right, comparison);
            int back = left - 1;
            for (int i = left; i + 4 < right; i += 5)
            {
                int index = InsertSort<T>(arr, i, i + 4, comparison);
                Swap(arr, ++back, index);
            }
            return BFPRT(arr, left, back, ((left + back) >> 1) + 1, comparison);
        }

        private int Partition<T>(List<T> arr, int left, int right, int pivot, Comparison<T> comparison)
        {
            Swap(arr, right, pivot);
            int mid = left;
            for (int i = left; i < right; i++)
            {
                if (comparison(arr[i], arr[right]) <= 0)
                {
                    Swap(arr, i, mid++);
                }
            }
            Swap(arr, right, mid);
            return mid;
        }


        private int BFPRT<T>(List<T> arr, int left, int right, int k, Comparison<T> comparison)
        {
            if (arr.Count < 5)
            {
                arr.Sort(comparison);
                return k;
            }

            int pivot = GetPivotIndex(arr, left, right, comparison);
            int mid = Partition(arr, left, right, pivot, comparison);
            int count = mid - left + 1;
            if (count == k) return mid;
            if (count > k) return BFPRT(arr, left, mid - 1, k, comparison);
            return BFPRT(arr, mid + 1, right, k - count, comparison);
        }

        private void Swap<T>(List<T> arr, int left, int right)
        {
            T temp = arr[left];
            arr[left] = arr[right];
            arr[right] = temp;
        }
    }
}
