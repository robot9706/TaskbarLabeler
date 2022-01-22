using System;
using System.Runtime.InteropServices;
using System.Text;

namespace TaskbarLabel
{
    static class Native
    {
        public static int WM_COPYDATA = 0x004A;

        [DllImport("native", EntryPoint = "native_init")]
        public static extern int Init(IntPtr ownerHandle);

        [DllImport("native", EntryPoint = "native_exit")]
        public static extern bool Exit();

        [DllImport("native", EntryPoint = "native_is_private_message")]
        public static extern bool IsPrivateMessage(int message);

        [DllImport("native", EntryPoint = "native_set_showpids")]
        public static extern void SetShowPIDs(bool showPIDs);

        [DllImport("native", EntryPoint = "native_add_label", CharSet = CharSet.Unicode)]
        public static extern bool AddLabel(int pid, int color, [MarshalAsAttribute(UnmanagedType.LPWStr)] StringBuilder text);

        [DllImport("native", EntryPoint = "native_remove_label")]
        public static extern bool RemoveLabel(int pid);

        [DllImport("native", EntryPoint = "native_get_button_pids")]
        public static extern bool GetButtonPIDs(IntPtr lParam, int[] array, ref int count);

        [DllImport("native", EntryPoint = "native_get_window_title")]
        public static extern int GetWindowTitleByPID(int pid, [MarshalAsAttribute(UnmanagedType.LPWStr)] StringBuilder title, int maxLength);
    }
}
