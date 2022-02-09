# Include main definition file: 
SRL_INC_DIR = /home/tie/repo/library/cpp/src
include ${SRL_INC_DIR}/makefile.def

# 
PROGS = tException tLog tTimeInterval  tRegularExpression tValue tTimeInterval tDateTime tErrorMap \
	tSocketAddress tSocket tSocketPool tTcpServer tTcpClient tUdpServer tUdpClient tUnixTcpServer tUnixTcpClient tUnixUdpServer tUnixUdpClient \
	tBufferFrame tBuffer tMysql tFileIni tAppParameter tBufferAnalyzer tMessageBase tSipMessage tNameValuePair tSipHeader tSipStartLine tRecord tXmlXPath tTcpServerMultiThread \
	tThreadWorkerTest
#	
#   tMd5 tMessage tSdp tSipHeader tSip tSipStress tMysql tRadPacket tRadClient tThread tAppParameter tStreamAnalyzer tFileIni
#   tBuffer tAnalyzer

.SUFFIXES: .cpp

.cpp.o: 
	${CC} -g -D _REENTRANT ${CFLAGS} -I/usr/local/include -c $<

# 
all: ${PROGS}

clean: 
	rm -f *.o ${PROGS}

# com: 
tException: tException.o ${SRL_COM_OBJS}
	${LD} tException.o ${SRL_COM_OBJS} -o $@ ${LDFLAGS}

tLog: tLog.o ${SRL_COM_OBJS}
	${LD} tLog.o ${SRL_COM_OBJS} -o $@ ${LDFLAGS}

# typ: 
tRegularExpression: tRegularExpression.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS}
	${LD} tRegularExpression.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS} -o $@ ${LDFLAGS}
tUtility: tUtility.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS}
	${LD} tUtility.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS} -o $@ ${LDFLAGS}
tMd5: tMd5.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS}
	${LD} tMd5.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS} -o $@ ${LDFLAGS}
tValue: tValue.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS}
	${LD} tValue.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS} -o $@ ${LDFLAGS}
tTimeInterval: tTimeInterval.o ${SRL_COM_OBJS}
	${LD} tTimeInterval.o ${SRL_COM_OBJS} -o $@ ${LDFLAGS}
tDateTime: tDateTime.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS}
	${LD} tDateTime.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS} -o $@ ${LDFLAGS}
tErrorMap: tErrorMap.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS}
	${LD} tErrorMap.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS} -o $@ ${LDFLAGS}
tRecord: tRecord.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS}
	${LD} tRecord.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS} -o $@ ${LDFLAGS}
tXmlXPath: tXmlXPath.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS}
	${LD} tXmlXPath.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS} -o $@ ${LDFLAGS}

# net: 
# CSocketAddressInte, CSocketAddressUnix: 
tSocketAddress: tSocketAddress.o ${SRL_COM_OBJS} ${SRL_NET_OBJS}
	${LD} tSocketAddress.o ${SRL_COM_OBJS} ${SRL_NET_OBJS} -o $@ ${LDFLAGS}

# CSocket, CSocketStream, CSocketDatagram: 
tSocket: tSocket.o ${SRL_COM_OBJS} ${SRL_NET_OBJS}
	${LD} tSocket.o ${SRL_COM_OBJS} ${SRL_NET_OBJS} -o $@ ${LDFLAGS}

# 
tUdpServer: tUdpServer.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS} ${SRL_NET_OBJS}
	${LD} tUdpServer.o ${SRL_COM_OBJS}  ${SRL_TYP_OBJS} ${SRL_NET_OBJS} -o $@ ${LDFLAGS}
tUdpClient: tUdpClient.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS} ${SRL_NET_OBJS}
	${LD} tUdpClient.o ${SRL_COM_OBJS}  ${SRL_TYP_OBJS} ${SRL_NET_OBJS} -o $@ ${LDFLAGS}

tTcpServer: tTcpServer.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS} ${SRL_NET_OBJS}
	${LD} tTcpServer.o ${SRL_COM_OBJS}  ${SRL_TYP_OBJS} ${SRL_NET_OBJS} -o $@ ${LDFLAGS}
tTcpClient: tTcpClient.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS} ${SRL_NET_OBJS}
	${LD} tTcpClient.o ${SRL_COM_OBJS}  ${SRL_TYP_OBJS} ${SRL_NET_OBJS} -o $@ ${LDFLAGS}
