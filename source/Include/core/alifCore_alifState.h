#pragma once

#include "alifCore_runtime.h"

// سيتم تجهيز هذا الملف ل تهيئة الدوال و وتهيئة ال API الخاص بدوال ملف alifState.c











































































































static inline AlifInterpreterState* alifInterpreterState_get() {
	AlifThreadState* tstate = alifThreadState_get();
#ifdef Py_DEBUG
	alif_ensureTStateNotNull(tstate);
#endif
	return tstate->interp;
}
