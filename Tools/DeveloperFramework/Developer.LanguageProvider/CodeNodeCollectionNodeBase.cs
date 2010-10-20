using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Developer.LanguageProvider
{
    public class CodeNodeCollectionNodeBase : CodeNode
    {
        protected void OnNodeAdded(CodeNode node)
        {
            if (node != null)
            {
                node.ParentNode = this;
            }
        }

        protected void OnNodeRemoved(CodeNode node)
        {
            if (node != null)
            {
                node.ParentNode = null;
            }
        }
    }
}
