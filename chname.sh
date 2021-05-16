#!/usr/bin/env bash

for name in $@;do
    mv $name/* $name/$name.json
done
