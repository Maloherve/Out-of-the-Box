#!/bin/bash
scons platform=x11 generate_bindings=yes -j2 use_custom_api_file=yes custom_api_file=../api.json bits=64
