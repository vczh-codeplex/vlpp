using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Xml.Linq;

namespace Jumper
{
    class GameSettings
    {
        private readonly string settingFilePath = Application.ExecutablePath + ".configuration.xml";
        private Dictionary<string, string> values = new Dictionary<string, string>();

        private void Load()
        {
            try
            {
                if (File.Exists(settingFilePath))
                {
                    XDocument document = XDocument.Load(this.settingFilePath);
                    foreach (var setting in document.Root.Elements("Setting"))
                    {
                        this.values.Add(setting.Attribute("Key").Value, setting.Attribute("Value").Value);
                    }
                }
            }
            catch (Exception ex)
            {
                this.values.Clear();
                MessageBox.Show(ex.Message, "Jumper v1.0");
            }
        }

        private void Save()
        {
            try
            {
                XDocument document = new XDocument(
                    new XElement("Settings",
                        this.values.Select(p => new XElement("Setting", new XAttribute("Key", p.Key), new XAttribute("Value", p.Value))).ToArray()
                        )
                    );
                document.Save(this.settingFilePath);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Jumper v1.0");
            }
        }

        public GameSettings()
        {
            Load();
        }

        public string Get(string key)
        {
            return this.values[key];
        }

        public void Set(string key, string value)
        {
            this.values[key] = value;
            Save();
        }

        public int HigestScore
        {
            get
            {
                try
                {
                    return int.Parse(Get("HigestScore"));
                }
                catch (Exception)
                {
                    return 0;
                }
            }
            set
            {
                Set("HigestScore", value.ToString());
            }
        }
    }
}
