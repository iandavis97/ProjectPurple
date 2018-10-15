using System;
using System.IO;
using System.Windows;
using Caliburn.Micro;
using Microsoft.Win32;
using Newtonsoft.Json.Linq;
using OfficeOpenXml;
using System.Linq;
using System.Windows.Media;
using System.Text;
using System.ComponentModel;
using IniParser;
using IniParser.Model;

namespace CardParser
{
    public class CardParserViewModel : PropertyChangedBase
    {
        public class LogViewModel : PropertyChangedBase
        {
            private string _message;
            private string _prefix;
            private Brush _foreground;

            public string Text => $"[{Prefix}] {Message}";

            public string Message
            {
                get { return _message; }
                set
                {
                    _message = value;
                    NotifyOfPropertyChange(() => Text);
                }
            }

            public string Prefix
            {
                get { return _prefix; }
                set
                {
                    _prefix = value;
                    NotifyOfPropertyChange(() => Prefix);
                }
            }

            public Brush Foreground
            {
                get { return _foreground; }
                set
                {
                    _foreground = value;
                    NotifyOfPropertyChange(() => Foreground);
                }
            }

            public static LogViewModel Error(string message) => new LogViewModel
            {
                Foreground = new SolidColorBrush(Colors.DarkRed),
                Prefix = "Error",
                Message = message,
            };


            public static LogViewModel Warn(string message) => new LogViewModel
            {
                Foreground = new SolidColorBrush(Colors.OrangeRed),
                Prefix = "Warn",
                Message = message,
            };

            public static LogViewModel Log(string message) => new LogViewModel
            {
                Foreground = new SolidColorBrush(Colors.DarkBlue),
                Prefix = "Log",
                Message = message,
            };

        }

        public BindableCollection<LogViewModel> Logs { get; } = new BindableCollection<LogViewModel>();

        public bool ShouldGenerateRawJson { get; set; }

        public string SpreadsheetPath { get; set; }

        private const string IniPath = "config.ini";
        private const string FileNotSetWarning = "";
        private FileIniDataParser parser;
        private IniData data;

        public CardParserViewModel()
        {
            parser = new FileIniDataParser();

            if (!File.Exists(IniPath))
            {
                File.CreateText("config.ini").Close();
            }

            data = parser.ReadFile(IniPath);

            SpreadsheetPath = data.Global["spreadsheet-path"] ?? "";
        }

        public void GenerateJson()
        {
            JObject json = ParseSpreadsheet();

            if (json == null)
            {
                return;
            }

            if (ShouldGenerateRawJson)
            {
                SaveRawJson(json);
            }
            else
            {
                SaveJsonIntoTemplate(json);
            }

            parser.ReadFile(IniPath);
        }

        public void OpenSpreadsheet()
        {
            // Create OpenFileDialog 
            var openFileDialog = new OpenFileDialog
            {
                // Set filter for file extension and default file extension 
                FileName = SpreadsheetPath,
                DefaultExt = ".xlsx",
                Filter = "Excel Worksheet (*.xlsx)|*.xlsx"
            };

            if (!string.IsNullOrWhiteSpace(SpreadsheetPath))
            {
                openFileDialog.InitialDirectory = Path.GetDirectoryName(SpreadsheetPath);
            }


            // Display OpenFileDialog by calling ShowDialog method 
            var result = openFileDialog.ShowDialog();


            // Get the selected file name and display in a TextBox 
            if (result != true)
            {
                return;
            }

            SpreadsheetPath = openFileDialog.FileName;
            NotifyOfPropertyChange("SpreadsheetPath");
            data.Global["spreadsheet-path"] = SpreadsheetPath;
            parser.WriteFile(IniPath, data);
        }

        private void SaveJsonIntoTemplate(JObject json)
        {
            var saveFileDialog = new SaveFileDialog
            {
                InitialDirectory = Path.GetDirectoryName(data.Global["cpp-path"] ?? SpreadsheetPath),
                FileName = Path.GetFileName(data.Global["cpp-path"] ?? "CardData.cpp"),
                DefaultExt = ".cpp",
                Filter = "C++ File (*.cpp)|*.cpp"
            };
           
            var result = saveFileDialog.ShowDialog();
            if (result != true)
            {
                Logs.Add(LogViewModel.Warn("Operation canceled!"));
                return;
            }

            var fileName = saveFileDialog.FileName;

            string cardData;
            try
            {
                cardData = CreateCardDataCpp(json.ToString());
            }
            catch (Exception e)
            {
                Logs.Add(LogViewModel.Error($"An error ocurred while trying to open the template file ({e.Message})."));
                return;
            }

            try
            {
                Directory.CreateDirectory(Path.GetDirectoryName(fileName));
                File.WriteAllText(fileName, cardData);
            }
            catch (Exception e)
            {
                Logs.Add(LogViewModel.Error($"An error ocurred while trying to save the file ({e.Message})."));
                return;
            }

            data.Global["cpp-path"] = fileName;
            parser.WriteFile(IniPath, data);

            Logs.Add(LogViewModel.Log($"Asset saved at {fileName}!"));
        }

