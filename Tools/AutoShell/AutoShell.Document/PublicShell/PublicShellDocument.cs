using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AutoShell.PublicShell
{
    public static class PublicShellDocument
    {
        public static readonly Guid FileOpenMenuGroup = new Guid("{0D750880-B20F-43b2-8F3F-142B42A161E9}");
        public static readonly Guid FileSaveMenuGroup = new Guid("{9765EC60-165C-4c47-A7DF-8D5FD63F904C}");
        public static readonly Guid FileCloseMenuGroup = new Guid("{5F5D97A1-8F44-4d99-9368-52C29F24BD3E}");
        public static readonly Guid FileRecentMenuGroup = new Guid("{47466FC9-0B7F-468e-A752-EC29C660E9A1}");
        public static readonly Guid FileNewFileMenuGroup = new Guid("{A4A83FC3-57CB-43ac-A75F-9FC506A5B3F3}");
        public static readonly Guid FileOpenFileMenuGroup = new Guid("{519DEBAE-E546-4e9f-92F9-E6AF352ED56A}");

        public static readonly Guid EditMenuGroup = new Guid("{516122E4-61EA-4d9e-B425-CED49836A2E3}");
        public static readonly Guid EditUndoRedoMenuGroup = new Guid("{98D4C1F3-10A4-44e7-8875-B9AA3BBC6EAD}");
        public static readonly Guid EditClipboardMenuGroup = new Guid("{807851F3-C393-439e-81C8-7D9997B87CCB}");
        public static readonly Guid EditOperationMenUGroup = new Guid("{5474C26B-D60F-44f4-97E2-6BC823D941CF}");

        public static readonly Guid StandardToolBar = new Guid("{AECFF9D5-0275-4a53-97DA-C0517211B808}");
        public static readonly Guid StandardToolBarGroup = new Guid("{7EC52848-F3AA-450e-BE4B-B368837C3411}");
        public static readonly Guid StandardFileToolBarGroup = new Guid("{78725448-AFB4-42d7-BE13-21C5E8EB7464}");
        public static readonly Guid StandardClipboardToolBarGroup = new Guid("{ACB5439B-7547-44b1-8889-EE56CBD1EC3C}");
        public static readonly Guid StandardUndoRedoToolBarGroup = new Guid("{8A36FDAB-7121-42c1-8800-6178ACBEE8DF}");

        public static readonly string Command_File_New_File = "File.New.File";
        public static readonly string Command_File_Open_File = "File.Open.File";
        public static readonly string Command_File_Save = "File.Save";
        public static readonly string Command_File_SaveAs = "File.SaveAs";
        public static readonly string Command_File_SaveAll = "File.SaveAll";
        public static readonly string Command_File_Close = "File.Close";
        public static readonly string Command_Edit_Undo = "Edit.Undo";
        public static readonly string Command_Edit_Redo = "Edit.Redo";
        public static readonly string Command_Edit_Cut = "Edit.Cut";
        public static readonly string Command_Edit_Copy = "Edit.Copy";
        public static readonly string Command_Edit_Paste = "Edit.Paste";
        public static readonly string Command_Edit_Delete = "Edit.Delete";
        public static readonly string Command_Edit_SelectAll = "Edit.SelectAll";
    }
}
