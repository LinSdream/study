using System;

namespace Demo.Effect
{
    public abstract class BaseEffect
    {
        protected bool _isActive;
        public bool Active => _isActive;

        public Action playCompletedCallback;
        public virtual void Init() { }
        public virtual void Update(float deltaTime) { }
        public virtual void Destroy() { }

        public abstract void Play<T>(T args);

        public abstract void Pause();

        public abstract void Stop();
    }
}
