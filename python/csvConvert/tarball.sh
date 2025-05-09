source $PY_VENV_G/bin/activate

rm -rf ./dist
pyinstaller -F main.py

rm -rf ./__pycache__
rm -rf ./src/__pycache__
rm -rf ./build
rm ./main.spec
mv -f dist/main ./csvConvert
rm -rf dist