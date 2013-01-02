@echo off
rmdir /q /s src\single_dir_for_windows_kernel 1>nul 2>nul
mkdir src\single_dir_for_windows_kernel 1>nul 2>nul
copy /y src\lfds601_abstraction\* src\single_dir_for_windows_kernel 1>nul 2>nul
copy /y src\lfds601_freelist\* src\single_dir_for_windows_kernel 1>nul 2>nul
copy /y src\lfds601_queue\* src\single_dir_for_windows_kernel 1>nul 2>nul
copy /y src\lfds601_ringbuffer\* src\single_dir_for_windows_kernel 1>nul 2>nul
copy /y src\lfds601_slist\* src\single_dir_for_windows_kernel 1>nul 2>nul
copy /y src\lfds601_stack\* src\single_dir_for_windows_kernel 1>nul 2>nul
copy /y sources.dynamic src\single_dir_for_windows_kernel\sources 1>nul 2>nul
copy /y src\driver_entry.c src\single_dir_for_windows_kernel 1>nul 2>nul
echo Windows kernel dynamic library build directory structure created.
echo (Note the effects of this batch file are idempotent).

