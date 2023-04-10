#pragma once

#include "Compiler.h"
#include "MemoryBlock.h"
#include "AlifStack.h"

class Interpreter {
public:

	Interpreter(AlifArray<Container*>* _containers, MemoryBlock* _alifMemory);

	void run_code();
};