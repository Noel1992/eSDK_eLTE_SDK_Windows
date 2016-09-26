#include "sdk_export.h"


#if 0
#ifdef __cplusplus
extern "C"{
#endif
SDK_IMPORT_EXPORT int start_gisproxy();
SDK_IMPORT_EXPORT int stop_gisproxy();
#ifdef __cplusplus
}
#endif
#endif

class SDK_IMPORT_EXPORT Gisproxy_api
{
public:
	static int start_gisproxy();
	static int stop_gisproxy();
};