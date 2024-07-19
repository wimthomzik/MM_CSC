#!/bin/bash

for v in {0..3}
do
	scp -i ./server_key root@195.201.17.122:/root/perf$v.svg Flammengraphen/v$v.svg
done