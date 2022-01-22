using System;
using System.Windows.Forms;

namespace TaskbarLabel
{
    static class Program
    {
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            MainForm mainForm = new MainForm();

            if (!InitNative(mainForm.Handle))
            {
                return;
            }

            Application.Run(mainForm);

            Native.Exit();
        }

        static bool InitNative(IntPtr owner)
        {
            int init = Native.Init(owner);

            switch(init)
            {
                case 0: // OK
                    return true;

                case 1:
                    MessageBox.Show("OS version not supported!", "Error", MessageBoxButtons.OK);
                    break;
                case 2:
                    MessageBox.Show("Architecture mismatch!", "Error", MessageBoxButtons.OK);
                    break;

                default:
                    MessageBox.Show("Unknown error!", "Error", MessageBoxButtons.OK);
                    break;
            }

            return false;
        }
    }
}
