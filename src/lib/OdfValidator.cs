using System;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;

namespace DetectOdfFormatAPI
{
    public class OdfValidator
    {
        [UnmanagedCallersOnly(EntryPoint = "DetectOdfFile")]
        public static int DetectOdfFile(IntPtr filePathPtr)
        {
            string filePath = Marshal.PtrToStringAnsi(filePathPtr);
            if (filePath == null)
            {
                return -1;
            }
            string jarFileName = "odfvalidator-0.12.0-jar-with-dependencies.jar";
            string tempJarPath = Path.Combine(Path.GetTempPath(), jarFileName);

            // 提取嵌入的 JAR 檔案到臨時目錄
            using (Stream resourceStream = Assembly.GetExecutingAssembly().GetManifestResourceStream($"DetectOdfFormatAPI.Resources.{jarFileName}")
                ?? throw new FileNotFoundException("Embedded resource not found."))
            {
                using (FileStream fileStream = new FileStream(tempJarPath, FileMode.Create, FileAccess.Write))
                {
                    resourceStream.CopyTo(fileStream);
                }
            }

            try
            {
                string odftoolkitPath = tempJarPath;

                ProcessStartInfo processStartInfo = new ProcessStartInfo
                {
                    FileName = "java",
                    Arguments = $"-jar \"{odftoolkitPath}\" \"{filePath}\" -v -e",
                    RedirectStandardOutput = true,
                    RedirectStandardError = true,
                    UseShellExecute = false,
                    CreateNoWindow = true
                };

                using (Process process = new Process { StartInfo = processStartInfo })
                {
                    process.Start();
                    string output = process.StandardOutput.ReadToEnd();
                    string error = process.StandardError.ReadToEnd();
                    process.WaitForExit();

                    Console.WriteLine("Output: " + output);
                    if (!string.IsNullOrEmpty(error))
                    {
                        Console.WriteLine("Error: " + error);
                        return -2;
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("An error occurred: " + ex.Message);
                return -3;
            }
            finally
            {
                // 刪除臨時 JAR 檔案
                if (File.Exists(tempJarPath))
                {
                    File.Delete(tempJarPath);
                }
            }

            return 0;
        }
    }
}
