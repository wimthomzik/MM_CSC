venv:
	[ -d venv ] || (python3 -m venv venv && ./venv/bin/pip install -r ./requirements.txt)

build *FLAGS:
	(cd ./Implementierung && make)

generate *Flags: venv
	./venv/bin/python ./Implementierung/generator.py {{FLAGS}}
