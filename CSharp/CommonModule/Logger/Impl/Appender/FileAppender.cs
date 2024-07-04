using System.IO;

namespace CommonModule.Logger.Impl.Appender
{
    internal class FileAppender : ILogAppender
    {
        private string _filePath;
        private ILogFormatter _formatter;

        public bool Enable { get; set; } = true;

        public FileAppender(string filePath)
        {
            _filePath = filePath;
            _formatter = new Formatter.DefaultStandardFormatter();
        }

        public FileAppender(string filePath, ILogFormatter formatter) : this(filePath)
        {
            _formatter = formatter;
        }

        public void Append(LoggerInfo loggingInfo)
        {
            lock (_formatter)
            {
                if (!Enable) return;

                string msg = _formatter.Format(loggingInfo);
                byte[] bytes = System.Text.Encoding.UTF8.GetBytes(msg + '\n');

                using (FileStream fs = new FileStream(_filePath, FileMode.OpenOrCreate, FileAccess.ReadWrite))
                {
                    fs.Seek(0, SeekOrigin.End);
                    fs.Write(bytes, 0, bytes.Length);
                    fs.Flush();
                    fs.Close();
                }
            }
        }

        public void SetFormatter(ILogFormatter formatter)
        {
            _formatter = formatter;
        }
    }
}
