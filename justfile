default:
	@just --list

## C ##

build *FLAGS:
	cd ./Implementierung && make

run *FLAGS:
	cd ./Implementierung && make && ./main


## Python ##

venv:
	[ -d venv ] || (python3 -m venv venv && ./venv/bin/pip install -r ./requirements.txt)

generate *FLAGS: venv
	./venv/bin/python ./Implementierung/generator.py {{FLAGS}}
