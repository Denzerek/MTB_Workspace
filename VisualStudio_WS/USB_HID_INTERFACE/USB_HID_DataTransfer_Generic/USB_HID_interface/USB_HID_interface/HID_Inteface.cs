using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using CyUSB;

namespace USB_HID_interface
{
    public partial class HID_Inteface : Form
    {


        USBDeviceList usbDevices = null;
        CyHidDevice myHidDevice = null;
        int VID = 0x04B4;
        int PID = 0x1132;
        int Vendor_ID = 0x04B4;
        int Product_ID = 0x1132;


        public void GetDevice()
        {
            //Look for device matching VID/PID
            myHidDevice = usbDevices[Vendor_ID, Product_ID]
            as CyHidDevice;
            if (myHidDevice != null)
            {
                InputTimer.Enabled = true;
                Status.Text = "Connected";
                Status.ForeColor = Color.Green;

            }
            else
            {
                Status.Text = "Disconnected";
                Status.ForeColor = Color.Red;
                InputTimer.Enabled = false;
            }
        }

        //Handler for Device Attach
        void usbDevices_DeviceAttached(object sender, EventArgs e)
        {
            if (myHidDevice == null)
            {
                GetDevice(); // Process device status
            }
        }

        //Handler for Device Removal
        void usbDevices_DeviceRemoved(object sender, EventArgs e)
        {
            USBEventArgs usbEvent = e as USBEventArgs;
            // Check that it was the expected device that was removed
            if ((usbEvent.ProductID == PID) && (usbEvent.VendorID == VID))
            {
                // Disable interrupts for polling HID
                InputTimer.Enabled = false;
                // Set HID pointer to NULL
                myHidDevice = null;
                // Process device status
                GetDevice();
            }
        }



        private void textBox2_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                Console.WriteLine("Enter pressed on Textbox 2\r\n");
                if (textBox2.Text != null && int.TryParse(textBox2.Text, out int value))
                {
                    myHidDevice.Outputs.DataBuf[1] = (byte)value;
                    Console.WriteLine("Writing to output endpoint of 2\r\n");

                    myHidDevice.WriteOutput();
                }
            }
        }
        private void textBox3_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                Console.WriteLine("Enter pressed on Textbox 3\r\n");
                if (textBox3.Text != null && int.TryParse(textBox3.Text, out int value))
                {
                    myHidDevice.Outputs.DataBuf[2] = (byte)value;
                    Console.WriteLine("Writing to output endpoint of 3\r\n");

                    myHidDevice.WriteOutput();

                }
            }
        }
        private void textBox4_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                Console.WriteLine("Enter pressed on Textbox 4\r\n");
                if (textBox4.Text != null && int.TryParse(textBox4.Text, out int value))
                {
                    myHidDevice.Outputs.DataBuf[3] = (byte)value;
                    Console.WriteLine("Writing to output endpoint of 4\r\n");

                    myHidDevice.WriteOutput();
                }
            }
        }
        private void textBox5_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                Console.WriteLine("Enter pressed on Textbox 5\r\n");
                if (textBox5.Text != null && int.TryParse(textBox5.Text, out int value))
                {
                    myHidDevice.Outputs.DataBuf[4] = (byte)value;
                    Console.WriteLine("Writing to output endpoint of 5\r\n");

                    myHidDevice.WriteOutput();
                }
            }
        }
        private void textBox6_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                Console.WriteLine("Enter pressed on Textbox 6\r\n");
                if (textBox6.Text != null && int.TryParse(textBox6.Text, out int value))
                {
                    myHidDevice.Outputs.DataBuf[5] = (byte)value;
                    Console.WriteLine("Writing to output endpoint of 6\r\n");

                    myHidDevice.WriteOutput();
                }
            }
        }
        private void textBox7_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                Console.WriteLine("Enter pressed on Textbox 7\r\n");
                if (textBox7.Text != null && int.TryParse(textBox7.Text, out int value))
                {
                    myHidDevice.Outputs.DataBuf[6] = (byte)value;
                    Console.WriteLine("Writing to output endpoint of 7\r\n");

                    myHidDevice.WriteOutput();
                }
            }
        }
        private void textBox8_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                Console.WriteLine("Enter pressed on Textbox 8\r\n");
                if (textBox8.Text != null && int.TryParse(textBox8.Text, out int value))
                {
                    myHidDevice.Outputs.DataBuf[7] = (byte)value;
                    Console.WriteLine("Writing to output endpoint of 8\r\n");

                    myHidDevice.WriteOutput();
                }
            }
        }
        private void textBox9_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                Console.WriteLine("Enter pressed on Textbox 9\r\n");
                if (textBox9.Text != null && int.TryParse(textBox9.Text, out int value))
                {
                    myHidDevice.Outputs.DataBuf[8] = (byte)value;
                    Console.WriteLine("Writing to output endpoint of 9\r\n");

                    myHidDevice.WriteOutput();
                }
            }
        }
        public HID_Inteface()
        {


            Console.WriteLine("\r\n Starting USB HID Interfacer");
            //Initialize the main GUI
            InitializeComponent();


            textBox2.KeyDown += textBox2_KeyDown;
            textBox3.KeyDown += textBox3_KeyDown;
            textBox4.KeyDown += textBox4_KeyDown;
            textBox5.KeyDown += textBox5_KeyDown;
            textBox6.KeyDown += textBox6_KeyDown;
            textBox7.KeyDown += textBox7_KeyDown;
            textBox8.KeyDown += textBox8_KeyDown;
            textBox9.KeyDown += textBox9_KeyDown;
            

            Console.WriteLine("\r\n Creating USB Device List");
            // Create a list of CYUSB devices for this application
            usbDevices = new USBDeviceList(CyConst.DEVICES_HID);

            Console.WriteLine("\r\n Adding handlers");
            //Add event handler for device attachment
            usbDevices.DeviceAttached += new EventHandler(usbDevices_DeviceAttached);

            //Add event handler for device removal
            usbDevices.DeviceRemoved += new EventHandler(usbDevices_DeviceRemoved);


            Console.WriteLine("\r\n Getting Device");
            //Connect to the USB device
            GetDevice();
        }
        
        private void tableLayoutPanel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void Input_Click(object sender, EventArgs e)
        {

        }

        private void InputTimer_Tick_1(object sender, EventArgs e)
        {
            if (myHidDevice != null)
            {
                // Disable timer
                InputTimer.Enabled = false;
                //Query the device for new data
                myHidDevice.ReadInput();

                //Console.WriteLine("\r\nRead byte " + string.Join(" ", myHidDevice.Inputs.DataBuf));


                label1.Text = string.Join(" ", myHidDevice.Inputs.DataBuf[1]);
                label3.Text = string.Join(" ", myHidDevice.Inputs.DataBuf[2]);
                label5.Text = string.Join(" ", myHidDevice.Inputs.DataBuf[3]);
                label8.Text = string.Join(" ", myHidDevice.Inputs.DataBuf[4]);
                label10.Text = string.Join(" ", myHidDevice.Inputs.DataBuf[5]);
                label12.Text = string.Join(" ", myHidDevice.Inputs.DataBuf[6]);
                label13.Text = string.Join(" ", myHidDevice.Inputs.DataBuf[7]);
                label16.Text = string.Join(" ", myHidDevice.Inputs.DataBuf[8]);

                InputTimer.Enabled = true;
            }
        }
    }
}
