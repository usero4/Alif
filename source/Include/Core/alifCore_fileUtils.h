#pragma once
























enum AlifErrorHandler {
	Alif_Error_Unknown = 0,
	Alif_Error_Strict,
	Alif_Error_SurroGateEscape,
	Alif_Error_Replace,
	Alif_Error_Ignore,
	Alif_Error_BackSlashReplace,
	Alif_Error_SurroGatePass,
	Alif_Error_XMLCharRefReplace,
	Alif_Error_Other
};



ALIFAPI_FUNC(int) alif_decodeLocaleEx(const char*, wchar_t**, size_t*, const char**, int, AlifErrorHandler);




























































































































































































extern void alif_resetForceASCII(void);
