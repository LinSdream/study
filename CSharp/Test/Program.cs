using CommonModule.Logger;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Text.Unicode;
using System.Text.Encodings;
using System.Text;
public static class Program
{
    public static ILogger Logger = LogFactory.CreateLogger(LogFactory.ECreateLoggerType.CONSOLE);

    public class Obs
    {
        public Action<int> OnValueChanged;

        private int _val;

        public int Val
        {
            get
            {
                return _val;
            }
            set
            {
                _val = value;
                OnValueChanged?.Invoke(_val);
            }
        }

        public struct Auto : IDisposable
        {
            private Obs _o;
            private Action<int> _onValueChanged;
            public Auto(Obs o, Action<int> call)
            {
                _o = o;
                _onValueChanged = call;
                _o.OnValueChanged += _onValueChanged;
            }

            public void Dispose()
            {
                _o.OnValueChanged -= _onValueChanged;
            }
        }
    }

    public static int Main(params string[] args)
    {
        Socket c = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        try
        {
            c.Connect("127.0.0.1", 23548);
            string fuck = "update_config";
            List<byte> data = new List<byte>();

            var list = BitConverter.GetBytes(fuck.Length);
            if (BitConverter.IsLittleEndian)
            {
                data.AddRange(list.Reverse());
            }
            else
            {
                data.AddRange(list);
            }

            data.AddRange(Encoding.UTF8.GetBytes(fuck));
            c.Send(data.ToArray());

            byte[] resp = new byte[1024];
            c.Receive(resp);
            var res  = Encoding.UTF8.GetString(resp);
            Logger.Info(res);
            return 0;
        }
        finally
        {
            Logger.Info("close socket");
            c.Close();
        }
        return 0;
    }
}