using System.Collections.Generic;
using UnityEngine;

namespace Demo.Effect
{

    public struct ChainLightningArgs
    {
        public Transform startPoint;
        public Transform endPoint;
    }

    public class ChainLightningEffect : BaseEffect
    {
        private GameObject _instance;
        private LineRenderer _lineRenderer;

        private List<Vector3> _samplingPoints;
        private Transform _startTrans;
        private Transform _endTrans;
        private float _minDivisionNum = 1;
        private float _maxDivisionNum = 15;
        private float _yOffset = 0;

        private float _duration = 1f;
        private float _cumulative = 0f;
        

        public ChainLightningEffect(Material material)
        {
            _instance = new GameObject("ChainLightn_Effect");
            _instance.transform.position = Vector3.zero;

            _samplingPoints = new List<Vector3>();
            _lineRenderer = _instance.AddComponent<LineRenderer>();
            _lineRenderer.material = material;
        }

        public override void Init()
        {
            base.Init();

        }

        public override void Play<T>(T param)
        {
            if (param is ChainLightningArgs args)
            {
                _startTrans = args.startPoint;
                _endTrans = args.endPoint;
                _instance.SetActive(true);
                _isActive = true;
            }
        }

        public override void Pause()
        {
            
        }

        public override void Stop()
        {
            playCompletedCallback?.Invoke();
            playCompletedCallback = null;
            _isActive = false;
            _cumulative = 0f;
            _instance.SetActive(false);
        }

        public override void Update(float deltaTime)
        {
            _cumulative += deltaTime;
            if (_cumulative > _duration)
            {
                playCompletedCallback?.Invoke();
                playCompletedCallback = null;
                _isActive = false;
                _cumulative = 0f;
                _instance.SetActive(false);
            }
            _samplingPoints.Clear();
            Vector3 endPoint = _startTrans.position + Vector3.up * _yOffset;
            Vector3 starPoint = _endTrans.position + Vector3.up * _yOffset;
            MediumDivision(starPoint, endPoint, _maxDivisionNum);
            _samplingPoints.Add(endPoint);

            _lineRenderer.positionCount = _samplingPoints.Count;
            for (int i = 0; i < _samplingPoints.Count; i++)
            {
                _lineRenderer.SetPosition(i, _samplingPoints[i]);
            }
        }

        private void MediumDivision(Vector3 start, Vector3 end, float displace)
        {
            if (displace < _minDivisionNum)
            {
                _samplingPoints.Add(start);
                return;
            }
            Vector3 midPoint = (start + end) / 2;
            midPoint.x += (Random.value - 0.5f);
            midPoint.y += (Random.value - 0.5f);
            midPoint.z += (Random.value - 0.5f);
            MediumDivision(start, midPoint, displace / 2);
            MediumDivision(midPoint, end, displace / 2);
        }
    }
}
