using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Net8PluginWithWindowsForms
{
  public class CustomForm : Form
  {
    public CustomForm()
    {
      InitializeComponent();
    }

    private void InitializeComponent()
    {
      components = new Container();
      ComponentResourceManager resources = new ComponentResourceManager(typeof(CustomForm));
      label1 = new Label();
      button1 = new Button();
      label2 = new Label();
      toolTip1 = new ToolTip(components);
      SuspendLayout();
      // 
      // label1
      // 
      label1.AutoSize = true;
      label1.Font = new Font("Courier New", 11F, FontStyle.Bold);
      label1.Location = new Point(101, 51);
      label1.Name = "label1";
      label1.Size = new Size(242, 221);
      label1.TabIndex = 0;
      label1.Text = resources.GetString("label1.Text");
      toolTip1.SetToolTip(label1, "Meow");
      // 
      // button1
      // 
      button1.FlatAppearance.BorderColor = Color.IndianRed;
      button1.FlatStyle = FlatStyle.Flat;
      button1.Font = new Font("Microsoft YaHei UI", 11F);
      button1.Location = new Point(138, 284);
      button1.Name = "button1";
      button1.Size = new Size(174, 33);
      button1.TabIndex = 1;
      button1.Text = "I like it!";
      // 
      // label2
      // 
      label2.AutoSize = true;
      label2.Font = new Font("Microsoft YaHei UI", 11F, FontStyle.Bold);
      label2.Location = new Point(18, 20);
      label2.Name = "label2";
      label2.Size = new Size(400, 19);
      label2.TabIndex = 2;
      label2.Text = "Hello from Renga Net8 plugin with Windows Forms!";
      // 
      // CustomForm
      // 
      BackColor = SystemColors.Control;
      CancelButton = button1;
      ClientSize = new Size(434, 329);
      Controls.Add(label2);
      Controls.Add(button1);
      Controls.Add(label1);
      Font = new Font("Microsoft YaHei UI", 10F);
      FormBorderStyle = FormBorderStyle.FixedSingle;
      MaximizeBox = false;
      MinimizeBox = false;
      Name = "CustomForm";
      ShowIcon = false;
      StartPosition = FormStartPosition.CenterScreen;
      Text = "Net8 plugin with Windows Forms";
      ResumeLayout(false);
      PerformLayout();
    }

    private Button button1;
    private Label label2;
    private ToolTip toolTip1;
    private System.ComponentModel.IContainer components;
    private Label label1;
  }
}
