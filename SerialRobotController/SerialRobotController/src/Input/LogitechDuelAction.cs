using System;

namespace SerialRobotController {
	public class LogitechDuelAction {
		public Joystick AxisRight { get; private set; } = new Joystick(0, 0, false);
		public Joystick AxisLeft { get; private set; } = new Joystick(0, 0, false);

		public Dpad Dpad { get; private set; }

		public bool Button4 { get; private set; }
		public bool Button3 { get; private set; }
		public bool Button2 { get; private set; }
		public bool Button1 { get; private set; }

		public bool TriggerTopRight { get; private set; }
		public bool TriggerBottomRight { get; private set; }
		public bool TriggerTopLeft { get; private set; }
		public bool TriggerBottomLeft { get; private set; }

		public bool ButtonStart { get; private set; }
		public bool ButtonSelect { get; private set; }

		public LogitechDuelAction() {
		}

		public void Update(byte[] rawData) {
			AxisLeft = new Joystick(rawData[1], rawData[2], (rawData[6] & 0b01000000) == 0b01000000);
			AxisRight = new Joystick(rawData[3], rawData[4], (rawData[6] & 0b10000000) == 0b10000000);

			Button4 = (rawData[5] & 0b10000000) == 0b10000000;
			Button3 = (rawData[5] & 0b01000000) == 0b01000000;
			Button2 = (rawData[5] & 0b00100000) == 0b00100000;
			Button1 = (rawData[5] & 0b00010000) == 0b00010000;

			switch (rawData[5] & 0b00001111) {
				case 0b00000001:
					Dpad = Dpad.UP_RIGHT;
					break;
				case 0b00000010:
					Dpad = Dpad.RIGHT;
					break;
				case 0b00000011:
					Dpad = Dpad.DOWN_RIGHT;
					break;
				case 0b00000100:
					Dpad = Dpad.DOWN;
					break;
				case 0b00000101:
					Dpad = Dpad.DOWN_LEFT;
					break;
				case 0b00000110:
					Dpad = Dpad.LEFT;
					break;
				case 0b00000111:
					Dpad = Dpad.UP_LEFT;
					break;
				case 0b00000000:
					Dpad = Dpad.UP;
					break;
				case 0b00001000:
					Dpad = Dpad.NONE;
					break;
				default:
					Dpad = Dpad.NONE;
					break;
			}

			ButtonStart		= (rawData[6] & 0b00100000) == 0b00100000;
			ButtonSelect	= (rawData[6] & 0b00010000) == 0b00010000;

			TriggerBottomRight	= (rawData[6] & 0b00001000) == 0b00001000;
			TriggerBottomLeft	= (rawData[6] & 0b00000100) == 0b00000100;
			TriggerTopRight		= (rawData[6] & 0b00000010) == 0b00000010;
			TriggerTopLeft		= (rawData[6] & 0b00000001) == 0b00000001;
		}

		public string GetStateInfo() {
			string info = "Controller State:\n";
			info += $"Right Axis (X: {AxisRight.rawX}, Y: {AxisRight.rawY}) isPressed: {AxisRight.isPressed}\n";
			info += $"Left Axis (X: {AxisLeft.rawX}, Y: {AxisLeft.rawY}) isPressed: {AxisLeft.isPressed}\n";
			info += $"Dpad: {Enum.GetName(typeof(Dpad), Dpad)}\n";
			info += $"Buttons: Button1: {Button1}, Button2: {Button2}, Button3: {Button3}, Button4: {Button4}\n";
			info += $"Start: {ButtonStart}, Select: {ButtonSelect}\n";
			info += $"TriggerTopRight: {TriggerTopRight}, TriggerTopLeft: {TriggerTopLeft}, TriggerBottomRight: {TriggerBottomRight}, TriggerBottomLeft: {TriggerBottomLeft}";

			return info;
		}
	}
}
