namespace CommonModule.Logger
{
    public interface ILogAppender
    {
        bool Enable { get; set; }

        void SetFormatter(ILogFormatter formatter);

        void Append(LoggerInfo loggingInfo);
    }
}
