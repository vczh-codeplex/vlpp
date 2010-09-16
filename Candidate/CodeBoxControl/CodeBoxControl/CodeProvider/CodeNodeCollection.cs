using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CodeBoxControl.CodeProvider
{
    public class CodeNodeCollection : IEnumerable<CodeNode>
    {
        protected Dictionary<string, CodeNode> nodes = new Dictionary<string, CodeNode>();

        public CodeNode Owner { get; internal set; }

        public int Count
        {
            get
            {
                return this.nodes.Count;
            }
        }

        public virtual CodeNode this[string index]
        {
            get
            {
                CodeNode result = null;
                if (this.nodes.TryGetValue(index, out result))
                {
                    return result;
                }
                else
                {
                    return null;
                }
            }
            set
            {
                if (this.Owner != null)
                {
                    CodeNode old = this[index];
                    if (old != null)
                    {
                        old.ParentNode = null;
                    }
                }
                if (value == null)
                {
                    this.nodes.Remove(index);
                }
                else
                {
                    this.nodes[index] = value;
                    if (this.Owner != null)
                    {
                        value.ParentNode = this.Owner;
                    }
                }
            }
        }

        public IEnumerable<string> Keys
        {
            get
            {
                return this.nodes.Keys;
            }
        }

        public IEnumerator<CodeNode> GetEnumerator()
        {
            return ((IEnumerable<CodeNode>)this.nodes.Values).GetEnumerator();
        }

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            return ((IEnumerable<CodeNode>)this.nodes.Values).GetEnumerator();
        }
    }
}
