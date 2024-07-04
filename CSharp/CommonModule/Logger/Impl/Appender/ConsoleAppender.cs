using CommonModule.Logger.Impl.Formatter;
using System;

namespace CommonModule.Logger.Impl.Appender
{
    internal class ConsoleAppender : ILogAppender
    {
        ILogFormatter _formatter = new DefaultStandardFormatter();

        public bool Enable { get; set; } = true;

        public void Append(LoggerInfo loggerInfo)
        {
            if (!Enable) return;
            string msg = _formatter.Format(loggerInfo);
            ConsoleColor color = Console.ForegroundColor;
            switch (loggerInfo.Level)
            {
                case ELogLevel.DEBUG:
                    Console.ForegroundColor = ConsoleColor.DarkGray;
                    Console.WriteLine(msg);
                    break;

                case ELogLevel.INFO:
                    Console.ForegroundColor = ConsoleColor.DarkCyan;
                    Console.WriteLine(msg);
                    break;

                case ELogLevel.WARN:
                    Console.ForegroundColor = ConsoleColor.DarkYellow;
                    Console.WriteLine(msg);
                    break;

                case ELogLevel.ERROR:
                case ELogLevel.FATAL:
                    Console.ForegroundColor = ConsoleColor.DarkRed;
                    Console.Error.WriteLine(msg);
                    break;
            }
            Console.ForegroundColor = color;

        }

        public void SetFormatter(ILogFormatter formatter)
        {
            _formatter = formatter;
        }
    }
}
