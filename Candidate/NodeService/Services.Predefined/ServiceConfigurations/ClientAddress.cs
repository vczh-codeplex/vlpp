using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Net;
using System.Net.Sockets;

namespace ServiceConfigurations
{
    public static class ClientAddress
    {
        private static Regex ipAddress = new Regex(@"^\d+\.\d+\.\d+\.\d+$", RegexOptions.Singleline);

        public static string BuildHttpAddress(string address, string host)
        {
            return address.Replace("+", host);
        }

        public static string BuildTcpAddress(string address, string host)
        {
            if (!ipAddress.Match(host).Success)
            {
                foreach (var ip in Dns.GetHostAddresses(host))
                {
                    if (ip.AddressFamily == AddressFamily.InterNetwork)
                    {
                        return ip.ToString() + ":" + address;
                    }
                }
            }
            return host + ":" + address;
        }

        public static string BuildNamedPipeAddress(string address, string host)
        {
            return host + "/" + address;
        }
    }
}
