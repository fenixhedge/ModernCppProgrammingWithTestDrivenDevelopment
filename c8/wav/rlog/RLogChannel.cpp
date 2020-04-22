#include <rlog/RLogChannel.h>

namespace rlog
{
    RLogChannel::RLogChannel( const std::string &name, LogLevel level ) {}
    RLogChannel::~RLogChannel() {}
    void RLogChannel::publish(const RLogData &data) {}

    const std::string nameReturn{""};
    const std::string &RLogChannel::name() const { return nameReturn; }

    LogLevel RLogChannel::logLevel() const { return LogLevel{}; }
    void RLogChannel::setLogLevel(LogLevel level) {}
    RLogChannel *RLogChannel::getComponent(RLogChannel *componentParent,
                                           const char *component) { return nullptr; }
}
