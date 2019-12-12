using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO.Ports;
using System.Windows.Forms;
using Linearstar.Windows.RawInput;

namespace SerialRobotController {
	public partial class MainWindow : Form {
		private bool isStarted = false;
		private string currentPortName = string.Empty;

		private List<string> previousCommands = new List<string>();
		private int previousCommandIndex = 0;

		private LogitechDuelAction gamepad = new LogitechDuelAction();

		public MainWindow() {
			InitializeComponent();

			this.FormClosing += OnExit;

			buttonStart.Click += OnButtonStart;
			buttonRefresh.Click += OnButtonRefresh;

			consoleInput.KeyUp += OnConsoleInputKeyUp;

			Program.InputWindow.Input += OnControllerInput;
			Program.Ports.DataRecived += OnDataRecived;

			OnButtonRefresh(null, null);

			// Read hid index value.
			string stringHidIndex = Program.INIFile.IniReadValue("General", "HidIndex");
			if (stringHidIndex != string.Empty) {
				try {
					hidDropdown.SelectedIndex = Convert.ToInt32(stringHidIndex);
				}
				catch { }
			}

			// Read port name value.
			string stringPortName = Program.INIFile.IniReadValue("General", "PortName");
			if (stringPortName != string.Empty) {
				portDropdown.SelectedIndex = portDropdown.Items.IndexOf(stringPortName);
			}

			// Read baud rate value.
			string stringBaudRate = Program.INIFile.IniReadValue("General", "BaudRate");
			if (stringBaudRate != string.Empty) {
				baudRateInput.Text = stringBaudRate;
			}
		}
		
		private void OnDataRecived(int length) {
			if (isStarted == true) {
				PrintLine($"{currentPortName}>{Program.Ports.ReadLine()}");
			}
		}

		private void OnExit(object sender, FormClosingEventArgs e) {
			// Save the hid indx and port name to an ini file.
			Program.INIFile.IniWriteValue("General", "HidIndex", $"{hidDropdown.SelectedIndex}");
			Program.INIFile.IniWriteValue("General", "PortName", currentPortName);
			Program.INIFile.IniWriteValue("General", "BaudRate", baudRateInput.Text);

			Program.Ports.Disconnect();
		}

		private void OnControllerInput(RawInputData data) {
			if (isStarted == true && hidDropdown.SelectedIndex >= 0 && Program.Hids.ConnectedHids[hidDropdown.SelectedIndex] != null && data.Header.Type == RawInputDeviceType.Hid) {
				RawInputHidData hid = (RawInputHidData)data;

				gamepad.Update(hid.Hid.RawData);
				PrintLine($"Left({gamepad.AxisLeft.rawX},{gamepad.AxisLeft.rawY})::Right({gamepad.AxisRight.rawX},{gamepad.AxisRight.rawY})");
			}
		}

		private void OnConsoleInputKeyUp(object sender, KeyEventArgs e) {
			if (isStarted == true) {
				if (e.KeyCode == Keys.Enter) {
					if (consoleInput.Text.Trim() == "clear") {
						consoleLog.Clear();
						previousCommands.Add("clear");
					}
					else {
						if (Program.Ports.WriteCommand($"{consoleInput.Text.Trim()}")) {
							PrintLine($">{consoleInput.Text}");
							previousCommands.Add(consoleInput.Text);
							if (previousCommands.Count >= 20) {
								previousCommands.RemoveAt(0);
							}
						}
					}
					consoleInput.Clear();
				}
				else if (e.KeyCode == Keys.Up) {
					if (string.IsNullOrEmpty(consoleInput.Text) || string.IsNullOrWhiteSpace(consoleInput.Text)) {
						previousCommandIndex = previousCommands.Count - 1;
						consoleInput.Text = previousCommands[previousCommandIndex];
					}
					else {
						previousCommandIndex = Math.Max(Math.Min(previousCommandIndex - 1, previousCommands.Count - 1), 0);
						consoleInput.Text = previousCommands[previousCommandIndex];
					}

					consoleInput.SelectionStart = consoleInput.Text.Length;
				}
				else if (e.KeyCode == Keys.Down) {
					if (previousCommandIndex == previousCommands.Count - 1) {
						consoleInput.Text = string.Empty;
					}
					else {
						previousCommandIndex = Math.Max(Math.Min(previousCommandIndex + 1, previousCommands.Count - 1), 0);
						consoleInput.Text = previousCommands[previousCommandIndex];
					}

					consoleInput.SelectionStart = consoleInput.Text.Length;
				}
			}
		}

		private void OnButtonRefresh(object sender, EventArgs e) {
			// Clear dropdown tables.
			hidDropdown.Items.Clear();
			portDropdown.Items.Clear();

			// Update hid list.
			Program.Hids.UpdateConnectedHids();

			// Fill ports dropdown table with new hids.
			for (int i = Program.Hids.ConnectedHids.Length - 1; i >= 0; i--) {
				hidDropdown.Items.Add($"{Program.Hids.ConnectedHids[i].ManufacturerName} : {Program.Hids.ConnectedHids[i].ProductName}");
			}

			// Get all connected ports.
			string[] allPorts = Program.Ports.GetAllPorts();

			// Fill ports dropdown tables with new ports.
			for (int i = allPorts.Length - 1; i >= 0; i--) {
				portDropdown.Items.Add($"{allPorts[i]}");
			}
		}

		private void OnButtonStart(object sender, EventArgs e) {
			currentPortName = (string)portDropdown.SelectedItem;

			// If the application wasn't started, start it.
			if (isStarted == false) {
				if (hidDropdown.SelectedIndex < 0 && Program.Ports.Connect(currentPortName, 9600) == false) {
					PrintLine($"Failed To start. Check that the port({currentPortName}) and hid selection({hidDropdown.SelectedIndex}) is correct.");

					return;
				}

				hidDropdown.Enabled = false;
				portDropdown.Enabled = false;

				buttonRefresh.Enabled = false;
				buttonStart.Text = "Stop";

				isStarted = true;
			}
			// If the application was already started, stop it.
			else {
				hidDropdown.Enabled = true;
				portDropdown.Enabled = true;

				buttonRefresh.Enabled = true;
				buttonStart.Text = "Start";

				Program.Ports.Disconnect();

				isStarted = false;
			}
		}

		string logBuffer = string.Empty;
		Stopwatch lastPrintTime = new Stopwatch();

		public void PrintLine(string message) {
			Print($"{message}\r\n");
		}

		public void Print(string message) {
			if (lastPrintTime.IsRunning == false) {
				lastPrintTime.Start();
			}

			if (InvokeRequired == true) {
				if (lastPrintTime.ElapsedMilliseconds < 1000) {
					Invoke((MethodInvoker)delegate { Print(message); });
					lastPrintTime.Reset();
				}
				else {
					logBuffer += message;
				}
			}
			else {
				consoleLog.AppendText(logBuffer + message);
				logBuffer = string.Empty;
				lastPrintTime.Restart();
			}
		}
	}
}
