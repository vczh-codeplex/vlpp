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

        public static unsafe void Alpha(this ImageData image1, ImageData image2, double image1Alpha)
        {
            if (image1.Width != image2.Width || image1.Height != image2.Height || image1.ColorSize != image2.ColorSize)
            {
                throw new InvalidOperationException("The formats of two images do not match.");
            }

            fixed (byte* read = image2.RawData)
            fixed (byte* write = image1.RawData)
            {
                double a1 = image1Alpha;
                double a2 = 1 - image1Alpha;
                int length = image1.Stride * image1.Height;
                for (int i = 0; i < length; i++)
                {
                    write[i] = (byte)Math.Round(write[i] * a1 + read[i] * a2);
                }
            }
        }

        public static unsafe void EnlargeColorRange(this ImageData imageData)
        {
            fixed (byte* colors = imageData.RawData)
            {
                int length = imageData.Stride * imageData.Height;
                byte min = byte.MaxValue;
                byte max = byte.MinValue;
                for (int i = 0; i < length; i++)
                {
                    min = Math.Min(min, colors[i]);
                    max = Math.Max(max, colors[i]);
                }

                double scale = (double)byte.MaxValue / (max - min);
                for (int i = 0; i < length; i++)
                {
                    colors[i] = (byte)Math.Round((colors[i] - min) * scale);
                }
            }
        }

        public static unsafe void ReduceColorRange(this ImageData imageData, byte min, byte max)
        {
            fixed (byte* colors = imageData.RawData)
            {
                int length = imageData.Stride * imageData.Height;
                double scale = (double)byte.MaxValue / (max - min);
                for (int i = 0; i < length; i++)
                {
                    byte color = colors[i];
                    if (color <= min)
                    {
                        colors[i] = byte.MinValue;
                    }
                    else if (color >= max)
                    {
                        colors[i] = byte.MaxValue;
                    }
                    else
                    {
                        colors[i] = (byte)Math.Round((color - min) * scale);
                    }
                }
            }
        }

        public static unsafe void Booling(this ImageData imageData, byte border)
        {
            fixed (byte* colors = imageData.RawData)
            {
                int length = imageData.Stride * imageData.Height;
                for (int i = 0; i < length; i++)
                {
                    colors[i] = colors[i] < border ? byte.MinValue : byte.MaxValue;
                }
            }
        }

        public static unsafe ImageData ZipAndCopy(this ImageData imageData)
        {
            if (imageData.SingleByte)
            {
                throw new InvalidOperationException("Source is already zipped.");
            }

            ImageData copy = new ImageData(imageData.Width, imageData.Height, true);
            fixed (byte* read = imageData.RawData)
            fixed (byte* write = copy.RawData)
            {
                for (int y = 0; y < imageData.Height; y++)
                {
                    for (int x = 0; x < imageData.Width; x++)
                    {
                        int sum = 0;
                        for (int z = 0; z < imageData.ColorSize; z++)
                        {
                            sum += read[y * imageData.Stride + x * imageData.ColorSize + z];
                        }
                        write[y * copy.Stride + x] = (byte)Math.Round((double)sum / imageData.ColorSize);
                    }
                }
            }
            return copy;
        }

        public static unsafe ImageData UnzipAndCopy(this ImageData imageData)
        {
            if (!imageData.SingleByte)
            {
                throw new InvalidOperationException("Source is already unzipped.");
            }

            ImageData copy = new ImageData(imageData.Width, imageData.Height, false);
            fixed (byte* read = imageData.RawData)
            fixed (byte* write = copy.RawData)
            {
                int length = imageData.Stride * imageData.Height;
                for (int i = 0; i < length; i++)
                {
                    for (int j = 0; j < copy.ColorSize; j++)
                    {
                        write[i * copy.ColorSize + j] = read[i];
                    }
                }
            }
            return copy;
        }

        public static unsafe ImageData EnlargeColorRangeAndCopy(this ImageData imageData, int radius)
        {
            if (!imageData.SingleByte)
            {
                throw new InvalidOperationException("Source is not zipped.");
            }

            ImageData copy = new ImageData(imageData.Width, imageData.Height, imageData.SingleByte);
            fixed (byte* read = imageData.RawData)
            fixed (byte* write = copy.RawData)
            {
                for (int y = 0; y < imageData.Height; y++)
                {
                    for (int x = 0; x < imageData.Width; x++)
                    {
                        byte min = byte.MaxValue;
                        byte max = byte.MinValue;

                        for (int dy = -radius; dy <= radius; dy++)
                        {
                            int cy = y + dy;
                            if (0 <= cy && cy < imageData.Height)
                            {
                                for (int dx = -radius; dx <= radius; dx++)
                                {
                                    int cx = x + dx;
                                    if (0 <= cx && cx < imageData.Width)
                                    {
                                        byte color = read[cy * imageData.Stride + cx];
                                        min = Math.Min(min, color);
                                        max = Math.Max(max, color);
                                    }
                                }
                            }
                        }

                        if (max != min)
                        {
                            double scale = (double)byte.MaxValue / (max - min);
                            byte readColor = read[y * imageData.Stride + x];
                            write[y * imageData.Stride + x] = (byte)Math.Round((readColor - min) * scale);
                        }
                    }
                }
            }
            return copy;
        }

        public static unsafe ImageData DifferentAbsAndCopy(this ImageData imageData, int radius)
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
                            for (int dy = -radius; dy <= radius; dy++)
                            {
                                for (int dx = -radius; dx <= radius; dx++)
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
