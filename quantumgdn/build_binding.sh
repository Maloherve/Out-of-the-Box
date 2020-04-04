#!/bin/bash
scons platform=linux generate_bindings=yes -j2 use_custom_api_file=yes custom_api_file=../api.json bits=64