tHttpClient: tHttpClient.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS} ${SRL_NET_OBJS}
	${LD} tHttpClient.o ${SRL_COM_OBJS}  ${SRL_TYP_OBJS} ${SRL_NET_OBJS} -o $@ ${LDFLAGS}

tUnixUdpServer: tUnixUdpServer.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS} ${SRL_NET_OBJS}
	${LD} tUnixUdpServer.o ${SRL_COM_OBJS}  ${SRL_TYP_OBJS} ${SRL_NET_OBJS} -o $@ ${LDFLAGS}
tUnixUdpClient: tUnixUdpClient.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS} ${SRL_NET_OBJS}
	${LD} tUnixUdpClient.o ${SRL_COM_OBJS}  ${SRL_TYP_OBJS} ${SRL_NET_OBJS} -o $@ ${LDFLAGS}

tUnixTcpServer: tUnixTcpServer.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS} ${SRL_NET_OBJS}
	${LD} tUnixTcpServer.o ${SRL_COM_OBJS}  ${SRL_TYP_OBJS} ${SRL_NET_OBJS} -o $@ ${LDFLAGS}
tUnixTcpClient: tUnixTcpClient.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS} ${SRL_NET_OBJS}
	${LD} tUnixTcpClient.o ${SRL_COM_OBJS}  ${SRL_TYP_OBJS} ${SRL_NET_OBJS} -o $@ ${LDFLAGS}

# Socket pool: 
tSocketPool: tSocketPool.o ${SRL_COM_OBJS} ${SRL_NET_OBJS}
	${LD} tSocketPool.o ${SRL_COM_OBJS} ${SRL_NET_OBJS} -o $@ ${LDFLAGS}

# Curl: 
tCurl: tCurl.o ${SRL_COM_OBJS} ${SRL_NET_OBJS}
	${LD} tCurl.o ${SRL_COM_OBJS} ${SRL_NET_OBJS} -o $@ ${LDFLAGS}

# mem: 
# 
tBufferFrame: tBufferFrame.o ${SRL_COM_OBJS} ${SRL_MEM_OBJS}
	${LD} tBufferFrame.o ${SRL_COM_OBJS} ${SRL_MEM_OBJS} -o $@ ${LDFLAGS}

tBuffer: tBuffer.o ${SRL_COM_OBJS} ${SRL_MEM_OBJS}
	${LD} tBuffer.o ${SRL_COM_OBJS}  ${SRL_MEM_OBJS} -o $@ ${LDFLAGS}

# app: 
tFileIni: tFileIni.o ${SRL_COM_OBJS} ${SRL_MEM_OBJS} ${SRL_STR_OBJS} ${SRL_APP_OBJS}
	${LD} tFileIni.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS} ${SRL_MEM_OBJS} ${SRL_STR_OBJS} ${SRL_APP_OBJS} -o $@ ${LDFLAGS}
# 
tAppParameter: tAppParameter.o ${SRL_COM_OBJS} ${SRL_MEM_OBJS} ${SRL_STR_OBJS} ${SRL_APP_OBJS}
	${LD} tAppParameter.o ${SRL_COM_OBJS} ${SRL_MEM_OBJS} ${SRL_STR_OBJS} ${SRL_APP_OBJS} -o $@ ${LDFLAGS}

# Server template: 
tTcpServerIterative: tTcpServerIterative.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS} ${SRL_NET_OBJS}
	${LD} tTcpServerIterative.o ${SRL_COM_OBJS}  ${SRL_TYP_OBJS} ${SRL_NET_OBJS} -o $@ ${LDFLAGS}

# str: 
tBufferAnalyzer: tBufferAnalyzer.o ${SRL_COM_OBJS} ${SRL_MEM_OBJS} ${SRL_STR_OBJS}
	${LD} tBufferAnalyzer.o ${SRL_COM_OBJS}  ${SRL_MEM_OBJS} ${SRL_STR_OBJS} -o $@ ${LDFLAGS}

# sip: 
# 
tNameValuePair: tNameValuePair.o ${SRL_COM_OBJS} ${SRL_MEM_OBJS} ${SRL_STR_OBJS} ${SRL_SIP_OBJS}
	${LD} tNameValuePair.o ${SRL_COM_OBJS}  ${SRL_MEM_OBJS} ${SRL_STR_OBJS} ${SRL_SIP_OBJS} -o $@ ${LDFLAGS}