        private void SaveRawJson(JObject json)
        {
            var saveFileDialog = new SaveFileDialog
            {
                InitialDirectory = Path.GetDirectoryName(data.Global["json-path"] ?? SpreadsheetPath),
                FileName = Path.GetFileName(data.Global["json-path"] ?? "result.json"),
                DefaultExt = ".json",
                Filter = "Json Asset (*.json)|*.json"
            };

            var result = saveFileDialog.ShowDialog();
            if (result != true)
            {
                Logs.Add(LogViewModel.Warn("Operation canceled!"));
                return;
            }

            var jsonFileName = saveFileDialog.FileName;

            try
            {
                Directory.CreateDirectory(Path.GetDirectoryName(jsonFileName));
                File.WriteAllText(jsonFileName, json.ToString());
            }
            catch (Exception e)
            {
                Logs.Add(LogViewModel.Error($"An error ocurred while trying to save the file ({e.Message})."));
            }

            data.Global["json-path"] = jsonFileName;
            parser.WriteFile(IniPath, data);

            Logs.Add(LogViewModel.Log($"Json asset saved at {jsonFileName}!"));
        }

        private JObject ParseSpreadsheet()
        {
            Logs.Add(LogViewModel.Log($"Opening {SpreadsheetPath}..."));
            // Read excel sheet and create json from here...

            if (!File.Exists(SpreadsheetPath))
            {
                Logs.Add(LogViewModel.Error("Invalid path. Please choose a spreadsheet to parse."));
                return null;
            }

            ExcelPackage package;
            try
            {
                package = new ExcelPackage(new FileInfo(SpreadsheetPath));
            }
            catch (System.Exception)
            {
                Logs.Add(LogViewModel.Error("Couldn't open file. Please make sure that the file " +
                                            "is not being blocked by another application."));
                return null;
            }

            var json = new JObject();

            // parse cards tab by tab
            ParseSheet(json, "resources", package.Workbook.Worksheets["Resources"], ParseResources);
            ParseSheet(json, "events", package.Workbook.Worksheets["Events"], ParseEvents);
            ParseSheet(json, "communal_responsibilities", package.Workbook.Worksheets["CommunalResps"], ParseCommunalResponsibilities);
            ParseSheet(json, "family_responsibilities", package.Workbook.Worksheets["FamilyResps"], ParseFamilyResponsibilities);
            ParseSheet(json, "roles", package.Workbook.Worksheets["Roles"], ParseRoles); 

            // close package
            package.Dispose();

            return json;
        }

        private string CreateCardDataCpp(string json)
        {
            const string templatePath = "CardData.template.cpp";
            const int numColumns = 8;

            StringBuilder builder = new StringBuilder(json.Length * 8);

            var column = 0;
            foreach (var character in json)
            {
                if (column == 0)
                {
                    builder.Append("\r\n\t");
                }

                var b = Convert.ToUInt16(character);
                builder.Append($" {b,4},");

                // cycle columns
                column = (column + 1) % numColumns;
            }

            builder.Append("\r\n");

            if (!File.Exists(templatePath))
            {
                throw new Exception("Template file 'CardData.template.cpp' not found.");
            }

            return File.ReadAllText(templatePath)
                    .Replace("{size}", json.Length.ToString())
                    .Replace("{data}", builder.ToString());
        }

        private void ParseSheet(JObject json, string key, ExcelWorksheet sheet, Func<ExcelWorksheet, LogViewModel, JArray> parser)
        {
            Logs.Add(LogViewModel.Log($"Parsing {sheet.Name} sheet..."));
            try
            {
                var log = LogViewModel.Log("Parsing cards...");
                Logs.Add(log);
                var cards = parser(sheet, log);
                log.Message = $"{sheet.Name} successfully imported!";

                json.Add(key, cards);
            }
            catch (Exception e)
            {
                Logs.Add(LogViewModel.Warn($"Coudn't parse {sheet.Name} sheet! ({e.Message})"));
            }
        }

