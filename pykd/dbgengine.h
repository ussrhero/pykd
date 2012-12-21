#pragma once

#include "dbgmem.h"
#include "variant.h"

namespace pykd {

// manage debug target
ULONG startProcess( const std::wstring  &processName );
ULONG attachProcess( ULONG pid );
void detachProcess( ULONG processId = -1);
void terminateProcess( ULONG processId = -1);

void loadDump( const std::wstring &fileName );

bool isDumpAnalyzing();
bool isKernelDebugging();

void debugGo();
void debugStep();
void debugStepIn();
void debugBreak();
std::string debugCommand( const std::wstring &command );
ULONG64 evaluate( const std::wstring  &expression );

// debug output
void dprint( const std::wstring &str, bool dml = false );
void dprintln( const std::wstring &str, bool dml = false );
std::string dreadline();
void eprint( const std::wstring &str );
void eprintln( const std::wstring &str );

// system properties
ULONG ptrSize();
bool is64bitSystem();
ULONG getPageSize();

//manage debug module
ULONG64 findModuleBase( const std::string &moduleName );
ULONG64 findModuleBase( ULONG64 offset );
ULONG64 findModuleBySymbol( const std::string &symbolName );
std::string getModuleName( ULONG64 baseOffset );
std::string getModuleImageName( ULONG64 baseOffset ); 
ULONG getModuleSize( ULONG64 baseOffset );
std::string getModuleSymbolFileName( ULONG64 baseOffset );
ULONG getModuleTimeStamp( ULONG64 baseOffset );
ULONG getModuleCheckSum( ULONG64 baseOffset );

// CPU registers
ULONG getRegIndexByName( const std::string &regName );
std::string getRegNameByIndex( ULONG index );
BaseTypeVariant getRegVariantValue( ULONG index );
ULONG64 getRegInstructionPointer();

ULONG64 loadMSR( ULONG  msr );
void setMSR( ULONG msr, ULONG64 value);
std::string getProcessorMode();
std::string getProcessorType();
void setProcessorMode( const std::string &mode );

// Stack and local variables
struct STACK_FRAME_DESC {
    ULONG number;
    ULONG64 instructionOffset;
    ULONG64 returnOffset;
    ULONG64 frameOffset;
    ULONG64 stackOffset;
};

void getStackTrace(std::vector<STACK_FRAME_DESC> &frames);

void getStackTraceWow64(std::vector<STACK_FRAME_DESC> &frames); 

// callback events

enum DEBUG_CALLBACK_RESULT {
    DebugCallbackProceed = 0,
    DebugCallbackNoChange = 1,
    DebugCallbackBreak = 2,
    DebugCallbackMax = 3
};

struct ExceptionInfo {

    bool FirstChance;

    ULONG ExceptionCode; /* NTSTATUS */
    ULONG ExceptionFlags;
    ULONG64 ExceptionRecord;
    ULONG64 ExceptionAddress;

    std::vector<ULONG64> Parameters;

    ExceptionInfo(ULONG FirstChance, const EXCEPTION_RECORD64 &Exception);

    python::list getParameters() const;
    std::string print() const;
};
typedef boost::shared_ptr< ExceptionInfo > ExceptionInfoPtr;

struct DEBUG_EVENT_CALLBACK {

    virtual DEBUG_CALLBACK_RESULT OnBreakpoint( ULONG bpId ) = 0;
    virtual DEBUG_CALLBACK_RESULT OnModuleLoad( ULONG64 offset, const std::string &name ) = 0;
    virtual DEBUG_CALLBACK_RESULT OnModuleUnload( ULONG64 offset, const std::string &name ) = 0;
    virtual DEBUG_CALLBACK_RESULT OnException( ExceptionInfoPtr exceptInfo ) = 0;
};

enum EVENT_TYPE {
    EventTypeBreakpoint             = 0x0001,
    EventTypeException              = 0x0002,
    EventTypeCreateThread           = 0x0004,
    EventTypeExitThread             = 0x0008,
    EventTypeCreateProcess          = 0x0010,
    EventTypeExitProcess            = 0x0020,
    EventTypeLoadModule             = 0x0040,
    EventTypeUnloadModule           = 0x0080,
    EventTypeSystemError            = 0x0100,
    EventTypeSessionStatus          = 0x0200,
    EventTypeChangeDebuggeeState    = 0x0400,
    EventTypeChangeEngineState      = 0x0800,
    EventTypeChangeSymbolState      = 0x1000,

    EventTypeMax,
};

EVENT_TYPE getLastEventType();
ExceptionInfoPtr getLastExceptionInfo();

void eventRegisterCallbacks( const DEBUG_EVENT_CALLBACK *callbacks );
void eventRemoveCallbacks( const DEBUG_EVENT_CALLBACK *callbacks );

//breakpoints
ULONG breakPointSet( ULONG64 offset, bool hardware = false, ULONG size = 0, ULONG accessType = 0 );
void breakPointRemove( ULONG id );
void breakPointRemoveAll();


// processes end threads
ULONG64 getCurrentProcess();
ULONG64 getImplicitThread();
void setCurrentProcess( ULONG64 processAddr );
void setImplicitThread( ULONG64 threadAddr );
void getAllProcessThreads( std::vector<ULONG64> &threadsArray );

// Symbol path
std::string getSymbolPath();
void setSymbolPath(const std::string &symPath);
void appendSymbolPath(const std::string &symPath);

// Extensions
ULONG64 loadExtension(const std::wstring &extPath );
void removeExtension( ULONG64 extHandle );
std::string callExtension( ULONG64 extHandle, const std::wstring command, const std::wstring  &params  );

};

