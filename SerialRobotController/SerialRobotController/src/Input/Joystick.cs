using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SerialRobotController {
	public struct Joystick {
		public byte rawX;
		public byte rawY;
		public bool isPressed;

		public Joystick(byte rawX, byte rawY, bool isPressed) {
			this.rawX = rawX;
			this.rawY = rawY;
			this.isPressed = isPressed;
		}
	}
}
