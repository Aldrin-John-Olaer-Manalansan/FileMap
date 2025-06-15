/*
 * @File: FileMap.c
 * @Author: Aldrin John O. Manalansan (ajom)
 * @Email: aldrinjohnolaermanalansan@gmail.com
 * @Brief: Create Binary Buffer from File or Save Binary Buffer to File
 * @LastUpdate: June 7, 2025
 * 
 * Copyright (C) 2025  Aldrin John O. Manalansan  <aldrinjohnolaermanalansan@gmail.com>
 * 
 * This Source Code is served under Open-Source AJOM License
 * You should have received a copy of License_OS-AJOM
 * along with this source code. If not, see:
 * <https://raw.githubusercontent.com/Aldrin-John-Olaer-Manalansan/AJOM_License/refs/heads/main/LICENSE_AJOM-OS>
 */

#include "FileMap.h"

/* Saves a raw binary data as a file using filemap
 * data does not need to be inside a filemap, it can be any valid address
 * Returns true if the file was saved sucessfully
 */
bool FileMap_SaveRawBinaryToFile(const char* restrict const filename, const void* restrict data, size_t size) {
    HANDLE hFile = CreateFileA(
        filename,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,               // Overwrite if file exists
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if (hFile == INVALID_HANDLE_VALUE)
        return false;

    HANDLE hMapping = CreateFileMappingA(
        hFile,
        NULL,
        PAGE_READWRITE,
#ifdef _WIN64
        (DWORD)((size >> 32) & 0xFFFFFFFF),   // High DWORD of file size
        (DWORD)(size & 0xFFFFFFFF),           // Low DWORD of file size
#else
        0,    // High DWORD of file size
        size, // Low DWORD of file size
#endif
        NULL
    );
    if (!hMapping) {
        CloseHandle(hFile);
        return false;
    }

    void* mapped = MapViewOfFile(
        hMapping,
        FILE_MAP_ALL_ACCESS,
        0, 0,
        size
    );
    if (!mapped) {
        CloseHandle(hMapping);
        CloseHandle(hFile);
        return false;
    }

    // Copy user data to the mapped region
    memcpy(mapped, data, size);

    // Optional: flush to disk explicitly
    FlushViewOfFile(mapped, size);

    // Cleanup
    UnmapViewOfFile(mapped);
    CloseHandle(hMapping);
    CloseHandle(hFile);

    return true;
}

/* Creates a new FileMap containing the contents(data) of a file
 * Returns the newly created filemap's data
 * out_size will contain the number of bytes occupied by the data
 * NOTICE: The filemap needs to be manually freed after it is not used, else will cause MEMORY LEAK
 * Use FileMap_Free function to free the filemap
 */
void* FileMap_CreateFromFile(const char* restrict const filename, size_t* restrict const out_size) {
    HANDLE file = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE) return NULL;

    DWORD size = GetFileSize(file, NULL);
    HANDLE mapping = CreateFileMappingA(file, NULL, PAGE_READONLY, 0, 0, NULL);
    if (!mapping) {
        CloseHandle(file);
        return NULL;
    }

    void* data = MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, 0);
    CloseHandle(mapping);
    CloseHandle(file);

    *out_size = size;
    return data;
}
