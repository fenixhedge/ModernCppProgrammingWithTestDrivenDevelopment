#include <rlog/StdioNode.h>

namespace rlog {
    StdioNode::StdioNode(int _fdOut, int flags)
        : RLogNode() {}
    StdioNode::StdioNode(int _fdOut, bool colorize_if_tty)
        : RLogNode(), fdOut( _fdOut) {}
    StdioNode::~StdioNode() {}
    void StdioNode::subscribeTo(RLogNode *node) {}
    void StdioNode::publish(const RLogData &data) {}
}
