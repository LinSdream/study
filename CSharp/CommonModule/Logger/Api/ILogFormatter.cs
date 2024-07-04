namespace CommonModule.Logger
{
    public interface ILogFormatter
    {
        string Format(LoggerInfo loggingInfo);
    }
}
