
#pragma once

#include "typedefs.h"

typedef void(*NetworkConnectionCallback)(u8 ClientID, u8 Connected);
typedef void(*NetworkDataCallback)(u16 ServerData, u16 Player1Data, u16 Player2Data, u16 Player3Data);

void network_init(NetworkConnectionCallback ConnectionCallback, NetworkDataCallback DataCallback);
void network_update();

void network_server_start();
void network_server_stop();

void network_client_start();
void network_client_stop();

void network_send_word(u16 Data);

