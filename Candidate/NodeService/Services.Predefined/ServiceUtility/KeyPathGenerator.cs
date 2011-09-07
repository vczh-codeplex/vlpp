using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NodeService;

namespace ServiceUtility
{
    public static class KeyPathGenerator
    {
        private static char[] HexTable = "0123456789ABCDEF".ToCharArray();

        public static string GenerateKey(params string[] keyPath)
        {
            byte[][] bytes = keyPath.Select(s => s.NodeServiceEncode()).ToArray();
            int totalLength = bytes.Sum(b => b.Length) * 2 + bytes.Length + 1;
            char[] chars = new char[totalLength];
            chars[0] = '_';
            int currentPosition = 1;
            foreach (var fragment in bytes)
            {
                for (int i = 0; i < fragment.Length; i++)
                {
                    byte b = fragment[i];
                    chars[currentPosition + i * 2] = HexTable[b / 16];
                    chars[currentPosition + i * 2 + 1] = HexTable[b % 16];
                }
                chars[currentPosition + bytes.Length * 2] = '_';
                int nextPosition = currentPosition + fragment.Length * 2 + 1;
            }
            return new string(chars);
        }
    }
}
