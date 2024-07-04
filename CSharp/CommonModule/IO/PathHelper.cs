using CommonModule.Logger;
using System.IO;

namespace CommonModule.IO
{
    public static class PathHelper
    {

        public static bool DeleteFile(ILogger logger, string source)
        {
            if (File.Exists(source))
            {
                FileAttributes attributes = File.GetAttributes(source);
                if ((attributes & FileAttributes.ReadOnly) == FileAttributes.ReadOnly)
                {
                    attributes &= ~FileAttributes.ReadOnly;
                    File.SetAttributes(source, attributes);
                }
                try
                {
                    File.Delete(source);
                    return true;
                }
                catch (System.Exception e)
                {
                    logger.Fatal(e);
                    logger.Error("Fatal Path is :{0}", source);
                    return false;
                }
            }
            return false;
        }

        public static bool DeleteDir(ILogger logger, string source)
        {
            bool success;
            try
            {
                success = DeleteChildDir(logger, source);
                if (!success) return false;
                Directory.Delete(source);
            }
            catch (System.Exception e)
            {
                logger.Fatal(e);
                logger.Error("Fatal Path is :{0}", source);
                success = false;
            }
            return success;
        }

        private static bool DeleteChildDir(ILogger logger, string source)
        {
            bool success = true;
            var paths = Directory.GetFileSystemEntries(source);
            foreach (string path in paths)
            {
                if (Directory.Exists(path))
                {
                    if (!DeleteChildDir(logger, path)) success = false;
                    try
                    {
                        Directory.Delete(path);
                    }
                    catch (System.Exception e)
                    {
                        logger.Fatal(e);
                        logger.Error("Fatal Path is :{0}", source);
                        success = false;
                    }
                }
                else if (File.Exists(path))
                {
                    if (!DeleteFile(logger, path)) success = false;
                }
            }
            return success;
        }

        public static string PathFormat(this string path)
        {
            if (string.IsNullOrEmpty(path)) return path;
            path = path.Replace("\\", "/");
            return path;
        }

        public static string GetUniqueDirectory(string parentDir, string dirName)
        {
            string dirPath;

            int index = 0;
            string tempName = dirName;
            while (true)
            {
                dirPath = Path.Combine(parentDir, tempName);
                if (!Directory.Exists(dirPath)) break;
                tempName = string.Format("{0}_{1:x2}", dirName, index++);
            }
            return dirPath;
        }

        public static string GetUniqueFilePath(string parentDir, string fileName)
        {
            string filePath = Path.Combine(parentDir, fileName);

            int dotIdx = fileName.LastIndexOf('.');
            string prefix = dotIdx > 0 ? fileName.Remove(dotIdx) : fileName;
            string ext = dotIdx > 0 ? fileName.Substring(dotIdx) : string.Empty;

            int index = 0;
            while (File.Exists(filePath))
            {
                fileName = string.Format("{0}_{1}{2}", prefix, index++, ext);
                filePath = Path.Combine(parentDir, fileName);
            }
            return filePath;
        }

        public static string GetUniqueFilePath(string filePath)
        {
            filePath = filePath.Replace('\\', '/');
            int index = filePath.LastIndexOf('/');
            string parentDir = filePath.Remove(index);
            string fileName = filePath.Substring(index + 1);
            return GetUniqueFilePath(parentDir, fileName);
        }
    }
}
