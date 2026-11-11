#ifndef _RPC_CLIENT_H
#define _RPC_CLIENT_H

int rpc_led_control(int on);


int rpc_dht11_read(char *humi, char *temp);

/* 连接RPC Server
 * 返回值: (>0)socket, (-1)失败
 */
int RPC_Client_Init(void) ;

#endif /* _RPC_CLIENT_H */


