using CommonModule.Logger.Impl;
using CommonModule.Logger.Impl.Appender;

namespace CommonModule.Logger
{
    public class LogFactory
    {
        public enum ECreateLoggerType
        {
            DEFAULT,
            FILE,
            CONSOLE,
            FILE_AND_CONSOLE,
        }
        public enum ECreateAppenderType
        {
            FILE,
            CONSOLE,
        }

        public static ILogger CreateLogger(ECreateLoggerType type, string? filePath = null, string? tag = null)
        {
            DefaultLogger logger = new DefaultLogger(tag);
            switch (type)
            {
                case ECreateLoggerType.DEFAULT:
                    break;
                case ECreateLoggerType.FILE:
                    if (filePath == null)
                        throw new System.Exception("Can't Create File Logger.Because file path can't be null");
                    logger.AddAppender(new FileAppender(filePath));
                    break;
                case ECreateLoggerType.CONSOLE:
                    logger.AddAppender(new ConsoleAppender());
                    break;
                case ECreateLoggerType.FILE_AND_CONSOLE:
                    if (filePath == null)
                        throw new System.Exception("Can't Create File Logger.Because file path can't be null");
                    logger.AddAppender(new FileAppender(filePath));
                    logger.AddAppender(new ConsoleAppender());
                    break;
                default:
                    break;
            }
            return logger;
        }

        public static ILogFormatter CreateDefaultStandardFormatter()
        {
            return new Impl.Formatter.DefaultStandardFormatter();
        }

        public static ILogAppender CreateDefaultAppender(ECreateAppenderType type, string? path = null)
        {
            switch (type)
            {
                case ECreateAppenderType.FILE:
                    if (path == null) throw new System.Exception("Can't Create File Appender,Because file path can't be null");
                    return new FileAppender(path);
                case ECreateAppenderType.CONSOLE:
                    return new ConsoleAppender();
                default:
                    throw new System.Exception("Unknown Error ,Can't Create Appender");
            }
        }
    }
}
