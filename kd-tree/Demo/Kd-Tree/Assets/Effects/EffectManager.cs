using System;
using System.Collections.Generic;

namespace Demo.Effect
{
    public class EffectManager
    {

        private Dictionary<Type, List<BaseEffect>> _effectsMap;
        private HashSet<BaseEffect> _effects;

        public EffectManager()
        {
            _effectsMap = new Dictionary<Type, List<BaseEffect>>();
            _effects = new HashSet<BaseEffect>();
        }

        public void Update(float deltaTime)
        {
            if (_effectsMap.Count <= 0) return;
            //IEnumerator<BaseEffect> e = _effects.GetEnumerator();
            foreach (BaseEffect effect in _effects)
            {
                if (effect.Active)
                {
                    effect.Update(deltaTime);
                }
            }
        }

        public void CreateEffect<T>(BaseEffect effect)
        {
            Type type = typeof(T);
            if (!_effectsMap.ContainsKey(type))
            {
                _effectsMap[type] = new List<BaseEffect>();
            }
            _effectsMap[type].Add(effect);
            _effects.Add(effect);
        }

        public BaseEffect PlayEffect<T>(T args)
        {
            Type type = typeof(T);
            if (!_effectsMap.ContainsKey(type)) return null;
            var effects = _effectsMap[type];
            if (effects == null || effects.Count == 0) return null;
            foreach (var effect in effects)
            {
                if (!effect.Active)
                {
                    effect.Play(args);
                    return effect;
                }
            }
            return null;
        }

    }
}