using UnityEngine;

namespace Alg
{
    public class KdTreeNode
    {
        public KdTreeNode parent;
        public KdTreeNode left;
        public KdTreeNode right;
        public Transform value;

        public short split;
        public bool IsLeaf
        {
            get
            {
                return split == -1;
            }
            set
            {
                if (!value) return;
                split = -1;
            }
        }
    }
}