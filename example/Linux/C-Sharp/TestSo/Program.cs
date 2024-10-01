using System;
using System.Runtime.InteropServices;

namespace OdfValidatorCaller
{
    class Program
    {
        // 使用 DllImport 屬性來聲明非托管方法
        [DllImport("odfvalidator", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr ODFValidator_new();

        [DllImport("odfvalidator", CallingConvention = CallingConvention.Cdecl)]
        public static extern void ODFValidator_delete(IntPtr validator);

        [DllImport("odfvalidator", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr ODFValidator_check(IntPtr validator, string filePath);

        [DllImport("odfvalidator", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr ODFValidator_getJsonResult(IntPtr validator);

        static void Main(string[] args)
        {
            if (args.Length == 0)
            {
                Console.WriteLine("Usage: OdfValidatorCaller <file-path>");
                return;
            }

            string filePath = args[0];

            IntPtr validator = ODFValidator_new();

            try
            {
                // 調用非托管方法，獲取 JSON 結果
                IntPtr jsonResultPtr = ODFValidator_check(validator, filePath);
                string jsonResult = Marshal.PtrToStringAnsi(jsonResultPtr) ?? string.Empty;
                Console.WriteLine($"JSON Result: {jsonResult}");
                Console.WriteLine("Validation completed successfully.");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"An error occurred: {ex.Message}");
            }
            finally
            {
                ODFValidator_delete(validator);
            }
        }
    }
}