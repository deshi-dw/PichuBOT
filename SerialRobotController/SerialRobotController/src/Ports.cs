using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SerialRobotController {
	public class Ports {
		public event Action<int> DataRecived;

		private SerialPort port;

		public Ports() { }

		public bool IsClosing { get; private set; }

		public bool Connect(string portName, int baudRate) {
			if (port == null || (port != null && port.IsOpen == false)) {
				try {
					// Create a new port and open it.
					port = new SerialPort(portName, baudRate);
					port.DataReceived += OnDataRecived;
					port.Open();

					Program.MainWindow.PrintLine($"Connected to the port {portName} with a baud rate of {baudRate}.");
				}
				catch {
					// If creating or opening the port fails, return false.
					return false;
				}
			}
			else if (portName != port.PortName) {
				try {
					port.Close();
					port.Dispose();

					port = new SerialPort(portName, baudRate);
					port.DataReceived += OnDataRecived;
					port.Open();
				}
				catch {
					return false;
				}
			}

			// If opening and writing to the port is successful, return true.
			return true;
		}

		private void OnDataRecived(object sender, SerialDataReceivedEventArgs e) {
			if (DataRecived != null) {
				DataRecived(port.BytesToRead);
			}
		}

		public void Disconnect() {
			Clear();
			IsClosing = true;

			try {
				port.Close();
			}
			catch { }

			IsClosing = false;
		}

		public void Clear() {
			try {
				port.DiscardInBuffer();
				port.DiscardOutBuffer();
			}
			catch { }
		}

		public string[] GetAllPorts() {
			return SerialPort.GetPortNames();
		}

		public byte[] Read(int length) {
			if (length < port.BytesToRead) {
				return null;
			}

			byte[] buffer = new byte[length];
			port.Read(buffer, 0, length);

			return buffer;
		}

		public string ReadLine() {
			try {
				// return Encoding.ASCII.GetString(Encoding.Unicode.GetBytes(port.ReadLine()));
				return port.ReadLine();
			}
			catch {
				return string.Empty;
			}
		}

		public bool Write(byte[] bytes) {
			try {
				port.Write(bytes, 0, bytes.Length);
				return true;
			}
			catch {
				return false;
			}
		}

		public void WriteLine(string message) {
			port.Write($"{((char)0b0)}{message}\n");
		}

		public bool WriteCommand(string message) {
			if (string.IsNullOrEmpty(message) || string.IsNullOrWhiteSpace(message)) {
				return false;
			}

			byte id = 0b00000000;
			byte[] buffer = new byte[1];
			byte[] ascii = new byte[1];

			int spaceIndex = message.IndexOf(' ');

			if (spaceIndex <= 0) {
				spaceIndex = message.Length;
			}
			string cmdString = message.Substring(0, spaceIndex).ToLower().Trim();
			string[] argStrings = message.Substring(spaceIndex >= message.Length ? spaceIndex : spaceIndex + 1).ToLower().Trim().Split(' ');
			for (int i = argStrings.Length - 1; i >= 0; i--) {
				argStrings[i] = argStrings[i].Trim();
			}

			switch (cmdString) {
				case "help":
					WriteHelp();
					break;
				case "health":
					WriteHealth();
					break;
				case "start_test":
					if (RobotMain.IsRunning == false) {
						RobotMain.Start();
					}
					RobotMain.State = RobotMain.RobotState.TestInit;
					break;
				case "start_idle":
					if (RobotMain.IsRunning == false) {
						RobotMain.Start();
					}
					RobotMain.State = RobotMain.RobotState.IdleInit;
					break;
				case "start_autonomous":
					if (RobotMain.IsRunning == false) {
						RobotMain.Start();
					}
					RobotMain.State = RobotMain.RobotState.AutonomousInit;
					break;
				case "start_teleop":
					if (RobotMain.IsRunning == false) {
						RobotMain.Start();
					}
					RobotMain.State = RobotMain.RobotState.TeleopInit;
					break;
				case "setchnnl":
					WriteSetChnnl(Convert.ToByte(argStrings[0]));
					break;
				case "start":
					WriteStart();
					if (RobotMain.IsRunning == false) {
						RobotMain.Start();
					}
					break;
				case "stop":
					WriteStop();
					if (RobotMain.IsRunning == true) {
						RobotMain.Stop();
					}
					break;

				case "print":
					string msg = string.Empty;
					for (int i = 0; i < argStrings.Length; i++) {
						if (i == 0) {
							msg += $"{argStrings[i]}";
						}
						else {
							msg += $" {argStrings[i]}";
						}
					}

					WritePrint(msg);
					break;
				case "ping":
					WritePing();
					break;
				case "setbchnnl":
					WriteSetBChnnl(Convert.ToByte(argStrings[0]));
					break;
				case "drive":
					WriteDrive(Convert.ToByte(argStrings[0]), Convert.ToByte(argStrings[1]));
					break;
				case "drivetimed":
					WriteDriveTimed(Convert.ToByte(argStrings[0]), Convert.ToByte(argStrings[1]), Convert.ToInt16(argStrings[2]));
					break;
				case "tdrive":
					WriteTDrive(Convert.ToByte(argStrings[0]), Convert.ToByte(argStrings[1]));
					break;
				case "tdrivetimed":
					WriteTDriveTimed(Convert.ToByte(argStrings[0]), Convert.ToByte(argStrings[1]), Convert.ToInt16(argStrings[2]));
					break;
				case "claw":
					WriteClaw(Convert.ToByte(argStrings[0]));
					break;

				default:
					WriteBlank();
					break;
			}

			return true;
		}

		public void WriteBlank() {
			byte[] buffer = new byte[1];
			buffer[0] = (byte)RobotMain.MsgIds.Blank;
			// buffer[1] = 0;

			Write(buffer);
		}

		public void WriteHelp() {
			byte[] buffer = new byte[1];
			buffer[0] = (byte)RobotMain.MsgIds.Help;

			Write(buffer);
		}

		public void WriteHealth() {
			byte[] buffer = new byte[1];
			buffer[0] = (byte)RobotMain.MsgIds.Health;

			Write(buffer);
		}

		public void WritePrint(string message) {

			byte[] ascii = Encoding.ASCII.GetBytes(message);
			byte[] buffer = new byte[1 + ascii.Length];

			for (int i = ascii.Length - 1; i >= 0; i--) {
				buffer[i + 1] = ascii[i];
			}

			buffer[0] = (byte)RobotMain.MsgIds.Print;
			buffer[buffer.Length - 1] = (byte)'\0';

			Write(buffer);
		}

		public void WritePing() {
			byte[] buffer = new byte[1];
			buffer[0] = (byte)RobotMain.MsgIds.Ping;

			Write(buffer);
		}

		public void WriteStart() {
			byte[] buffer = new byte[1];
			buffer[0] = (byte)RobotMain.MsgIds.Start;

			Write(buffer);
		}

		public void WriteStop() {
			byte[] buffer = new byte[1];
			buffer[0] = (byte)RobotMain.MsgIds.Stop;

			Write(buffer);
		}

		public void WriteSetChnnl(byte channel) {
			byte[] buffer = new byte[2];
			buffer[0] = (byte)RobotMain.MsgIds.Setchnnl;
			buffer[1] = channel;

			Write(buffer);
		}

		public void WriteSetBChnnl(byte channel) {
			byte[] buffer = new byte[2];
			buffer[0] = (byte)RobotMain.MsgIds.Setbchnnl;
			buffer[1] = channel;

			Write(buffer);
		}

		public void WriteDrive(byte speed, byte turn) {
			byte[] buffer = new byte[3];
			buffer[0] = (byte)RobotMain.MsgIds.Drive;
			buffer[1] = (byte)speed;
			buffer[2] = (byte)turn;

			Write(buffer);
		}

		public void WriteTDrive(byte right, byte left) {
			byte[] buffer = new byte[3];
			buffer[0] = (byte)RobotMain.MsgIds.TDrive;
			buffer[1] = (byte)right;
			buffer[2] = (byte)left;

			Program.MainWindow.PrintLine($">>>>>drive({buffer[1]},{buffer[2]})");

			Write(buffer);
		}

		public void WriteDriveTimed(byte speed, byte turn, short time) {
			byte[] buffer = new byte[5];
			buffer[0] = (byte)RobotMain.MsgIds.Drivetimed;
			buffer[1] = (byte)speed;
			buffer[2] = (byte)turn;
			byte[] timeBytes = BitConverter.GetBytes(time);
			buffer[3] = timeBytes[0];
			buffer[4] = timeBytes[1];

			Write(buffer);
		}

		public void WriteTDriveTimed(byte right, byte left, short time) {
			byte[] buffer = new byte[5];
			buffer[0] = (byte)RobotMain.MsgIds.TDrivetimed;
			buffer[1] = (byte)right;
			buffer[2] = (byte)left;
			byte[] timeBytes = BitConverter.GetBytes(time);
			buffer[3] = timeBytes[0];
			buffer[4] = timeBytes[1];

			Write(buffer);
		}

		public void WriteClaw(byte angle) {
			byte[] buffer = new byte[2];
			buffer[0] = (byte)RobotMain.MsgIds.Claw;
			buffer[1] = (byte)angle;

			Write(buffer);
		}

	}
}
