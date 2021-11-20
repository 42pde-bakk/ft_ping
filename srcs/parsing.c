//
// Created by Peer De bakker on 11/20/21.
//

#include "ft_ping.h"

void    parse_error(void) {
    const char* usage_msg = "usage: ping [-AaDdfnoQqRrv] [-c count] [-G sweepmaxsize]\n"
                            "            [-g sweepminsize] [-h sweepincrsize] [-i wait]\n"
                            "            [-l preload] [-M mask | time] [-m ttl] [-p pattern]\n"
                            "            [-S src_addr] [-s packetsize] [-t timeout][-W waittime]\n"
                            "            [-z tos] host\n"
                            "       ping [-AaDdfLnoQqRrv] [-c count] [-I iface] [-i wait]\n"
                            "            [-l preload] [-M mask | time] [-m ttl] [-p pattern] [-S src_addr]\n"
                            "            [-s packetsize] [-T ttl] [-t timeout] [-W waittime]\n"
                            "            [-z tos] mcast-group\n"
                            "Apple specific options (to be specified before mcast-group or host like all options)\n"
                            "            -b boundif           # bind the socket to the interface\n"
                            "            -k traffic_class     # set traffic class socket option\n"
                            "            -K net_service_type  # set traffic class socket options\n"
                            "            -apple-connect       # call connect(2) in the socket\n"
                            "            -apple-time          # display current time\n";
    exit_error(usage_msg);
}
