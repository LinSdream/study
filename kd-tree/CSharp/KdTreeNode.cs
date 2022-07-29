namespace Alg
{
    internal class KdTreeNode<T>
    {
        public KdTreeNode<T> parent;
        public KdTreeNode<T> left;
        public KdTreeNode<T> right;
        public short split;
        public T value;
    }
}
