using CommonModule.Logger.Utilities;
using System;
using System.Collections.Generic;

namespace CommonModule.Logger.Impl
{
    internal class DefaultLogger : BasicLoggerTemplate
    {

        public DefaultLogger(string tag)
        {
            _tag = tag;
            _appenders = new List<ILogAppender>();
        }

        public DefaultLogger() : this(null) { }

        public override void Debug(string format, params object[] args)
        {
            AppendLogger(ELogLevel.DEBUG, format, args);
        }

        public override void Error(string format, params object[] args)
        {
            AppendLogger(ELogLevel.ERROR, format, args);
        }

        public override void Fatal(string format, params object[] args)
        {
            AppendLogger(ELogLevel.FATAL, format, args);
        }

        public override void Fatal(Exception ex)
        {
            string message = LoggerHelper.GetExceptionInfo(ex, null);
            Fatal(message);
        }

        public override void Info(string format, params object[] args)
        {
            AppendLogger(ELogLevel.INFO, format, args);
        }

        public override void Warn(string format, params object[] args)
        {
            AppendLogger(ELogLevel.WARN, format, args);
        }

        protected void AppendLogger(ELogLevel logLevel, string format, params object[] args)
        {
            LoggerInfo loggerInfo = LoggerHelper.CreateLoggingInfo(_tag, logLevel, format, args);
            Append(loggerInfo);
        }

    }
}