git submodule update --init --recursive
virtualenv env
source ./env/bin/activate

python3 -m pip install -r requirements.txt
