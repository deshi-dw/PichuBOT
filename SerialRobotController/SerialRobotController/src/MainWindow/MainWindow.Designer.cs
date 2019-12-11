namespace SerialRobotController {
	partial class MainWindow {
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing) {
			if (disposing && (components != null)) {
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent() {
			this.hidDropdown = new System.Windows.Forms.ComboBox();
			this.portDropdown = new System.Windows.Forms.ComboBox();
			this.consoleLog = new System.Windows.Forms.TextBox();
			this.buttonStart = new System.Windows.Forms.Button();
			this.buttonRefresh = new System.Windows.Forms.Button();
			this.consoleInput = new System.Windows.Forms.TextBox();
			this.baudRateInput = new System.Windows.Forms.TextBox();
			this.SuspendLayout();
			// 
			// hidDropdown
			// 
			this.hidDropdown.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.hidDropdown.FormattingEnabled = true;
			this.hidDropdown.Location = new System.Drawing.Point(12, 12);
			this.hidDropdown.Name = "hidDropdown";
			this.hidDropdown.Size = new System.Drawing.Size(222, 21);
			this.hidDropdown.TabIndex = 0;
			// 
			// portDropdown
			// 
			this.portDropdown.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.portDropdown.FormattingEnabled = true;
			this.portDropdown.Location = new System.Drawing.Point(12, 39);
			this.portDropdown.Name = "portDropdown";
			this.portDropdown.Size = new System.Drawing.Size(121, 21);
			this.portDropdown.TabIndex = 1;
			// 
			// consoleLog
			// 
			this.consoleLog.Location = new System.Drawing.Point(12, 66);
			this.consoleLog.Multiline = true;
			this.consoleLog.Name = "consoleLog";
			this.consoleLog.ReadOnly = true;
			this.consoleLog.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
			this.consoleLog.Size = new System.Drawing.Size(342, 246);
			this.consoleLog.TabIndex = 2;
			// 
			// buttonStart
			// 
			this.buttonStart.Location = new System.Drawing.Point(240, 11);
			this.buttonStart.Name = "buttonStart";
			this.buttonStart.Size = new System.Drawing.Size(113, 21);
			this.buttonStart.TabIndex = 3;
			this.buttonStart.Text = "Start";
			this.buttonStart.UseVisualStyleBackColor = true;
			// 
			// buttonRefresh
			// 
			this.buttonRefresh.Location = new System.Drawing.Point(241, 39);
			this.buttonRefresh.Name = "buttonRefresh";
			this.buttonRefresh.Size = new System.Drawing.Size(113, 21);
			this.buttonRefresh.TabIndex = 4;
			this.buttonRefresh.Text = "Refresh";
			this.buttonRefresh.UseVisualStyleBackColor = true;
			// 
			// consoleInput
			// 
			this.consoleInput.Location = new System.Drawing.Point(12, 318);
			this.consoleInput.Name = "consoleInput";
			this.consoleInput.Size = new System.Drawing.Size(342, 20);
			this.consoleInput.TabIndex = 5;
			// 
			// baudRateInput
			// 
			this.baudRateInput.Location = new System.Drawing.Point(137, 41);
			this.baudRateInput.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.baudRateInput.Name = "baudRateInput";
			this.baudRateInput.Size = new System.Drawing.Size(97, 20);
			this.baudRateInput.TabIndex = 6;
			// 
			// MainWindow
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(366, 350);
			this.Controls.Add(this.baudRateInput);
			this.Controls.Add(this.consoleInput);
			this.Controls.Add(this.buttonRefresh);
			this.Controls.Add(this.buttonStart);
			this.Controls.Add(this.consoleLog);
			this.Controls.Add(this.portDropdown);
			this.Controls.Add(this.hidDropdown);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "MainWindow";
			this.ShowIcon = false;
			this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
			this.Text = "Serial Robot Controller";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.ComboBox hidDropdown;
		private System.Windows.Forms.ComboBox portDropdown;
		private System.Windows.Forms.TextBox consoleLog;
		private System.Windows.Forms.Button buttonStart;
		private System.Windows.Forms.Button buttonRefresh;
		private System.Windows.Forms.TextBox consoleInput;
		private System.Windows.Forms.TextBox baudRateInput;
	}
}

