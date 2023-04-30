#pragma once

using namespace System;
using namespace System::Collections::Generic;

public ref class LogicalProcessor
{
public:
    property int ProcessorNumber;
    property int NumaNodeNumber;
};

public ref class LogicalProcessorInfo
{
public:
    static List<LogicalProcessor^>^ GetLogicalProcessors();
};
