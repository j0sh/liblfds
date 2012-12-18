@echo off
rmdir /q /s src\single_dir_for_windows_kernel 1>nul 2>nul
mkdir src\single_dir_for_windows_kernel 1>nul 2>nul
copy /y src\lfds600_abstraction\* src\single_dir_for_windows_kernel 1>nul 2>nul
copy /y src\lfds600_freelist\* src\single_dir_for_windows_kernel 1>nul 2>nul
copy /y src\lfds600_queue\* src\single_dir_for_windows_kernel 1>nul 2>nul
copy /y src\lfds600_ringbuffer\* src\single_dir_for_windows_kernel 1>nul 2>nul
copy /y src\lfds600_slist\* src\single_dir_for_windows_kernel 1>nul 2>nul
copy /y src\lfds600_stack\* src\single_dir_for_windows_kernel 1>nul 2>nul
copy /y sources.static src\single_dir_for_windows_kernel\sources 1>nul 2>nul
erase /f src\single_dir_for_windows_kernel\driver_entry.c 1>nul 2>nul
echo Windows kernel static library build directory structure created.
echo (Note the effects of this batch file are idempotent).

