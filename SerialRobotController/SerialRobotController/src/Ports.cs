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

		public bool Connect(string portName, int baudRate) {
			if (port == null || (port != null && port.IsOpen == false)) {
				try {
					// Create a new port and open it.
					port = new SerialPort(portName, baudRate);
					port.DataReceived += OnDataRecived;
					port.Open();

					// Write a message to the robot saying that it is now connected to the computer.
					byte[] connectMessage = Encoding.ASCII.GetBytes($"Connected to the port {portName}.");
					port.Write(connectMessage, 0, connectMessage.Length);
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

			try {
				port.Close();
			}
			catch { }
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

		public void Write(byte[] bytes) {
			port.Write(bytes, 0, bytes.Length);
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
					buffer = new byte[2];
					id = 0b00000001;
					break;
				case "health":
					buffer = new byte[2];
					id = 0b00000010;
					break;
				case "setchnnl":
					buffer = new byte[3];
					id = 0b00000100;

					buffer[1] = Convert.ToByte(argStrings[0]);
					break;
				case "start":
					buffer = new byte[2];
					id = 0b00000101;
					break;
				case "stop":
					buffer = new byte[2];
					id = 0b00000110;
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

					ascii = Encoding.ASCII.GetBytes(msg);
					buffer = new byte[2 + ascii.Length];
					id = 0b00100000;
					
					for (int i = ascii.Length - 1; i >= 0; i--) {
						buffer[i+1] = ascii[i];
					}
					break;
				case "ping":
					buffer = new byte[2];
					id = 0b00100001;
					break;
				case "setbchnnl":
					buffer = new byte[3];
					id = 0b00100010;

					buffer[1] = (byte)Convert.ToInt16(argStrings[0]);
					break;
				case "drive":
					buffer = new byte[4];
					id = 0b00100011;
					
					buffer[1] = Convert.ToByte(argStrings[0]);
					buffer[2] = Convert.ToByte(argStrings[1]);
					break;
				case "drivetimed":
					buffer = new byte[5];
					id = 0b00100100;
					
					buffer[1] = Convert.ToByte(argStrings[0]);
					buffer[2] = Convert.ToByte(argStrings[1]);
					buffer[3] = Convert.ToByte(argStrings[2]);
					break;
				case "tdrive":
					buffer = new byte[4];
					id = 0b00100101;
					
					buffer[1] = Convert.ToByte(argStrings[0]);
					buffer[2] = Convert.ToByte(argStrings[1]);
					break;
				case "tdrivetimed":
					buffer = new byte[5];
					id = 0b00100110;
					
					buffer[1] = Convert.ToByte(argStrings[0]);
					buffer[2] = Convert.ToByte(argStrings[1]);
					buffer[3] = Convert.ToByte(argStrings[2]);
					break;
				case "claw":
					buffer = new byte[3];
					id = 0b00100111;
					
					buffer[1] = Convert.ToByte(argStrings[0]);
					break;

				default:
					id = 0b00000000;

					ascii = Encoding.ASCII.GetBytes(message);
					buffer = new byte[2 + ascii.Length];
					
					for (int i = ascii.Length - 1; i >= 0; i--) {
						buffer[i + 1] = ascii[i];
					}

					break;
			}

			buffer[0] = id;
			buffer[buffer.Length - 1] = Encoding.ASCII.GetBytes(new char[] { '\n' })[0];

			Write(buffer);

			return true;
		}
	}
}