        private JArray ParseRoles(ExcelWorksheet worksheet, LogViewModel progress)
        {
            //TODO - Finish Implementing - Dakota 
            var result = new JArray();
            // iterate from the second row (first row containing data)
            // to the last data row
            for (int i = 2; i <= worksheet.Dimension.End.Row; i++)
            {
                var card = new JObject();

                //jump empty columns
                if (worksheet.Cells[i, 1].Value == null)
                {
                    continue;
                }
                progress.Message = $"Importing role on line {i}...";

                var name = worksheet.Cells[i, 1].GetValue<string>();
                card.Add("name", new JValue(name));

                var startingResources = worksheet.Cells[i, 2].GetValue<string>();
                card.Add("startingResources", new JValue(startingResources));

                var text = worksheet.Cells[i, 3].GetValue<string>();
                card.Add("text", new JValue(text));

                var type = worksheet.Cells[i, 4].GetValue<string>();
                card.Add("type", new JValue(type));

                var owned = worksheet.Cells[i, 5].GetValue<string>();
                card.Add("ownedObjects", new JValue(owned));

                var image = worksheet.Cells[i, 6].GetValue<string>();
                card.Add("image", new JValue(image));

                result.Add(card); 
            }

            return result; 
        }

        private JArray ParseResources(ExcelWorksheet worksheet, LogViewModel progress)
        {
            var result = new JArray();
            // iterate from the second row (first row containing data)
            // to the last data row
            for (int i = 2; i <= worksheet.Dimension.End.Row; i++)
            {
                var card = new JObject();

                // jump empty columns
                if (worksheet.Cells[i, 1].Value == null)
                {
                    continue;
                }

                progress.Message = $"Importing resource on line {i}...";

                var name = worksheet.Cells[i, 1].GetValue<string>();
                card.Add("name", new JValue(name));

                var type = worksheet.Cells[i, 2].GetValue<string>();
                card.Add("type", new JValue(type));

                var owner = worksheet.Cells[i, 3].GetValue<string>();
                card.Add("owner", new JValue(owner));

                var text = worksheet.Cells[i, 4].GetValue<string>();
                card.Add("text", new JValue(text));

                var value = worksheet.Cells[i, 5].GetValue<int>();
                card.Add("value", new JValue(value));

                var count = worksheet.Cells[i, 6].GetValue<int>();
                card.Add("count", new JValue(count));

                var image = worksheet.Cells[i, 7].GetValue<string>();
                card.Add("image", new JValue(image));

                var imageIcon = worksheet.Cells[i, 8].GetValue<string>();
                card.Add("iconImage", new JValue(imageIcon)); 

                result.Add(card);
            }

            return result;
        }

        private JArray ParseCommunalResponsibilities(ExcelWorksheet worksheet, LogViewModel progress)
        {
            var result = new JArray();
            // iterate from the second row (first row containing data)
            // to the last data row
            for (int i = 2; i <= worksheet.Dimension.End.Row; i++)
            {
                var card = new JObject();

                // jump empty columns
                if (worksheet.Cells[i, 1].Value == null)
                {
                    continue;
                }

                progress.Message = $"Importing resource on line {i}...";

                var name = worksheet.Cells[i, 1].GetValue<string>();
                card.Add("name", new JValue(name));

                var type = worksheet.Cells[i, 2].GetValue<string>();
                card.Add("type", new JValue(type));

                var count = worksheet.Cells[i, 3].GetValue<int>();
                card.Add("count", new JValue(count));

                // text

                if (worksheet.Cells[i, 5].Value != null)
                {
                    var category = worksheet.Cells[i, 5].GetValue<string>();
                    card.Add("category", new JValue(category));
                }

                var bonusFrom = worksheet.Cells[i, 6].GetValue<string>();
                card.Add("bonus_from", new JValue(bonusFrom));

                var text = worksheet.Cells[i, 7].GetValue<string>();
                card.Add("text", new JValue(text));

                var cost = worksheet.Cells[i, 8].GetValue<int>();
                card.Add("cost", new JValue(cost));

                var bonusValue = worksheet.Cells[i, 9].GetValue<int>();
                card.Add("bonus", new JValue(bonusValue));

                var bonusToCom = worksheet.Cells[i, 10].GetValue<string>();
                card.Add("bonus_to_communal", new JValue(bonusToCom));

                var bonusToFam = worksheet.Cells[i, 11].GetValue<string>();
                card.Add("bonus_to_family", new JValue(bonusToFam));

                if (worksheet.Cells[i, 12].Value != null)
                {
                    var additionalText = worksheet.Cells[i, 12].GetValue<string>();
                    card.Add("additional_text", new JValue(additionalText));
                }

                if (worksheet.Cells[i, 13].Value != null)
                {
                    var additionalBonus = worksheet.Cells[i, 13].GetValue<int>();
                    card.Add("additional_bonus", new JValue(additionalBonus));
                }

                var image = worksheet.Cells[i, 14].GetValue<string>();
                card.Add("image", new JValue(image));

                result.Add(card);
            }

            return result;
        }


