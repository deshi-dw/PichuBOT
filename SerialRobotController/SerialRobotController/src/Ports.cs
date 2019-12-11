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
			// Close and dispose of the port.
			// Note that the port object might not have to be created and destroyed every time but whatever.
			port.DiscardInBuffer();
			port.DiscardOutBuffer();
			port.Close();
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
			port.Write($"{message}\n");
		}
	}
}