# 
tSipHeader: tSipHeader.o ${SRL_COM_OBJS} ${SRL_MEM_OBJS} ${SRL_STR_OBJS} ${SRL_SIP_OBJS}
	${LD} tSipHeader.o ${SRL_COM_OBJS}  ${SRL_MEM_OBJS} ${SRL_STR_OBJS} ${SRL_SIP_OBJS} -o $@ ${LDFLAGS}
# 
tSipStartLine: tSipStartLine.o ${SRL_COM_OBJS} ${SRL_MEM_OBJS} ${SRL_STR_OBJS} ${SRL_SIP_OBJS}
	${LD} tSipStartLine.o ${SRL_COM_OBJS}  ${SRL_MEM_OBJS} ${SRL_STR_OBJS} ${SRL_SIP_OBJS} -o $@ ${LDFLAGS}
# 
tMessageBase: tMessageBase.o ${SRL_COM_OBJS} ${SRL_MEM_OBJS} ${SRL_STR_OBJS} ${SRL_SIP_OBJS}
	${LD} tMessageBase.o ${SRL_COM_OBJS}  ${SRL_MEM_OBJS} ${SRL_STR_OBJS} ${SRL_SIP_OBJS} -o $@ ${LDFLAGS}
# 
tSipMessage: tSipMessage.o ${SRL_COM_OBJS} ${SRL_MEM_OBJS} ${SRL_STR_OBJS} ${SRL_SIP_OBJS}
	${LD} tSipMessage.o ${SRL_COM_OBJS}  ${SRL_MEM_OBJS} ${SRL_STR_OBJS} ${SRL_SIP_OBJS} -o $@ ${LDFLAGS}

# uti: 


# str: 
tAnalyzer: tAnalyzer.o ${SRL_COM_OBJS} ${SRL_MEM_OBJS} ${SRL_STR_OBJS}
	${LD} tAnalyzer.o ${SRL_COM_OBJS} ${SRL_MEM_OBJS} ${SRL_STR_OBJS} -o $@ ${LDFLAGS}
tStreamAnalyzer: tStreamAnalyzer.o ${SRL_COM_OBJS} ${SRL_MEM_OBJS} ${SRL_STR_OBJS}
	${LD} tStreamAnalyzer.o ${SRL_COM_OBJS} ${SRL_MEM_OBJS} ${SRL_STR_OBJS} -o $@ ${LDFLAGS}

# imf: 
tMessage: tMessage.o ${SRL_COM_OBJS} ${SRL_MEM_OBJS} ${SRL_STR_OBJS} ${SRL_IMF_OBJS}
	${LD} tMessage.o ${SRL_COM_OBJS} ${SRL_MEM_OBJS} ${SRL_STR_OBJS} ${SRL_IMF_OBJS} -o $@ ${LDFLAGS}

# sdp: 
tSdp: tSdp.o ${SRL_COM_OBJS} ${SRL_MEM_OBJS} ${SRL_STR_OBJS} ${SRL_IMF_OBJS} ${SRL_SDP_OBJS}
	${LD} tSdp.o ${SRL_COM_OBJS} ${SRL_MEM_OBJS} ${SRL_STR_OBJS} ${SRL_IMF_OBJS} ${SRL_SDP_OBJS} -o $@ ${LDFLAGS}

# sip: 
tSip: tSip.o ${SRL_COM_OBJS} ${SRL_MEM_OBJS} ${SRL_STR_OBJS} ${SRL_IMF_OBJS} ${SRL_SDP_OBJS} ${SRL_SIP_OBJS}
	${LD} tSip.o ${SRL_COM_OBJS} ${SRL_MEM_OBJS} ${SRL_STR_OBJS} ${SRL_IMF_OBJS} ${SRL_SDP_OBJS} ${SRL_SIP_OBJS} -o $@ ${LDFLAGS}

tSipKeepAlive: tSipKeepAlive.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS} ${SRL_NET_OBJS} ${SRL_MEM_OBJS} ${SRL_STR_OBJS} ${SRL_IMF_OBJS} ${SRL_SDP_OBJS} ${SRL_SIP_OBJS}
	${LD} tSipKeepAlive.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS} ${SRL_NET_OBJS} ${SRL_MEM_OBJS} ${SRL_STR_OBJS} ${SRL_IMF_OBJS} ${SRL_SDP_OBJS} ${SRL_SIP_OBJS} -o $@ ${LDFLAGS}

