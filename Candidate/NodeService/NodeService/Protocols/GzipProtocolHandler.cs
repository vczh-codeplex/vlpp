using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.Compression;
using System.IO;

namespace NodeService.Protocols
{
    public class GzipProtocolHandler : ITranslatorProtocolHandlerSimple
    {
        public byte[] Decode(byte[] bytes)
        {
            using (MemoryStream memoryStream = new MemoryStream())
            {
                memoryStream.Write(bytes, 0, bytes.Length);
                memoryStream.Seek(0, SeekOrigin.Begin);
                using (GZipStream gzipStream = new GZipStream(memoryStream, CompressionMode.Decompress, true))
                {
                    byte[] result = gzipStream.ReadAllBytesAndClose();
                    return result;
                }
            }
        }

        public byte[] Encode(byte[] bytes)
        {
            using (MemoryStream memoryStream = new MemoryStream())
            {
                using (GZipStream gzipStream = new GZipStream(memoryStream, CompressionMode.Compress, true))
                {
                    gzipStream.Write(bytes, 0, bytes.Length);
                    gzipStream.Flush();
                    gzipStream.Close();
                }
                memoryStream.Seek(0, SeekOrigin.Begin);
                byte[] result = new byte[memoryStream.Length];
                memoryStream.Read(result, 0, result.Length);
                return result;
            }
        }
    }
}
