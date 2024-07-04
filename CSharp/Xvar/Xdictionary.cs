using System.Collections;
using System.Diagnostics.CodeAnalysis;

internal class Xdictionary<TKey, TVal> : IDictionary<TKey, TVal>, IDictionary, IReadOnlyDictionary<TKey, TVal>, IXvar
{
    public TVal this[TKey key] { get => throw new NotImplementedException(); set => throw new NotImplementedException(); }
    public object? this[object key] { get => throw new NotImplementedException(); set => throw new NotImplementedException(); }

    public ICollection<TKey> Keys => throw new NotImplementedException();

    public ICollection<TVal> Values => throw new NotImplementedException();

    public int Count => throw new NotImplementedException();

    public bool IsReadOnly => throw new NotImplementedException();

    public bool IsFixedSize => throw new NotImplementedException();

    public bool IsSynchronized => throw new NotImplementedException();

    public object SyncRoot => throw new NotImplementedException();

    public bool Invaild => throw new NotImplementedException();

    ICollection IDictionary.Keys => throw new NotImplementedException();

    IEnumerable<TKey> IReadOnlyDictionary<TKey, TVal>.Keys => throw new NotImplementedException();

    ICollection IDictionary.Values => throw new NotImplementedException();

    IEnumerable<TVal> IReadOnlyDictionary<TKey, TVal>.Values => throw new NotImplementedException();

    public void Add(TKey key, TVal value)
    {
        throw new NotImplementedException();
    }

    public void Add(KeyValuePair<TKey, TVal> item)
    {
        throw new NotImplementedException();
    }

    public void Add(object key, object? value)
    {
        throw new NotImplementedException();
    }

    public void AddLink(IXvar link)
    {
        throw new NotImplementedException();
    }

    public void Clear()
    {
        throw new NotImplementedException();
    }

    public bool Contains(KeyValuePair<TKey, TVal> item)
    {
        throw new NotImplementedException();
    }

    public bool Contains(object key)
    {
        throw new NotImplementedException();
    }

    public bool ContainsKey(TKey key)
    {
        throw new NotImplementedException();
    }

    public void CopyTo(KeyValuePair<TKey, TVal>[] array, int arrayIndex)
    {
        throw new NotImplementedException();
    }

    public void CopyTo(Array array, int index)
    {
        throw new NotImplementedException();
    }

    public IEnumerator<KeyValuePair<TKey, TVal>> GetEnumerator()
    {
        throw new NotImplementedException();
    }

    public bool Remove(TKey key)
    {
        throw new NotImplementedException();
    }

    public bool Remove(KeyValuePair<TKey, TVal> item)
    {
        throw new NotImplementedException();
    }

    public void Remove(object key)
    {
        throw new NotImplementedException();
    }

    public void RemoveLink(IXvar link)
    {
        throw new NotImplementedException();
    }

    public void SetDirty()
    {
        throw new NotImplementedException();
    }

    public bool TryGetValue(TKey key, [MaybeNullWhen(false)] out TVal value)
    {
        throw new NotImplementedException();
    }

    public void Unused()
    {
        throw new NotImplementedException();
    }

    IEnumerator IEnumerable.GetEnumerator()
    {
        throw new NotImplementedException();
    }

    IDictionaryEnumerator IDictionary.GetEnumerator()
    {
        throw new NotImplementedException();
    }
}
