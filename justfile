default:
	@just --list

## C ##

build *FLAGS:
	cd ./Implementierung && make {{FLAGS}}

run *FLAGS:
	cd ./Implementierung && make && ./main {{FLAGS}}


## RamDISK ##
ramdisk:
	diskutil erasevolume HFS+ 'RAM Disk' `hdiutil attach -nobrowse -nomount ram://1048576`
	rm -rf ./Beispiele
	ln -s /Volumes/RAM\ Disk ./Beispiele

ramdisk-stop:
	hdiutil detach /dev/disk4
	rm -f ./Beispiele

## Python ##

venv:
	[ -d venv ] || (python3 -m venv venv && ./venv/bin/pip install -r ./requirements.txt)

reset-venv:
	rm -rf ./venv
	just venv

generate *FLAGS: venv
	./venv/bin/python ./Skripte/generator.py {{FLAGS}}

test *FLAGS: venv
	./venv/bin/python ./Skripte/tester.py {{FLAGS}}