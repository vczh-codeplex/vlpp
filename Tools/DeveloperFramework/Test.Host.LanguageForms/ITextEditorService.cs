using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;
using System.Drawing;

namespace Test.Host.LanguageForms
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the interface name "ITextEditorService" in both code and config file together.
    [ServiceContract]
    public interface ITextEditorService
    {
        [OperationContract]
        Point GetSelectionAnchor();

        [OperationContract]
        Point GetSelectionCaret();

        [OperationContract]
        Point GetSelectionStart();

        [OperationContract]
        Point GetSelectionEnd();

        [OperationContract]
        int GetLineCount();

        [OperationContract]
        int GetLineLength(int index);

        [OperationContract]
        string GetString(Point p1, Point p2);

        [OperationContract]
        Point TextPositionToViewPoint(Point p);

        [OperationContract]
        int[] GetColorsOfLine(int index);
    }
}
