using System.Runtime.InteropServices;

namespace xiAPI.NET_Capture_images_to_file_dng
{
    internal class dng
    {
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        public unsafe struct XI_DNG_METADATA
        {
            // required parameters
            public int cfa;      // bayer matrix type

            public int maxPixelValue;              // maximum pixel value, important for 16-bit image formats (i.e. 255 for 8-bit formats)

            // optional parameters
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
            public string cameraModelName;      // camera model name, encoded in UTF8

            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
            public string cameraSerialNumber;

            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
            public string cameraUserId;         // camera ID given by the user, encoded in UTF8

            public int autoExposureOn;             // 1: on, 0: off, -1: N/A
            public int autoWhiteBalanceOn;         // 1: on, 0: off, -1: N/A
            public float whiteBalanceR, whiteBalanceG, whiteBalanceB;     // white balance coefficients
            public float lensAperture;
            public float lensFocalLength;
            public int acqTimeYear, acqTimeMonth, acqTimeDay, acqTimeHour, acqTimeMinute, acqTimeSecond;   // acquisition time
        };

        private const string dng_binary_name = "xiapi_dng_store.dll";

        [DllImport(dng_binary_name, EntryPoint = "xidngStore")]
        unsafe public static extern int Store(string filename, xiApi.XI_IMG img, XI_DNG_METADATA metadata);

        [DllImport(dng_binary_name, EntryPoint = "xidngInitMetadataStruct")]
        unsafe public static extern int InitMetadata(ref XI_DNG_METADATA metadata);

        [DllImport(dng_binary_name, EntryPoint = "xidngFillMetadataFromCameraParams")]
        unsafe public static extern int FillMetadata(void* hDevice, ref XI_DNG_METADATA metadata);
    }
}