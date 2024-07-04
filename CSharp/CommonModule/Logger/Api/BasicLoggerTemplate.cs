using System;
using System.Collections.Generic;

namespace CommonModule.Logger
{
    public abstract class BasicLoggerTemplate : ILogger
    {

        protected string _tag;
        protected IList<ILogAppender> _appenders;

        public abstract void Debug(string format, params object[] args);
        public abstract void Info(string format, params object[] args);
        public abstract void Warn(string format, params object[] args);
        public abstract void Error(string format, params object[] args);
        public abstract void Fatal(string format, params object[] args);

        public abstract void Fatal(Exception ex);

        public virtual void Append(LoggerInfo loggingInfo)
        {
            foreach (var appender in _appenders)
            {
                appender.Append(loggingInfo);
            }
        }

        public virtual void RemoveAppender(ILogAppender appender)
        {
            _appenders.Remove(appender);
        }

        public virtual void SetTag(string tag)
        {
            _tag = tag;
        }


        public void AddAppender(ILogAppender appender)
        {
            _appenders.Add(appender);
        }
    }
}
