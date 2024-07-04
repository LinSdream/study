namespace CommonModule.Logger.Impl.Formatter
{
    internal class DefaultStandardFormatter : ILogFormatter
    {
        public string Format(LoggerInfo loggingInfo)
        {
            string pidStr = loggingInfo.ProcessID.ToString().PadLeft(5, ' ');
            string tidStr = loggingInfo.ThreadID.ToString().PadLeft(2, ' ');
            string levelStr = loggingInfo.Level.ToString().Remove(1);

            string pattern = string.IsNullOrEmpty(loggingInfo.Tag)
                ? "| {0:yyyy-MM-dd HH:mm:ss:fff} | PID: {1}, TID: {2} | [{3}] #"
                : "| {0:yyyy-MM-dd HH:mm:ss:fff} | PID: {1}, TID: {2} | [{3}] # {4} #";

            string prefix = string.Format(pattern, loggingInfo.Time, pidStr, tidStr, levelStr, loggingInfo.Tag);
            string msg = loggingInfo.Message.Replace("\n", $"\n{prefix}");
            return string.Format("{0} {1}", prefix, msg);
        }
    }
}
