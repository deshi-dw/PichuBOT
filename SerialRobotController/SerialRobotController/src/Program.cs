using System;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Windows.Forms;
using Linearstar.Windows.RawInput;

namespace SerialRobotController {
    public class Program {
        public static RawInputWindow InputWindow { get; private set; }
        public static MainWindow MainWindow { get; private set; }

		public static Hids Hids { get; private set; } = new Hids();
		public static Ports Ports { get; private set; } = new Ports();

		public static INIFile INIFile { get; private set; }

		public static LogitechDuelAction Gamepad { get; private set; } = new LogitechDuelAction();

		[STAThread]
        public static void Main (string[] args) {
			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);

			// Setup INI file and create it if nessesary.
			string iniFilePath = $"{Application.StartupPath}\\config.ini";
			if (File.Exists(iniFilePath) == false) {
				File.Create(iniFilePath).Close();
			}
			INIFile = new INIFile(iniFilePath);

			InputWindow = new RawInputWindow();
			// RawInputDevice.RegisterDevice(HidUsageAndPage.TouchPad, RawInputDeviceFlags.ExInputSink, InputWindow.Handle);
			RawInputDevice.RegisterDevice(HidUsageAndPage.Joystick, RawInputDeviceFlags.ExInputSink, InputWindow.Handle);

			MainWindow = new MainWindow();
			Application.Run(MainWindow);

			RawInputDevice.UnregisterDevice(HidUsageAndPage.GamePad);
        }
    }
}
