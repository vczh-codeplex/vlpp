using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AutoShell.PublicShell
{
    public static class PublicShellCore
    {
        public static readonly Guid MainMenuGroup = new Guid("{E826A85B-EB1A-4704-9376-5A8CB56E9665}");
        public static readonly Guid FileMenuGroup = new Guid("{3F0A47C7-64AF-4944-BC29-22CFFF346F7B}");
        public static readonly Guid ViewMenuGroup = new Guid("{F3206F35-6C13-4563-86C1-A4B0A35CED9A}");
        public static readonly Guid ViewWindowsMenuGroup = new Guid("{7438390B-AF81-4cf9-8511-578FD8B3C460}");
        public static readonly Guid ViewToolbarsMenuGroup = new Guid("{9CF7DCFE-7ED7-44e4-8610-9E6AEFE40271}");
        public static readonly Guid ToolsMenuGroup = new Guid("{32E0885D-EFCC-4f7e-B622-4D5C67AD648C}");
        public static readonly Guid WindowMenuGroup = new Guid("{0C33C634-4FF4-4e8d-A40C-F617EFF7816E}");
        public static readonly Guid HelpMenuGroup = new Guid("{68639515-B6B6-4d89-B493-026188FF0797}");

        public static readonly Guid CommandWindow = new Guid("{7EEA0241-BF64-4b32-A754-E6C43C3CCC6E}");
        public static readonly Guid OutputWindow = new Guid("{D90635DB-2D1C-4299-8FA1-673905BE5B42}");
        public static readonly Guid LoggerOutputChannel = new Guid("{6791515D-38A4-43a7-9781-A9B376BA9B01}");
        public static readonly Guid EnvironmentOptions = new Guid("{B7E7BE4E-0205-4439-BBD2-AD5A04ED8C88}");

        public static readonly string Command_File_Exit = "File.Exit";
        public static readonly string Command_View_CommandWindow = "View.CommandWindow";
        public static readonly string Command_View_OutputWindow = "View.OutputWindow";
        public static readonly string Command_Tools_Options = "Tools.Options";
        public static readonly string Command_Help = "Help";
        public static readonly string Command_Clear = "Clear";
    }
}