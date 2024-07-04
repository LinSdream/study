using System;
using System.Text;

namespace CommonModule.Logger.Utilities
{
    internal class LoggerHelper
    {
        private static StringBuilder _builder = new StringBuilder();
        internal static string GetExceptionInfo(Exception ex, string prefix, params object[] args)
        {
            lock (_builder)
            {
                if (!string.IsNullOrEmpty(prefix))
                {
                    if (null != args && args.Length > 0)
                        _builder.AppendFormat(prefix, args);
                    else
                        _builder.Append(prefix);
                }

                while (ex != null)
                {
                    _builder.AppendFormat("{0}: {1}", ex.GetType().FullName, ex.Message).AppendLine();
                    _builder.AppendLine(ex.StackTrace);
                    ex = ex.InnerException;
                }
                string exceptionMsg = _builder.ToString();
                _builder.Clear();
                return exceptionMsg;
            }
        }

        internal static LoggerInfo CreateLoggingInfo(string tag, ELogLevel logLevel, string format, params object[] args)
        {
            LoggerInfo loggingInfo = new LoggerInfo();
            loggingInfo.Time = System.DateTime.Now;
            loggingInfo.ProcessID = GetCurrentProcessId();
            loggingInfo.ThreadID = GetCurrentThreadId();
            loggingInfo.Level = logLevel;
            loggingInfo.Tag = tag;
            if (args == null || args.Length <= 0)
                loggingInfo.Message = format;
            else
                loggingInfo.Message = string.Format(format, args);
            return loggingInfo;
        }

        internal static int GetCurrentProcessId()
        {
            try
            {
                return System.Diagnostics.Process.GetCurrentProcess().Id;
            }
            catch (System.Exception)
            {
                return -1;
            }
        }

        internal static int GetCurrentThreadId()
        {
            try
            {
                return System.Threading.Thread.CurrentThread.ManagedThreadId;
            }
            catch (System.Exception)
            {
                return -1;
            }
        }

    }
}
