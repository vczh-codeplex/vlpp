using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace SeatBuilderConsole
{
    class SeatWorkbookBuilder
    {
        private XDocument excelWorkbook;
        private XElement[] excelWorksheetTemplates;

        public SeatWorkbookBuilder(string workbookTemplatePath)
        {
            this.excelWorkbook = XDocument.Load(workbookTemplatePath);
            this.excelWorksheetTemplates = this.excelWorkbook
                .Root
                .Elements()
                .Where(e => e.Name.LocalName == "Worksheet")
                .ToArray();
            foreach (var template in this.excelWorksheetTemplates)
            {
                template.Remove();
            }
        }

        public int GetTemplateSeatCount(int templateIndex)
        {
            var template = this.excelWorksheetTemplates[templateIndex];
            string xmlText = template.ToString();
            int totalSeats = new Regex(@"\{Name\[\d+\]\}")
                .Matches(xmlText)
                .Count;
            return totalSeats;
        }

        public void AddWorksheet(int templateIndex, string type, string index, string major, SeatInfo[] infos)
        {
            var template = this.excelWorksheetTemplates[templateIndex];
            string xmlText = template.ToString();
            int totalSeats = GetTemplateSeatCount(templateIndex);
            xmlText = xmlText.Replace("{Type}", type);
            xmlText = xmlText.Replace("{Index}", index);
            xmlText = xmlText.Replace("{Major}", major);
            for (int i = 0; i < totalSeats; i++)
            {
                SeatInfo info = i < infos.Length ? infos[i] : new SeatInfo { Name = "", Id = "", Type = "" };
                xmlText = xmlText.Replace(string.Format("{{Name[{0}]}}", i), info.Name);
                xmlText = xmlText.Replace(string.Format("{{Id[{0}]}}", i), info.Id);
                xmlText = xmlText.Replace(string.Format("{{Type[{0}]}}", i), info.Type);
            }
            XElement newWorksheet = XElement.Parse(xmlText);
            this.excelWorkbook.Root.Add(newWorksheet);
        }

        public void Save(string path)
        {
            this.excelWorkbook.Save(path);
        }
    }

    class SeatInfo
    {
        public string Name { get; set; }
        public string Id { get; set; }
        public string Type { get; set; }
    }
}
