using System;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Linq;
using System.Collections;

namespace PJ1000TrackerConfiguration
{
    #region Framing struct
    [StructLayout(LayoutKind.Sequential, Pack = 1, CharSet = CharSet.Ansi)]
    public struct PHONE_NO
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 16)]
        public string phoneNo;
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1, CharSet = CharSet.Ansi)]
    public struct GEO_FENCE
    {
        public byte enableGeoFency;
        [MarshalAs(UnmanagedType.R8)]
        public double lat;
        [MarshalAs(UnmanagedType.R8)]
        public double lon;
        [MarshalAs(UnmanagedType.R4)]
        public float radius;
        public ushort time;
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1, CharSet = CharSet.Ansi)]
    public struct ALARM_PARAM
    {
        public byte month;
        public byte mday;
        public byte hour;
        public byte min;
        public byte value;
        public byte type;
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1, CharSet = CharSet.Ansi)]
    public struct DEVICE_CONFIG_DATA
    {
        public uint size;
        // device info
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 18)]
        public string imei;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
        public string ccid;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
        public string cimi;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 18)]
        public string id;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 16)]
        public string firmwareVersion;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 16)]
        public string pass;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 10)]
        public PHONE_NO[] ownerNumber;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 16)]
        public string bigBoss;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 16)]
        public string password;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 16)]
        public string smsBurner;
        // gprs settings
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 16)]
        public string gprsApn;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 16)]
        public string gprsUser;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 16)]
        public string gprsPass;
        public ushort reportInterval;
        //server ota
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
        public string otaMainServerName;
        public ushort otaMainServerPort;
        //MQTT
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
        public string mqttServerDomain;
        public ushort mqttServerPort;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)]
        public string mqttUsername;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)]
        public string mqttPassword;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
        public string thingName;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
        public string thingOrgranization;
        public ushort feature;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 2048)]
        public string serverCA;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 512)]
        public string backup;
        public uint crc;
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1, CharSet = CharSet.Ansi)]
    public struct DATE_TIME
    {
        public ushort year;   // year with all 4-digit (2011)
        public byte month;   // month 1 - 12 (1 = Jan)
        public byte mday;   // day of month 1 - 31
        public byte wday;   // day of week 1 - 7 (1 = Sunday)
        public byte hour;   // hour 0 - 23
        public byte min;    // min 0 - 59
        public byte sec;    // sec 0 - 59
    }

    #endregion
}
