using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace TaskbarLabel
{
    public partial class MainForm : Form
    {
        private bool doExit = false;

        private Dictionary<int, LabelEntry> labels;
        private List<Rule> rules;

        public MainForm()
        {
            InitializeComponent();

            labels = new Dictionary<int, LabelEntry>();
            rules = new List<Rule>();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            LoadRules();
        }

        #region Window state
        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (doExit)
            {
                return;
            }

            e.Cancel = true;

            Hide();
        }

        private void taskbarIcon_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                Show();
            }
        }

        private void taskbarIcon_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                doExit = true;
                Close();
            }
        }
        #endregion

        #region Label panel
        private void listLabels_SelectedIndexChanged(object sender, EventArgs e)
        {
            panelLabelEdit.Enabled = (listLabels.SelectedItems.Count > 0);

            if (panelLabelEdit.Enabled)
            {
                int pid = (int)listLabels.SelectedItems[0].Tag;
                LabelEntry label = labels[pid];

                labelEditText.Text = label.Text;
                labelEditColor.BackColor = label.BackgroundColor;
            }
            else
            {
                labelEditText.Text = "";
                labelEditColor.BackColor = Color.White;
            }
        }

        private void labelEditText_TextChanged(object sender, EventArgs e)
        {
            if (!panelLabelEdit.Enabled)
            {
                return;
            }

            int pid = (int)listLabels.SelectedItems[0].Tag;
            LabelEntry label = labels[pid];
            label.Text = labelEditText.Text;

            UpdateLabelListViewItem(listLabels.SelectedItems[0], label);
            SendLabelToTaskbar(pid);
        }

        private void labelEditColor_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            if (!panelLabelEdit.Enabled)
            {
                return;
            }

            int pid = (int)listLabels.SelectedItems[0].Tag;
            LabelEntry label = labels[pid];

            using (ColorDialog dialog = new ColorDialog())
            {
                dialog.Color = label.BackgroundColor;

                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    label.BackgroundColor = dialog.Color;

                    UpdateLabelListViewItem(listLabels.SelectedItems[0], label);
                    SendLabelToTaskbar(pid);
                }
            }
        }

        private void listLabels_ItemChecked(object sender, ItemCheckedEventArgs e)
        {
            int pid = (int)e.Item.Tag;
            LabelEntry label = labels[pid];
            label.Override = e.Item.Checked;

            SendLabelToTaskbar(pid);
        }

        private void UpdateLabelsList()
        {
            List<int> listKeys = new List<int>();
            foreach (var item in listLabels.Items)
            {
                listKeys.Add((int)((ListViewItem)item).Tag);
            }

            List<int> newKeys = labels.Keys.Where(pid => !listKeys.Contains(pid)).ToList();
            List<int> deleteKeys = listKeys.Where(key => !labels.ContainsKey(key)).ToList();

            foreach (int newKey in newKeys)
            {
                ListViewItem newItem = CreateLabelListViewItem(newKey, labels[newKey]);
                listLabels.Items.Add(newItem);
            }

            foreach (var deleteKey in deleteKeys)
            {
                listLabels.Items.RemoveByKey(deleteKey.ToString());
            }
        }

        private ListViewItem CreateLabelListViewItem(int pid, LabelEntry label)
        {
            ListViewItem item = new ListViewItem();
            item.Tag = pid;
            item.Checked = label.Override;
            item.Name = pid.ToString();
            item.Text = pid.ToString();
            item.UseItemStyleForSubItems = false;
            item.SubItems.Add(label.Text);
            item.SubItems.Add(new ListViewItem.ListViewSubItem()
            {
                BackColor = label.BackgroundColor
            });

            return item;
        }

        private void UpdateLabelListViewItem(ListViewItem item, LabelEntry label)
        {
            item.Checked = label.Override;
            item.SubItems[1].Text = label.Text;
            item.SubItems[2].BackColor = label.BackgroundColor;
        }
        #endregion

        #region Tools panel
        private void checkBox1_CheckedChanged(object sender, System.EventArgs e)
        {
            Native.SetShowPIDs(checkBox1.Checked);
        }

        private void btnApplyRules_Click(object sender, EventArgs e)
        {
            foreach (int pid in labels.Keys)
            {
                string title = GetWindowTitleByPID(pid);
                if (title == null)
                {
                    continue;
                }

                foreach (Rule rule in rules)
                {
                    if (IsRuleMatching(title, rule.TitleContains))
                    {
                        LabelEntry label = labels[pid];
                        label.Text = rule.Label;
                        label.BackgroundColor = rule.BackgroundColor;
                        label.Override = true;

                        SendLabelToTaskbar(pid);
                        UpdateLabelListViewItem(listLabels.Items[pid.ToString()], label);

                        break;
                    }
                }
            }
        }
        #endregion

        #region Rules panel
        private string RulesFile => Path.Combine(Application.StartupPath, "rules.json");

        private void LoadRules()
        {
            string file = RulesFile;
            if (File.Exists(file))
            {
                rules = JsonConvert.DeserializeObject<List<Rule>>(File.ReadAllText(file));

                foreach (Rule rule in rules)
                {
                    listRules.Items.Add(CreateRuleListViewItem(rule));
                }
            }
        }

        private void SaveRules()
        {
            string file = RulesFile;
            File.WriteAllText(file, JsonConvert.SerializeObject(rules, Formatting.Indented));
        }

        private void listRules_SelectedIndexChanged(object sender, EventArgs e)
        {
            btnAddRule.Enabled = false;
            btnDeleteRule.Enabled = (listRules.SelectedItems.Count > 0);

            if (listRules.SelectedItems.Count > 0)
            {
                Rule rule = (Rule)listRules.SelectedItems[0].Tag;

                ruleEditLabel.Text = rule.Label;
                ruleEditTitle.Text = rule.TitleContains;
                ruleEditColor.BackColor = rule.BackgroundColor;
            }
            else
            {
                ruleEditLabel.Text = "";
                ruleEditTitle.Text = "";
                ruleEditColor.BackColor = Color.White;
            }
        }

        private void CheckAddRuleButton()
        {
            btnAddRule.Enabled = (listRules.SelectedItems.Count == 0 && !string.IsNullOrEmpty(ruleEditLabel.Text) && !string.IsNullOrEmpty(ruleEditTitle.Text));
        }

        private void btnTestRuleLabel_Click(object sender, EventArgs e)
        {
            string rule = ruleEditTitle.Text;

            string text = "";

            foreach (int pid in labels.Keys)
            {
                string title = GetWindowTitleByPID(pid);
                if (title == null)
                {
                    continue;
                }

                if (IsRuleMatching(title, rule))
                {
                    text += (text.Length > 0 ? (", " + pid.ToString()) : pid.ToString());
                }
            }

            MessageBox.Show(this, "Taskbar button PIDs matching:\n" + text, "Test", MessageBoxButtons.OK);
        }

        private void ruleEditLabel_TextChanged(object sender, EventArgs e)
        {
            if (listRules.SelectedItems.Count > 0)
            {
                Rule rule = (Rule)listRules.SelectedItems[0].Tag;
                rule.Label = ruleEditLabel.Text;

                UpdateRuleListViewItem(listRules.SelectedItems[0], rule);
            }
            else
            {
                CheckAddRuleButton();
            }
        }

        private void ruleEditTitle_TextChanged(object sender, EventArgs e)
        {
            if (listRules.SelectedItems.Count > 0)
            {
                Rule rule = (Rule)listRules.SelectedItems[0].Tag;
                rule.TitleContains = ruleEditTitle.Text;

                UpdateRuleListViewItem(listRules.SelectedItems[0], rule);
            }
            else
            {
                CheckAddRuleButton();
            }
        }

        private void ruleEditColor_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            using (ColorDialog dialog = new ColorDialog())
            {
                if (listRules.SelectedItems.Count > 0)
                {
                    Rule rule = (Rule)listRules.SelectedItems[0].Tag;
                    dialog.Color = rule.BackgroundColor;
                }

                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    if (listRules.SelectedItems.Count > 0)
                    {
                        Rule rule = (Rule)listRules.SelectedItems[0].Tag;
                        rule.BackgroundColor = dialog.Color;

                        UpdateRuleListViewItem(listRules.SelectedItems[0], rule);
                        SaveRules();
                    }

                    ruleEditColor.BackColor = dialog.Color;
                }
            }
        }

        private void ruleEditLabel_Leave(object sender, EventArgs e)
        {
            SaveRules();
        }

        private void btnAddRule_Click(object sender, EventArgs e)
        {
            Rule rule = new Rule();
            rule.BackgroundColor = ruleEditColor.BackColor;
            rule.Label = ruleEditLabel.Text;
            rule.TitleContains = ruleEditTitle.Text;

            rules.Add(rule);

            listRules.Items.Add(CreateRuleListViewItem(rule));

            listRules.SelectedItems.Clear();
            listRules_SelectedIndexChanged(sender, e);

            SaveRules();
        }

        private void btnDeleteRule_Click(object sender, EventArgs e)
        {
            Rule delete = (Rule)listRules.SelectedItems[0].Tag;
            rules.Remove(delete);

            listRules.Items.Remove(listRules.SelectedItems[0]);

            SaveRules();

            listRules_SelectedIndexChanged(sender, e);
        }

        private ListViewItem CreateRuleListViewItem(Rule rule)
        {
            ListViewItem item = new ListViewItem();

            item.Tag = rule;
            item.Text = rule.Label;
            item.UseItemStyleForSubItems = false;

            item.SubItems.Add(new ListViewItem.ListViewSubItem()
            {
                BackColor = rule.BackgroundColor
            });
            item.SubItems.Add(rule.TitleContains);

            return item;
        }

        private void UpdateRuleListViewItem(ListViewItem item, Rule rule)
        {
            item.Text = rule.Label;
            item.SubItems[1].BackColor = rule.BackgroundColor;
            item.SubItems[2].Text = rule.TitleContains;
        }

        private bool IsRuleMatching(string title, string rule)
        {
            return title.ToLower().Contains(rule.ToLower());
        }
        #endregion

        #region Taskbar communication
        private string GetWindowTitleByPID(int pid)
        {
            StringBuilder title = new StringBuilder(128);
            int len = Native.GetWindowTitleByPID(pid, title, title.Capacity);
            if (len < 0)
            {
                // No window found
                return null;
            }

            return title.ToString();
        }

        private void SendLabelToTaskbar(int pid)
        {
            LabelEntry label = labels[pid];

            if (label.Override)
            {
                StringBuilder sb = new StringBuilder(64);
                if (label.Text.Length > sb.Capacity)
                {
                    sb.Append(label.Text.Substring(0, sb.Capacity));
                }
                else
                {
                    sb.Append(label.Text);
                }

                Color managedColor = label.BackgroundColor;
                int color = managedColor.R | (managedColor.G << 8) | (managedColor.B << 16);

                Native.AddLabel(pid, color, sb);
            }
            else
            {
                Native.RemoveLabel(pid);
            }
        }

        protected override void WndProc(ref Message m)
        {
            if (m.Msg == Native.WM_COPYDATA)
            {
                int[] pids = new int[128];
                int count = 0;

                if (Native.GetButtonPIDs(m.LParam, pids, ref count))
                {
                    UpdateLabelsFromTaskbar(pids.Take(count));
                    UpdateLabelsList();

                    m.Result = IntPtr.Zero;
                    return;
                }
            }

            base.WndProc(ref m);
        }

        private void UpdateLabelsFromTaskbar(IEnumerable<int> currentPIDs)
        {
            List<int> newPIDs = currentPIDs.Where(pid => !labels.ContainsKey(pid)).Distinct().ToList();
            List<int> deletePIDs = labels.Keys.Where(pid => !currentPIDs.Contains(pid)).ToList();

            foreach (int newPID in newPIDs)
            {
                string title = GetWindowTitleByPID(newPID);
                if (title == null)
                {
                    continue;
                }

                LabelEntry label = new LabelEntry();

                bool hasRule = false;
                foreach (Rule rule in rules)
                {
                    if (IsRuleMatching(title, rule.TitleContains))
                    {
                        label.Text = rule.Label;
                        label.BackgroundColor = rule.BackgroundColor;
                        label.Override = true;

                        hasRule = true;
                        break;
                    }
                }

                if (!hasRule)
                {
                    label.Override = false;
                    label.Text = newPID.ToString();
                    label.BackgroundColor = Color.Black;
                }

                labels.Add(newPID, label);

                if (hasRule)
                {
                    SendLabelToTaskbar(newPID);
                }
            }

            foreach (int deletePID in deletePIDs)
            {
                labels.Remove(deletePID);
            }
        }
        #endregion
    }
}
