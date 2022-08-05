using UnityEngine;

namespace Demo
{
    public class Player
    {
        private GameObject _model;
        private CharacterController _cc;
        private Transform _transform;

        private float _horizontal;
        private float _vertical;
        private Vector3 _direction;

        public float moveSpeed = 2f;

        public float attackRange = 5f;

        public Transform Transform => _transform;

        public Player(GameObject model)
        {
            _model = model;
            _transform = _model.transform;
            _cc = _model.GetComponent<CharacterController>();
        }

        public void Update(float deltaTime)
        {
            _horizontal = Input.GetAxis("Horizontal");
            _vertical = Input.GetAxis("Vertical");

            if (_horizontal == 0 && _vertical == 0)
            {
                return;
            }

            _direction = (_transform.forward * _vertical + _transform.right * _horizontal) * moveSpeed;

            _cc.Move(_direction * deltaTime);
        }

        public void Attack(Transform target)
        {
            
        }

    }
}