        private JArray ParseFamilyResponsibilities(ExcelWorksheet worksheet, LogViewModel progress)
        {
            var result = new JArray();
            // iterate from the second row (first row containing data)
            // to the last data row
            for (int i = 2; i <= worksheet.Dimension.End.Row; i++)
            {
                var card = new JObject();

                // jump empty columns
                if (worksheet.Cells[i, 1].Value == null)
                {
                    continue;
                }

                progress.Message = $"Importing resource on line {i}...";

                var name = worksheet.Cells[i, 1].GetValue<string>();
                card.Add("name", new JValue(name));

                var type = worksheet.Cells[i, 2].GetValue<string>();
                card.Add("type", new JValue(type));

                var count = worksheet.Cells[i, 3].GetValue<int>();
                card.Add("count", new JValue(count));

                // text

                if (worksheet.Cells[i, 5].Value != null)
                {
                    var category = worksheet.Cells[i, 5].GetValue<string>();
                    card.Add("category", new JValue(category));
                }

                var bonusFrom = worksheet.Cells[i, 6].GetValue<string>();
                card.Add("bonus_from", new JValue(bonusFrom));

                var text = worksheet.Cells[i, 7].GetValue<string>();
                card.Add("text", new JValue(text));

                var cost = worksheet.Cells[i, 8].GetValue<int>();
                card.Add("cost", new JValue(cost));

                var bonusValue = worksheet.Cells[i, 9].GetValue<int>();
                card.Add("bonus", new JValue(bonusValue));

                var bonusToCom = worksheet.Cells[i, 10].GetValue<string>();
                card.Add("bonus_to", new JValue(bonusToCom));

                var image = worksheet.Cells[i, 11].GetValue<string>();
                card.Add("image", new JValue(image));

                result.Add(card);
            }

            return result;
        }


        private JArray ParseEvents(ExcelWorksheet worksheet, LogViewModel progress)
        {
            var result = new JArray();
            // iterate from the second row (first row containing data)
            // to the last data row
            for (int i = 2; i <= worksheet.Dimension.End.Row; i++)
            {
                var card = new JObject();

                // jump empty columns
                if (worksheet.Cells[i, 1].Value == null)
                {
                    continue;
                }

                progress.Message = $"Importing events on line {i}...";

                var name = worksheet.Cells[i, 1].GetValue<string>();
                card.Add("name", new JValue(name));

                var type = worksheet.Cells[i, 2].GetValue<string>();
                card.Add("type", new JValue(type));

                var count = worksheet.Cells[i, 3].GetValue<int>();
                card.Add("count", new JValue(count));

                var seasons = worksheet.Cells[i, 6].GetValue<string>();
                card.Add("seasons", new JRaw($"[{seasons}]"));

                var text = worksheet.Cells[i, 7].GetValue<string>();
                card.Add("text", new JValue(text));

                if (worksheet.Cells[i, 4].Value != null)
                {
                    var halachot = worksheet.Cells[i, 4].GetValue<string>();
                    card.Add("halachot", new JValue(halachot));
                }

                if (worksheet.Cells[i, 8].Value != null)
                {
                    var collectiveCost = worksheet.Cells[i, 8].GetValue<int>();
                    card.Add("col_cost", new JValue(collectiveCost));
                }

                if (worksheet.Cells[i, 9].Value != null)
                {
                    var disasterBonusTrigger = worksheet.Cells[i, 9].GetValue<int>();
                    card.Add("bonus_trigger", new JValue(disasterBonusTrigger));
                }

                if (worksheet.Cells[i, 10].Value != null)
                {
                    var disasterBonus = worksheet.Cells[i, 10].GetValue<int>();
                    card.Add("bonus", new JValue(disasterBonus));
                }

                if (worksheet.Cells[i, 11].Value != null)
                {
                    var netValue = worksheet.Cells[i, 11].GetValue<int>();
                    card.Add("value", new JValue(netValue));
                }

                if (worksheet.Cells[i, 12].Value != null)
                {
                    var affectedPlayerText = worksheet.Cells[i, 12].GetValue<string>();
                    card.Add("player_affected", new JValue(affectedPlayerText));
                }

                if (worksheet.Cells[i, 13].Value != null)
                {
                    var flags = worksheet.Cells[i, 13].GetValue<string>();
                    var flagsArray = flags.Split(new[] { ',' }, StringSplitOptions.RemoveEmptyEntries).Select(s => $"\"{s}\"");
                    card.Add("flags", new JRaw($"[{string.Join(",", flagsArray)}]"));
                }

                var image = worksheet.Cells[i, 23].GetValue<string>();
                card.Add("image", new JValue(image));

                result.Add(card);
            }

            return result;
        }
    }
}
