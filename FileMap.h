/*
 * @File: FileMap.h
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

#ifndef INCLUDE_FILEMAP_H
#define INCLUDE_FILEMAP_H

#include <stdint.h>
#include <stdbool.h>
#include <windows.h>

bool FileMap_SaveRawBinaryToFile(const char* restrict const filename, const void* restrict data, size_t size);
void* FileMap_CreateFromFile(const char* restrict const filename, size_t* restrict const out_size);

/* Frees the data's filemap from memory
 * Specify a pointer to data contained by a filemap (for example: data created by FileMap_CreateFromFile function)
 */
#define FileMap_Free(_data) UnmapViewOfFile(_data)
#endif
