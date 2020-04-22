#include <rlog/rlog.h>

namespace rlog
{
    void RLogInit(int &argc, char **argv) {}

    RLogChannel *GetComponentChannel(const char *component, 
	                             const char *path, 
				     LogLevel level) { return nullptr; }

    RLogChannel *GetGlobalChannel( const char *path,
				   LogLevel level) { return nullptr; }
    PublishLoc::~PublishLoc() {}

    void RLog_Register(PublishLoc *loc, RLogChannel *, 
	               const char *format, ... ) {};

    void rAssertFailed( const char *component, const char *file, 
	                const char *function, int line, 
	                const char *conditionStr ) {};

}
