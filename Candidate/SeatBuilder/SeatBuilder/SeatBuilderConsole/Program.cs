using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace SeatBuilderConsole
{
    class Program
    {
        static void Main(string[] args)
        {
            string[] groupByColumns = null;
            int defaultRoom = 0;
            Tuple<string, string, int>[] conditions = null;
            {
                XDocument inputSettings = XDocument.Load("Input.xml");
                groupByColumns = inputSettings
                    .Root
                    .Element("groupByColumns")
                    .Elements("column")
                    .Select(e => e.Value)
                    .ToArray();
                defaultRoom = int.Parse(inputSettings.Root.Element("roomSettings").Element("defaultRoom").Value);
                conditions = inputSettings
                    .Root
                    .Element("roomSettings")
                    .Element("conditions")
                    .Elements("condition")
                    .Select(e =>
                        Tuple.Create(
                            e.Element("key").Value,
                            e.Element("value").Value,
                            int.Parse(e.Element("room").Value)
                            )
                        )
                    .ToArray();
            }

            foreach (var inputPath in new string[]
                {
                    @"..\..\..\..\Input\市局中级名册（第一批）.xml",
                })
            {
                PersonWorkbook pwb = new PersonWorkbook(inputPath);
                foreach (var worksheet in pwb.Worksheets)
                {
                    SeatWorkbookBuilder swb = new SeatWorkbookBuilder("Template.xml");
                    string outputPath = string.Format(@"..\..\..\..\Output\[{0}] - [{1}].xml", Path.GetFileNameWithoutExtension(inputPath), worksheet.Name);
                    var peopleGroups = worksheet.Rows
                        .GroupBy(r => groupByColumns
                            .Select(c => r[c])
                            .Aggregate((a, b) => a + "#" + b)
                            )
                        .ToArray();

                    foreach (var peopleGroup in peopleGroups)
                    {
                        int templateIndex = defaultRoom;
                        foreach (var condition in conditions)
                        {
                            string key = condition.Item1;
                            string value = condition.Item2;
                            int roomIndex = condition.Item3;
                            var person = peopleGroup.First();
                            if (person[key] == value)
                            {
                                templateIndex = roomIndex;
                                break;
                            }
                        }
                        int totalSeats = swb.GetTemplateSeatCount(templateIndex);
                        var roomGroups = peopleGroup
                            .Select((p, i) => Tuple.Create(p, i / totalSeats))
                            .GroupBy(t => t.Item2, t => t.Item1)
                            .ToArray();
                        foreach (var roomGroup in roomGroups)
                        {
                            string type = roomGroup.First()["报考级别"];
                            string index = (roomGroup.Key + 1).ToString();
                            string major = roomGroup.First()["考试科目"];
                            var seatInfos = roomGroup
                                .Select(d => new SeatInfo
                                {
                                    Name = d["姓名"],
                                    Id = d["警号"],
                                    Type = d["单位"],
                                })
                                .ToArray();
                            swb.AddWorksheet(templateIndex, type, index, major, seatInfos);
                        }
                    }

                    swb.Save(outputPath);
                }
            }
        }
    }
}
