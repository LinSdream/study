namespace CommonModule.Logger
{
    public interface ILogger
    {
        void SetTag(string tag);

        void AddAppender(ILogAppender appender);

        void RemoveAppender(ILogAppender appender);

        void Debug(string format, params object[] args);

        void Info(string format, params object[] args);

        void Warn(string format, params object[] args);

        void Error(string format, params object[] args);

        void Fatal(string format, params object[] args);

        void Fatal(System.Exception ex);

        void Append(LoggerInfo loggingInfo);
    }
}
