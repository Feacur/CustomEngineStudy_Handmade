@echo off
chcp 1252

call project/init_environment_cl.bat

echo Build started at %time%
call project/build_cl.bat
echo Build finished at %time%
