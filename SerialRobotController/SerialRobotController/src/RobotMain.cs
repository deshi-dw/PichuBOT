using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace SerialRobotController {
	public static class RobotMain {
		public enum MsgIds : byte {
			Blank = 0b00000000,

			Help = 0b00000100,
			Health = 0b00000101,

			Print = 0b00001001,
			Ping = 0b00001010,
			Start = 0b00001011,
			Stop = 0b00001100,

			Setchnnl = 0b00010001,
			Setbchnnl = 0b00010010,

			Drive = 0b00100001,
			TDrive = 0b00100010,
			Drivetimed = 0b00100011,
			TDrivetimed = 0b00100100,
			Claw = 0b00100101
		}

		public enum RobotState {
			TestInit,
			TestLoop,
			IdleInit,
			IdleLoop,
			AutonomousInit,
			AutonomousLoop,
			TeleopInit,
			TeleopLoop,
			Disabled
		}

		public static Thread LoopThread { get; private set; }
		public static bool IsRunning { get; set; }
		public static RobotState State { get; set; }

		private static Stopwatch stopwatch = new Stopwatch();

		public static void Start() {
			stopwatch.Start();
			LoopThread = new Thread(MainLoop);

			IsRunning = true;
			State = RobotState.Disabled;

			LoopThread.Start();
		}
		public static void Stop() {
			IsRunning = false;
			stopwatch.Stop();
			stopwatch.Reset();
		}

		private static void MainLoop() {
			while (IsRunning == true) {
				while (stopwatch.ElapsedMilliseconds < 50 && IsRunning) ;
				stopwatch.Restart();

				switch (State) {
					case RobotState.TestInit:
						TestInit();
						State = RobotState.TestLoop;
						break;
					case RobotState.TestLoop:
						TestLoop();
						break;
					case RobotState.IdleInit:
						IdleInit();
						State = RobotState.IdleLoop;
						break;
					case RobotState.IdleLoop:
						IdleLoop();
						break;
					case RobotState.AutonomousInit:
						AutonomousInit();
						State = RobotState.AutonomousLoop;
						break;
					case RobotState.AutonomousLoop:
						AutonomousLoop();
						break;
					case RobotState.TeleopInit:
						TeleopInit();
						State = RobotState.TeleopLoop;
						break;
					case RobotState.TeleopLoop:
						TeleopLoop();
						break;
					case RobotState.Disabled:
						DisabledLoop();
						break;
					default:
						break;
				}
			}
		}

		private static void TestInit() {
		}
		private static void TestLoop() {
			Program.Ports.WriteDrive(Math.Max(Math.Min(Program.Gamepad.AxisLeft.rawY, (byte)254), (byte)1), Math.Max(Math.Min(Program.Gamepad.AxisLeft.rawX, (byte)254), (byte)1));
			Program.MainWindow.PrintLine($"x: {Program.Gamepad.AxisLeft.rawX} y: {Program.Gamepad.AxisLeft.rawY}");
		}

		private static void IdleInit() {
			Program.Ports.WriteDrive(Math.Max(Math.Min(Program.Gamepad.AxisLeft.rawY, (byte)254), (byte)1), Math.Max(Math.Min(Program.Gamepad.AxisLeft.rawX, (byte)254), (byte)1));
		}
		private static void IdleLoop() { }

		private static void AutonomousInit() { }
		private static void AutonomousLoop() { }

		private static void TeleopInit() { }
		private static void TeleopLoop() { }
		
		private static void DisabledLoop() { }
	}
}
