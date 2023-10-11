using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace WebFormsTestApp
{
    public partial class _Default : Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            var p = new Payload { Value = true };
        }
    }

    public struct Payload
    {
        public BlittableBool Value;
    }

    [StructLayout(LayoutKind.Explicit)]
    public struct BlittableBool
    {
        [FieldOffset(0)]
        private byte _value;

        public static implicit operator bool(BlittableBool b)
        {
            return b._value != 0;
        }

        public static implicit operator BlittableBool(bool b)
        {
            return new BlittableBool { _value = b ? (byte)1 : (byte)0 };
        }
    }
}