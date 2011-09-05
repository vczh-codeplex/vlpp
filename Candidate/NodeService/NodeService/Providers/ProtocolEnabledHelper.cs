using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.IO;

namespace NodeService.Providers
{
    public static class ProtocolEnabledHelper
    {
        private static Regex requestString = new Regex(@"^\[REQUEST\]\[(?<GUID>[a-zA-Z0-9-]+)\]\[(?<METHOD>\w+)\](?<MESSAGE>.*)$", RegexOptions.Singleline);
        private static Regex responseString = new Regex(@"^\[RESPONSE\]\[(?<GUID>[a-zA-Z0-9-]+)\](?<MESSAGE>.*)$", RegexOptions.Singleline);
        private static Regex responseStreamString = new Regex(@"^\[RESPONSESTREAM\]\[(?<GUID>[a-zA-Z0-9-]+)\]$", RegexOptions.Singleline);

        public static string BuildRequest(Guid guid, string method, string message)
        {
            return "[REQUEST][" + guid.ToString() + "][" + method + "]" + message;
        }

        public static string BuildResponse(Guid guid, string message)
        {
            return "[RESPONSE][" + guid.ToString() + "]" + message;
        }

        public static byte[] BuildResponse(Guid guid, byte[] body)
        {
            byte[] header = ("[RESPONSESTREAM][" + guid.ToString() + "]").NodeServiceEncode();
            byte[] message = new byte[header.Length + body.Length];
            Array.Copy(header, message, header.Length);
            Array.Copy(body, 0, message, header.Length, body.Length);
            return message;
        }

        public static bool SplitRequest(string request, out Guid guid, out string method, out string message)
        {
            Match match = requestString.Match(request);
            if (match.Success)
            {
                guid = new Guid(match.Groups["GUID"].Value);
                method = match.Groups["METHOD"].Value;
                message = match.Groups["MESSAGE"].Value;
                return true;
            }
            else
            {
                guid = Guid.Empty;
                method = null;
                message = null;
                return false;
            }
        }

        public static bool SplitResponse(string response, out Guid guid, out string message)
        {
            Match match = responseString.Match(response);
            if (match.Success)
            {
                guid = new Guid(match.Groups["GUID"].Value);
                message = match.Groups["MESSAGE"].Value;
                return true;
            }
            else
            {
                guid = Guid.Empty;
                message = null;
                return false;
            }
        }

        public static bool SplitResponse(string response, out Guid guid)
        {
            Match match = responseString.Match(response);
            if (match.Success)
            {
                guid = new Guid(match.Groups["GUID"].Value);
                return true;
            }
            else
            {
                guid = Guid.Empty;
                return false;
            }
        }
    }
}
