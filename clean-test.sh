#!/bin/bash
echo "[INFO] Clean testing Cameleon ..."
cd deliverable/instance/project/Tests
find . -name "*.tmp*" -exec rm -rf {} \;
find . -name "*.bck*" -exec rm -rf {} \;
find . -name "cameleon.log" -exec rm -rf {} \;

