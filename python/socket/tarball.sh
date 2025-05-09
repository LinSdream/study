source $PY_VENV_G/bin/activate

rm -rf ./dist

pyinstaller -F custom_server.py
pyinstaller -F custom_client.py

rm -rf ./__pycache__
rm -rf ./src/__pycache__
rm -rf ./build

rm ./custom_server.spec
mv -f dist/custom_server ./custom_server

rm ./custom_client.spec
mv -f dist/custom_client ./custom_client

rm -rf dist
