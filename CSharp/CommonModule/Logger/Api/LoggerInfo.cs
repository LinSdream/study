namespace CommonModule.Logger
{

    public enum ELogLevel
    {
        DEBUG = 1,
        INFO,
        WARN,
        ERROR,
        FATAL,
    }

    public struct LoggerInfo
    {
        /// <summary> 时间</summary>
        public System.DateTime Time;
        /// <summary> 进程ID </summary>
        public int ProcessID;
        /// <summary> 线程ID </summary>
        public int ThreadID;
        /// <summary> 等级 </summary>
        public ELogLevel Level;
        /// <summary> 消息 </summary>
        public string Message;
        /// <summary> 标签 </summary>
        public string Tag;
    }
}
