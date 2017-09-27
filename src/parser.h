#ifndef PARSER_H
#define PARSER_H
#include "darknet.h"
#include "network.h"

#if defined (__cplusplus)
extern "C" {
#endif

void save_network(network net, char *filename);
void save_weights_double(network net, char *filename);

#if defined (__cplusplus)
}
#endif

#endif
