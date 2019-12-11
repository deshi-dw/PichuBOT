using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Linearstar.Windows.RawInput;

namespace SerialRobotController {
	public class Hids {
		public RawInputHid[] ConnectedHids { get; private set; }

		public Hids() { }

		public void UpdateConnectedHids() {
			// ConnectedHids = RawInputDevice.GetDevices().OfType<RawInputHid>().Where((RawInputHid hid) => { return hid.IsConnected; }).ToArray();
			ConnectedHids = RawInputDevice.GetDevices().OfType<RawInputHid>().ToArray();
		}

		public RawInputHid GetHidByName(string name) {
			for (int i = ConnectedHids.Length - 1; i >= 0; i--) {
				if (ConnectedHids[i].ProductName == name) {
					return ConnectedHids[i];
				}
			}

			return null;
		}

		public RawInputHid GetHidById(int id) {
			for (int i = ConnectedHids.Length - 1; i >= 0; i--) {
				if (ConnectedHids[i].ProductId == id) {
					return ConnectedHids[i];
				}
			}

			return null;
		}
	}
}
