using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CodeBoxControl.CodeProvider
{
    public class CodeNodeList<T> : CodeNodeCollectionNodeBase, IList<T>
        where T : CodeNode
    {
        private List<T> nodes = new List<T>();

        public override IEnumerable<CodeNode> Nodes
        {
            get
            {
                return base.Nodes.Concat(this.nodes);
            }
        }

        public CodeNodeList()
        {
        }

        public CodeNodeList(IEnumerable<T> items)
        {
            this.nodes.AddRange(items);
            foreach (CodeNode node in this.nodes)
            {
                OnNodeAdded(node);
            }
        }

        #region IList<T> Members

        public int IndexOf(T item)
        {
            return this.nodes.IndexOf(item);
        }

        public void Insert(int index, T item)
        {
            this.Insert(index, item);
            OnNodeAdded(item);
        }

        public void RemoveAt(int index)
        {
            CodeNode node = this.nodes[index];
            this.nodes.RemoveAt(index);
            OnNodeRemoved(node);
        }

        public T this[int index]
        {
            get
            {
                return this.nodes[index];
            }
            set
            {
                OnNodeRemoved(this.nodes[index]);
                this.nodes[index] = value;
                OnNodeAdded(this.nodes[index]);
            }
        }

        #endregion

        #region ICollection<T> Members

        public void Add(T item)
        {
            this.nodes.Add(item);
            OnNodeAdded(item);
        }

        public void Clear()
        {
            foreach (CodeNode node in this.nodes)
            {
                OnNodeRemoved(node);
            }
        }

        public bool Contains(T item)
        {
            return this.nodes.Contains(item);
        }

        public void CopyTo(T[] array, int arrayIndex)
        {
            this.nodes.CopyTo(array, arrayIndex);
        }

        public int Count
        {
            get
            {
                return this.nodes.Count;
            }
        }

        public bool IsReadOnly
        {
            get
            {
                return false;
            }
        }

        public bool Remove(T item)
        {
            if (this.nodes.Remove(item))
            {
                OnNodeRemoved(item);
                return true;
            }
            else
            {
                return false;
            }
        }

        #endregion

        #region IEnumerable<T> Members

        public IEnumerator<T> GetEnumerator()
        {
            return this.nodes.GetEnumerator();
        }

        #endregion

        #region IEnumerable Members

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            return this.nodes.GetEnumerator();
        }

        #endregion
    }
}
