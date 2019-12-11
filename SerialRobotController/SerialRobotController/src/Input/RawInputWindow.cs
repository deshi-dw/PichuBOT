using Linearstar.Windows.RawInput;
using System;
using System.Windows.Forms;

namespace SerialRobotController {
	public class RawInputWindow : NativeWindow {
		public event Action<RawInputData> Input;

		public RawInputWindow() {
			CreateHandle(new CreateParams {
				X = 0,
				Y = 0,
				Width = 0,
				Height = 0,
				Style = 0x01000000,
			});
		}

		protected override void WndProc(ref Message m) {
			const int WM_INPUT = 0x00FF;

			if (m.Msg == WM_INPUT) {
				RawInputData data = RawInputData.FromHandle(m.LParam);

				if (Input != null) {
					Input(data);
				}
			}

			base.WndProc(ref m);
		}
	}
}
