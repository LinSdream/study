using UnityEngine;

namespace TestDemo
{
    public class PlayerController : MonoBehaviour
    {
        public Test test;
        public float moveSpeed;
        public int neighborCount = 3;

        private CharacterController _cc;
        private float _horizontal;
        private float _vertical;
        private Vector3 _direction;

        private Transform[] _neighbor;

        private void Start()
        {
            _neighbor = new Transform[neighborCount];
            _cc = GetComponent<CharacterController>();
        }

        private void Update()
        {
            _horizontal = Input.GetAxis("Horizontal");
            _vertical = Input.GetAxis("Vertical");

            if (_horizontal == 0 && _vertical == 0)
            {
                return;
            }

            _direction = (transform.forward * _vertical + transform.right * _horizontal) * moveSpeed;

            _cc.Move(_direction * Time.deltaTime);

            _neighbor = test.Find(transform.position, 3);
            foreach (var tran in _neighbor)
            {
                var mat = tran.GetComponent<MeshRenderer>().material;
                mat.color = Color.red;
                Debug.Log(tran.name);
            }

        }
    }
}