tSipStress: tSipStress.o ${SRL_COM_OBJS} ${SRL_MEM_OBJS} ${SRL_STR_OBJS} ${SRL_IMF_OBJS} ${SRL_SDP_OBJS} ${SRL_SIP_OBJS}
	${LD} tSipStress.o ${SRL_COM_OBJS} ${SRL_MEM_OBJS} ${SRL_STR_OBJS} ${SRL_IMF_OBJS} ${SRL_SDP_OBJS} ${SRL_SIP_OBJS} -o $@ ${LDFLAGS}

# dbs: 
tMysql: tMysql.o ${SRL_COM_OBJS} ${SRL_DBS_OBJS}
	${LD} tMysql.o ${SRL_COM_OBJS} ${SRL_DBS_OBJS} -o $@ ${LDFLAGS} ${MYSQL_LDFLAGS} ${PTHREAD_LDFLAGS}

# rad: 
tRadPacket: tRadPacket.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS} ${SRL_NET_OBJS} ${SRL_CRY_OBJS} ${SRL_RAD_OBJS}
	${LD} tRadPacket.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS} ${SRL_NET_OBJS} ${SRL_CRY_OBJS} ${SRL_RAD_OBJS} -o $@ ${LDFLAGS}

tRadClient: tRadClient.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS} ${SRL_NET_OBJS} ${SRL_CRY_OBJS} ${SRL_RAD_OBJS}
	${LD} tRadClient.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS} ${SRL_NET_OBJS} ${SRL_CRY_OBJS} ${SRL_RAD_OBJS} -o $@ ${LDFLAGS}

# 
tPoll: tPoll.o  ${SRL_UTI_OBJS} ${SRL_COM_OBJS} ${SRL_MEM_OBJS} ${SRL_NET_OBJS}
	${LD} tPoll.o ${SRL_UTI_OBJS} ${SRL_COM_OBJS}  ${SRL_MEM_OBJS} ${SRL_NET_OBJS}  -o $@ ${LDFLAGS} -lpthread

tPool: tPool.o  ${SRL_UTI_OBJS} ${SRL_COM_OBJS} ${SRL_MEM_OBJS} ${SRL_NET_OBJS}
	${LD} tPool.o ${SRL_UTI_OBJS} ${SRL_COM_OBJS}  ${SRL_MEM_OBJS} ${SRL_NET_OBJS}  -o $@ ${LDFLAGS} -lpthread

# 
tStorage: tStorage.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS} ${SRL_CRY_OBJS} ${SRL_DBS_OBJS} ../tsp/CStorage.o
	${LD} tStorage.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS} ${SRL_CRY_OBJS} ${SRL_DBS_OBJS} ../tsp/CStorage.o -o $@ ${LDFLAGS} ${MYSQL_LDFLAGS} ${PTHREAD_LDFLAGS}

# 
tThread: tThread.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS} ${SRL_THR_OBJS}
	${LD} tThread.o ${SRL_COM_OBJS} ${SRL_TYP_OBJS} ${SRL_THR_OBJS} -o $@ ${LDFLAGS} ${MYSQL_LDFLAGS} ${PTHREAD_LDFLAGS}

# 
tThreadWorkerTest: tThreadWorkerTest.o CThreadWorkerTest.o ${SRL_COM_OBJS} ${SRL_NET_OBJS} ${SRL_THR_OBJS} 
	${LD} tThreadWorkerTest.o CThreadWorkerTest.o ${SRL_COM_OBJS} ${SRL_NET_OBJS} ${SRL_THR_OBJS} -o $@ ${LDFLAGS} ${PTHREAD_LDFLAG}

# 
tTcpServerMultiThread: tTcpServerMultiThread.o CThreadWorkerTcp.o ${SRL_COM_OBJS} ${SRL_NET_OBJS} ${SRL_THR_OBJS} 
	${LD} tTcpServerMultiThread.o CThreadWorkerTcp.o ${SRL_COM_OBJS} ${SRL_NET_OBJS} ${SRL_THR_OBJS} -o $@ ${LDFLAGS} ${PTHREAD_LDFLAG}
