using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace SeatBuilderConsole
{
    class PersonWorksheet
    {
        public string Name { get; set; }
        public string[] Columns { get; set; }
        public Dictionary<string, string>[] Rows { get; set; }

        private string ReadCell(XElement cell)
        {
            var data = cell.Elements().Where(e => e.Name.LocalName == "Data").FirstOrDefault();
            if (data != null)
            {
                return data.Value;
            }
            else
            {
                return "";
            }
        }

        private string[] ReadRow(XElement row)
        {
            return row
                .Elements()
                .Where(e => e.Name.LocalName == "Cell")
                .Select(e => ReadCell(e))
                .ToArray();
        }

        public PersonWorksheet(XElement worksheet)
        {
            this.Name = worksheet
                .Attributes()
                .Where(a => a.Name.LocalName == "Name")
                .First()
                .Value;
            var table = worksheet
                .Elements()
                .Where(e => e.Name.LocalName == "Table")
                .First();
            var rows = table
                .Elements()
                .Where(e => e.Name.LocalName == "Row")
                .Select(e => ReadRow(e))
                .ToArray();

            List<string> columns = null;
            List<string[]> rowsValues = new List<string[]>();
            int nameIndex = -1;
            foreach (var row in rows)
            {
                if (columns == null)
                {
                    if (row.Any(s => s == "姓名"))
                    {
                        List<string> values = row.ToList();
                        while (values.Count > 0)
                        {
                            if (string.IsNullOrWhiteSpace(values.Last()))
                            {
                                values.RemoveAt(values.Count - 1);
                            }
                            else
                            {
                                break;
                            }
                        }
                        columns = values.ToList();
                        nameIndex = columns.IndexOf("姓名");
                    }
                }
                else if (row.Length > nameIndex && !string.IsNullOrWhiteSpace(row[nameIndex]))
                {
                    rowsValues.Add(row
                        .Select(r => r.Trim())
                        .Concat(Enumerable
                            .Range(0, columns.Count)
                            .Select(i => "")
                            )
                        .Take(columns.Count)
                        .ToArray()
                        );
                }
            }

            Columns = columns.ToArray();
            Rows = rowsValues
                .Select(row => columns
                    .Zip(row, Tuple.Create)
                    .ToDictionary(t => t.Item1, t => t.Item2)
                    )
                .ToArray();
        }
    }

    class PersonWorkbook
    {
        public PersonWorksheet[] Worksheets { get; set; }

        public PersonWorkbook(string path)
        {
            XDocument workbook = XDocument.Load(path);
            this.Worksheets = workbook
                .Root
                .Elements()
                .Where(e => e.Name.LocalName == "Worksheet")
                .Select(e => new PersonWorksheet(e))
                .ToArray();
        }
    }
}
