#include "client/windows/handler/exception_handler.h"
#include "breakpad_c_warpper.h"

using namespace google_breakpad;

static ExceptionHandler* handler = NULL;

static bool dumpCallback(const wchar_t* dump_path,
    const wchar_t* minidump_id,
    void* context,
    EXCEPTION_POINTERS* exinfo,
    MDRawAssertionInfo* assertion,
    bool succeeded)
{
    printf("Application crash, stack dump path: %ws.dmp\n", dump_path);
    return succeeded;
}

void initBreakpad(void) {
    handler = new ExceptionHandler(L"./",
        NULL,
        dumpCallback,
        NULL,
        ExceptionHandler::HANDLER_ALL);
}