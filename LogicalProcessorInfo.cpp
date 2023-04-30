#include "LogicalProcessorInfo.h"
#include <Windows.h>

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;

public ref class LogicalProcessor
{
public:
    property int ProcessorNumber;
    property int NumaNodeNumber;
};

public ref class LogicalProcessorInfo
{
    static List<LogicalProcessor^>^ GetLogicalProcessors()
    {
        List<LogicalProcessor^>^ result = gcnew List<LogicalProcessor^>();

        ULONG bufferSize = 0;
        DWORD success = GetLogicalProcessorInformationEx(RelationAll, nullptr, &bufferSize);

        if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
        {
            throw gcnew InvalidOperationException("Failed to retrieve processor information.");
        }

        array<Byte>^ buffer = gcnew array<Byte>(bufferSize);
        pin_ptr<Byte> pBuffer = &buffer[0]; // Pin the buffer's memory
        success = GetLogicalProcessorInformationEx(RelationAll, reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(pBuffer), &bufferSize);

        if (!success)
        {
            throw gcnew InvalidOperationException("Failed to retrieve processor information.");
        }

        PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX pInfo = nullptr;
        ULONG offset = 0;

        while (offset < bufferSize)
        {
            pInfo = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(pBuffer + offset);

            if (pInfo->Relationship == RelationProcessorCore || pInfo->Relationship == RelationProcessorPackage)
            {
                for (size_t i = 0; i < pInfo->Processor.GroupCount; ++i)
                {
                    for (ULONG_PTR processorMask = pInfo->Processor.GroupMask[i].Mask; processorMask; processorMask >>= 1)
                    {
                        if (processorMask & 1)
                        {
                            LogicalProcessor^ logicalProcessor = gcnew LogicalProcessor();
                            logicalProcessor->ProcessorNumber = i;
                            logicalProcessor->NumaNodeNumber = pInfo->Processor.GroupMask[i].Group;
                            result->Add(logicalProcessor);
                        }
                    }
                }
            }

            offset += pInfo->Size;
        }

        return result;
    }

    
};
