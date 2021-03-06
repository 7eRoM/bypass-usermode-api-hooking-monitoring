#include "pch.h"
#include <iostream>

#include <Windows.h>
#include <intrin.h>

typedef long NTSTATUS;


typedef struct _IO_STATUS_BLOCK {
	union {
		NTSTATUS Status;
		PVOID Pointer;
	} DUMMYUNIONNAME;

	ULONG_PTR Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

typedef struct _UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _OBJECT_ATTRIBUTES {
	ULONG           Length;
	HANDLE          RootDirectory;
	PUNICODE_STRING ObjectName;
	ULONG           Attributes;
	PVOID           SecurityDescriptor;
	PVOID           SecurityQualityOfService;
}  OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;


typedef NTSTATUS(__stdcall *_NtCreateFile)(
	PHANDLE FileHandle,
	ACCESS_MASK DesiredAccess,
	POBJECT_ATTRIBUTES ObjectAttributes,
	PIO_STATUS_BLOCK IoStatusBlock,
	PLARGE_INTEGER AllocationSize,
	ULONG FileAttributes,
	ULONG ShareAccess,
	ULONG CreateDisposition,
	ULONG CreateOptions,
	PVOID EaBuffer,
	ULONG EaLength
	);

typedef VOID(__stdcall *_RtlInitUnicodeString)(
	PUNICODE_STRING DestinationString,
	PCWSTR SourceString
	);

#define FILE_CREATE 0x00000002
#define FILE_NON_DIRECTORY_FILE 0x00000040
#define OBJ_CASE_INSENSITIVE 0x00000040L

#define InitializeObjectAttributes( i, o, a, r, s ) {    \
      (i)->Length = sizeof( OBJECT_ATTRIBUTES );         \
      (i)->RootDirectory = r;                            \
      (i)->Attributes = a;                               \
      (i)->ObjectName = o;                               \
      (i)->SecurityDescriptor = s;                       \
      (i)->SecurityQualityOfService = NULL;              \
   }

extern "C" NTSTATUS MyNtCreateFile(PHANDLE FileHandle,
	ACCESS_MASK DesiredAccess,
	POBJECT_ATTRIBUTES ObjectAttributes,
	PIO_STATUS_BLOCK IoStatusBlock,
	PLARGE_INTEGER AllocationSize,
	ULONG FileAttributes,
	ULONG ShareAccess,
	ULONG CreateDisposition,
	ULONG CreateOptions,
	PVOID EaBuffer,
	ULONG EaLength);



int main()
{
	_RtlInitUnicodeString RtlInitUnicodeString = (_RtlInitUnicodeString)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "RtlInitUnicodeString");

	HANDLE hFile;
	OBJECT_ATTRIBUTES objAttribs = { 0 };

#ifdef _WIN64
	PCWSTR filePath = L"\\??\\C:\\test\\Bypass_x64.txt";
#else 
	PCWSTR filePath = L"\\??\\C:\\test\\Bypass_x86.txt";
#endif

	UNICODE_STRING unicodeString;
	RtlInitUnicodeString(&unicodeString, filePath);

	InitializeObjectAttributes(&objAttribs, &unicodeString, OBJ_CASE_INSENSITIVE, NULL, NULL);

	const int allocSize = 2048;
	LARGE_INTEGER largeInteger;
	largeInteger.QuadPart = allocSize;

	IO_STATUS_BLOCK ioStatusBlock = { 0 };
	MyNtCreateFile(&hFile, STANDARD_RIGHTS_ALL, &objAttribs, &ioStatusBlock, &largeInteger,
		FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ, FILE_CREATE, FILE_NON_DIRECTORY_FILE, NULL, NULL);

	if (hFile == NULL) {
		std::cout << "Error #" << GetLastError() << std::endl;
		CloseHandle(hFile);
	}
	else
		std::cout << "Done!" << std::endl;

	std::cin.get();
}

