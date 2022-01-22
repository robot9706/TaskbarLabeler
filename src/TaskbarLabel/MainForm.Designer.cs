namespace TaskbarLabel
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.listLabels = new System.Windows.Forms.ListView();
            this.colPID = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.colLabel = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.colColor = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.panelLabelEdit = new System.Windows.Forms.Panel();
            this.labelEditColor = new System.Windows.Forms.Panel();
            this.label2 = new System.Windows.Forms.Label();
            this.labelEditText = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.panelRuleEdit = new System.Windows.Forms.Panel();
            this.btnDeleteRule = new System.Windows.Forms.Button();
            this.btnAddRule = new System.Windows.Forms.Button();
            this.btnTestRuleLabel = new System.Windows.Forms.Button();
            this.ruleEditTitle = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.ruleEditColor = new System.Windows.Forms.Panel();
            this.label3 = new System.Windows.Forms.Label();
            this.ruleEditLabel = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.listRules = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.btnApplyRules = new System.Windows.Forms.Button();
            this.taskbarIcon = new System.Windows.Forms.NotifyIcon(this.components);
            this.groupBox1.SuspendLayout();
            this.panelLabelEdit.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.panelRuleEdit.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            // 
            // checkBox1
            // 
            this.checkBox1.AutoSize = true;
            this.checkBox1.Location = new System.Drawing.Point(12, 21);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(168, 20);
            this.checkBox1.TabIndex = 1;
            this.checkBox1.Text = "Show taskbar item PIDs";
            this.checkBox1.UseVisualStyleBackColor = true;
            this.checkBox1.CheckedChanged += new System.EventHandler(this.checkBox1_CheckedChanged);
            // 
            // listLabels
            // 
            this.listLabels.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.listLabels.CheckBoxes = true;
            this.listLabels.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.colPID,
            this.colLabel,
            this.colColor});
            this.listLabels.FullRowSelect = true;
            this.listLabels.HideSelection = false;
            this.listLabels.Location = new System.Drawing.Point(12, 21);
            this.listLabels.MultiSelect = false;
            this.listLabels.Name = "listLabels";
            this.listLabels.Size = new System.Drawing.Size(408, 250);
            this.listLabels.TabIndex = 3;
            this.listLabels.UseCompatibleStateImageBehavior = false;
            this.listLabels.View = System.Windows.Forms.View.Details;
            this.listLabels.ItemChecked += new System.Windows.Forms.ItemCheckedEventHandler(this.listLabels_ItemChecked);
            this.listLabels.SelectedIndexChanged += new System.EventHandler(this.listLabels_SelectedIndexChanged);
            // 
            // colPID
            // 
            this.colPID.Text = "PID";
            this.colPID.Width = 97;
            // 
            // colLabel
            // 
            this.colLabel.Text = "Label";
            this.colLabel.Width = 209;
            // 
            // colColor
            // 
            this.colColor.Text = "Color";
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.groupBox1.Controls.Add(this.panelLabelEdit);
            this.groupBox1.Controls.Add(this.listLabels);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(433, 349);
            this.groupBox1.TabIndex = 6;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Labels";
            // 
            // panelLabelEdit
            // 
            this.panelLabelEdit.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panelLabelEdit.BackColor = System.Drawing.Color.White;
            this.panelLabelEdit.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelLabelEdit.Controls.Add(this.labelEditColor);
            this.panelLabelEdit.Controls.Add(this.label2);
            this.panelLabelEdit.Controls.Add(this.labelEditText);
            this.panelLabelEdit.Controls.Add(this.label1);
            this.panelLabelEdit.Enabled = false;
            this.panelLabelEdit.Location = new System.Drawing.Point(12, 276);
            this.panelLabelEdit.Name = "panelLabelEdit";
            this.panelLabelEdit.Size = new System.Drawing.Size(408, 58);
            this.panelLabelEdit.TabIndex = 6;
            // 
            // labelEditColor
            // 
            this.labelEditColor.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.labelEditColor.Location = new System.Drawing.Point(49, 29);
            this.labelEditColor.Name = "labelEditColor";
            this.labelEditColor.Size = new System.Drawing.Size(25, 25);
            this.labelEditColor.TabIndex = 7;
            this.labelEditColor.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.labelEditColor_MouseDoubleClick);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(1, 33);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(43, 16);
            this.label2.TabIndex = 6;
            this.label2.Text = "Color:";
            // 
            // labelEditText
            // 
            this.labelEditText.Location = new System.Drawing.Point(49, 3);
            this.labelEditText.Name = "labelEditText";
            this.labelEditText.Size = new System.Drawing.Size(354, 22);
            this.labelEditText.TabIndex = 5;
            this.labelEditText.TextChanged += new System.EventHandler(this.labelEditText_TextChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(-1, 6);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(45, 16);
            this.label1.TabIndex = 4;
            this.label1.Text = "Label:";
            // 
            // groupBox2
            // 
            this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.groupBox2.Controls.Add(this.panelRuleEdit);
            this.groupBox2.Controls.Add(this.listRules);
            this.groupBox2.Location = new System.Drawing.Point(451, 12);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(433, 406);
            this.groupBox2.TabIndex = 7;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Rules";
            // 
            // panelRuleEdit
            // 
            this.panelRuleEdit.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panelRuleEdit.BackColor = System.Drawing.Color.White;
            this.panelRuleEdit.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelRuleEdit.Controls.Add(this.btnDeleteRule);
            this.panelRuleEdit.Controls.Add(this.btnAddRule);
            this.panelRuleEdit.Controls.Add(this.btnTestRuleLabel);
            this.panelRuleEdit.Controls.Add(this.ruleEditTitle);
            this.panelRuleEdit.Controls.Add(this.label5);
            this.panelRuleEdit.Controls.Add(this.ruleEditColor);
            this.panelRuleEdit.Controls.Add(this.label3);
            this.panelRuleEdit.Controls.Add(this.ruleEditLabel);
            this.panelRuleEdit.Controls.Add(this.label4);
            this.panelRuleEdit.Location = new System.Drawing.Point(13, 276);
            this.panelRuleEdit.Name = "panelRuleEdit";
            this.panelRuleEdit.Size = new System.Drawing.Size(408, 120);
            this.panelRuleEdit.TabIndex = 8;
            // 
            // btnDeleteRule
            // 
            this.btnDeleteRule.Enabled = false;
            this.btnDeleteRule.Location = new System.Drawing.Point(4, 89);
            this.btnDeleteRule.Name = "btnDeleteRule";
            this.btnDeleteRule.Size = new System.Drawing.Size(136, 23);
            this.btnDeleteRule.TabIndex = 12;
            this.btnDeleteRule.Text = "Delete rule";
            this.btnDeleteRule.UseVisualStyleBackColor = true;
            this.btnDeleteRule.Click += new System.EventHandler(this.btnDeleteRule_Click);
            // 
            // btnAddRule
            // 
            this.btnAddRule.Enabled = false;
            this.btnAddRule.Location = new System.Drawing.Point(266, 89);
            this.btnAddRule.Name = "btnAddRule";
            this.btnAddRule.Size = new System.Drawing.Size(136, 23);
            this.btnAddRule.TabIndex = 11;
            this.btnAddRule.Text = "Add rule";
            this.btnAddRule.UseVisualStyleBackColor = true;
            this.btnAddRule.Click += new System.EventHandler(this.btnAddRule_Click);
            // 
            // btnTestRuleLabel
            // 
            this.btnTestRuleLabel.Location = new System.Drawing.Point(380, 57);
            this.btnTestRuleLabel.Name = "btnTestRuleLabel";
            this.btnTestRuleLabel.Size = new System.Drawing.Size(22, 25);
            this.btnTestRuleLabel.TabIndex = 10;
            this.btnTestRuleLabel.Text = "?";
            this.btnTestRuleLabel.UseVisualStyleBackColor = true;
            this.btnTestRuleLabel.Click += new System.EventHandler(this.btnTestRuleLabel_Click);
            // 
            // ruleEditTitle
            // 
            this.ruleEditTitle.Location = new System.Drawing.Point(49, 59);
            this.ruleEditTitle.Name = "ruleEditTitle";
            this.ruleEditTitle.Size = new System.Drawing.Size(325, 22);
            this.ruleEditTitle.TabIndex = 9;
            this.ruleEditTitle.TextChanged += new System.EventHandler(this.ruleEditTitle_TextChanged);
            this.ruleEditTitle.Leave += new System.EventHandler(this.ruleEditLabel_Leave);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(6, 62);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(37, 16);
            this.label5.TabIndex = 8;
            this.label5.Text = "Title:";
            // 
            // ruleEditColor
            // 
            this.ruleEditColor.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.ruleEditColor.Location = new System.Drawing.Point(49, 29);
            this.ruleEditColor.Name = "ruleEditColor";
            this.ruleEditColor.Size = new System.Drawing.Size(25, 25);
            this.ruleEditColor.TabIndex = 7;
            this.ruleEditColor.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.ruleEditColor_MouseDoubleClick);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(1, 33);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(43, 16);
            this.label3.TabIndex = 6;
            this.label3.Text = "Color:";
            // 
            // ruleEditLabel
            // 
            this.ruleEditLabel.Location = new System.Drawing.Point(49, 3);
            this.ruleEditLabel.Name = "ruleEditLabel";
            this.ruleEditLabel.Size = new System.Drawing.Size(354, 22);
            this.ruleEditLabel.TabIndex = 5;
            this.ruleEditLabel.TextChanged += new System.EventHandler(this.ruleEditLabel_TextChanged);
            this.ruleEditLabel.Leave += new System.EventHandler(this.ruleEditLabel_Leave);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(-1, 6);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(45, 16);
            this.label4.TabIndex = 4;
            this.label4.Text = "Label:";
            // 
            // listRules
            // 
            this.listRules.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.listRules.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3});
            this.listRules.FullRowSelect = true;
            this.listRules.HideSelection = false;
            this.listRules.Location = new System.Drawing.Point(13, 21);
            this.listRules.MultiSelect = false;
            this.listRules.Name = "listRules";
            this.listRules.Size = new System.Drawing.Size(408, 250);
            this.listRules.TabIndex = 7;
            this.listRules.UseCompatibleStateImageBehavior = false;
            this.listRules.View = System.Windows.Forms.View.Details;
            this.listRules.SelectedIndexChanged += new System.EventHandler(this.listRules_SelectedIndexChanged);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Label";
            this.columnHeader1.Width = 120;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Color";
            this.columnHeader2.Width = 66;
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "Title contains";
            this.columnHeader3.Width = 214;
            // 
            // groupBox3
            // 
            this.groupBox3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.groupBox3.Controls.Add(this.btnApplyRules);
            this.groupBox3.Controls.Add(this.checkBox1);
            this.groupBox3.Location = new System.Drawing.Point(12, 367);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(433, 51);
            this.groupBox3.TabIndex = 8;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Tools";
            // 
            // btnApplyRules
            // 
            this.btnApplyRules.Location = new System.Drawing.Point(291, 18);
            this.btnApplyRules.Name = "btnApplyRules";
            this.btnApplyRules.Size = new System.Drawing.Size(136, 23);
            this.btnApplyRules.TabIndex = 12;
            this.btnApplyRules.Text = "Apply rules";
            this.btnApplyRules.UseVisualStyleBackColor = true;
            this.btnApplyRules.Click += new System.EventHandler(this.btnApplyRules_Click);
            // 
            // taskbarIcon
            // 
            this.taskbarIcon.Icon = ((System.Drawing.Icon)(resources.GetObject("taskbarIcon.Icon")));
            this.taskbarIcon.Text = "Taskbar Label";
            this.taskbarIcon.Visible = true;
            this.taskbarIcon.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.taskbarIcon_MouseDoubleClick);
            this.taskbarIcon.MouseUp += new System.Windows.Forms.MouseEventHandler(this.taskbarIcon_MouseUp);
            // 
            // MainForm
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.ClientSize = new System.Drawing.Size(900, 427);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Taskbar label";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.groupBox1.ResumeLayout(false);
            this.panelLabelEdit.ResumeLayout(false);
            this.panelLabelEdit.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.panelRuleEdit.ResumeLayout(false);
            this.panelRuleEdit.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.CheckBox checkBox1;
        private System.Windows.Forms.ListView listLabels;
        private System.Windows.Forms.ColumnHeader colPID;
        private System.Windows.Forms.ColumnHeader colLabel;
        private System.Windows.Forms.ColumnHeader colColor;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.NotifyIcon taskbarIcon;
        private System.Windows.Forms.Panel panelLabelEdit;
        private System.Windows.Forms.Panel labelEditColor;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox labelEditText;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Panel panelRuleEdit;
        private System.Windows.Forms.Button btnTestRuleLabel;
        private System.Windows.Forms.TextBox ruleEditTitle;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Panel ruleEditColor;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox ruleEditLabel;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.ListView listRules;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.Button btnAddRule;
        private System.Windows.Forms.Button btnApplyRules;
        private System.Windows.Forms.Button btnDeleteRule;
    }
}

