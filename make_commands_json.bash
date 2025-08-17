#!/usr/bin/env bash

set -eu

cat Debug/compile_commands.json | sed "s|-I../|-I|g" | sed "s|-fcyclomatic-complexity||g" > ./compile_commands.json
