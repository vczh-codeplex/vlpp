using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ImagePattern
{
    public static class ImageDataAlgorithms
    {
        public static ImageData Copy(this ImageData imageData)
        {
            return new ImageData(imageData);
        }

        public static unsafe void Reverse(this ImageData imageData)
        {
            fixed (byte* buffer = imageData.RawData)
            {
                for (int i = 0; i < imageData.RawData.Length; i++)
                {
                    buffer[i] = (byte)(255 - buffer[i]);
                }
            }
        }

        public static unsafe ImageData DifferentAbsAndCopy(this ImageData imageData)
        {
            ImageData copy = imageData.Copy();
            fixed (byte* read = imageData.RawData)
            fixed (byte* write = copy.RawData)
            {
                for (int y = 0; y < imageData.Height; y++)
                {
                    for (int x = 0; x < imageData.Width; x++)
                    {
                        for (int z = 0; z < imageData.ColorSize; z++)
                        {
                            int centerColor = read[y * imageData.Stride + x * imageData.ColorSize + z];
                            int different = 0;
                            for (int dy = -1; dy <= 1; dy++)
                            {
                                for (int dx = -1; dx <= 1; dx++)
                                {
                                    int cy = y + dy;
                                    int cx = x + dx;
                                    if (0 <= cy && cy < imageData.Height && 0 <= cx && cx < imageData.Width)
                                    {
                                        int roundColor = read[cy * imageData.Stride + cx * imageData.ColorSize + z];
                                        int roundDifferent = centerColor - roundColor;
                                        if (roundDifferent > different) different = roundDifferent;
                                    }
                                }
                            }

                            write[y * copy.Stride + x * imageData.ColorSize + z] = (byte)different;
                        }
                    }
                }
            }
            return copy;
        }
    }
}
