#include <stdio.h>
// 
#include "CException.h"
#include "CMd5.h"

/*
./tMd5 qwertyuiopasdfghjklzxcvbnm1234567890
SELECT MD5('qwertyuiopasdfghjklzxcvbnm1234567890');
*/
int main(int argc, char* argv[])
{
    try
    {
        if(argc < 2)
        {
            printf("Usage: %s <string>\n", argv[0]);
            return(0);
        }
        // 
        uint8 md5sum[16];
        char szHex[MD5_DIGEST_HEX_LEN+1];
        CMd5        objMd5;
        // Digest: 
        objMd5.Start();
        objMd5.Update((unsigned char*)argv[1], strlen(argv[1]));
        objMd5.GetDigest(md5sum, sizeof(md5sum));
        // 
        for(int j = 0; j < 16; j++)
        {
            printf("%02x", md5sum[j]);
        }
        printf("\n");
        // DigestToHex: 
        objMd5.DigestToHex(md5sum, sizeof(md5sum), szHex, sizeof(szHex));
        printf("ToHex: [%s]\n", szHex);
        
        // DigestHex: 
        objMd5.Start();
        objMd5.Update((unsigned char*)argv[1], strlen(argv[1]));
        objMd5.GetDigestHex(szHex, sizeof(szHex));
        printf("GetHex: [%s]\n", szHex);
        
/*
        // Test MD5 update: 
        printf("\n");
        objMd5.Start();
        objMd5.Update((unsigned char*)"huangtx2004", strlen("huangtx2004"));
        objMd5.Update((unsigned char*)":", 1);
        objMd5.Update((unsigned char*)"74.13.233.144", strlen("74.13.233.144"));
        objMd5.Update((unsigned char*)":", 1);
        objMd5.Update((unsigned char*)"test2005", strlen("test2005"));
        // 
        objMd5.GetDigestHex(szHex, sizeof(szHex));
        printf("GetHex: [%s]\n", szHex);
        
        // Test once and more than once: 
        printf("Update once: \n");
        objMd5.Start();
        objMd5.Update((unsigned char*)"mysql", strlen("mysql"));
        objMd5.GetDigestHex(szHex, sizeof(szHex));
        printf("GetHex: [%s]\n", szHex);
        //
        printf("Update twice: \n");
        objMd5.Start();
        objMd5.Update((unsigned char*)"my", strlen("my"));
        objMd5.Update((unsigned char*)"sql", strlen("sql"));
        objMd5.GetDigestHex(szHex, sizeof(szHex));
        printf("GetHex: [%s]\n", szHex);
        
        // Test radius case: 
        unsigned char szPacket[128] = {0x02, 0xa4, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        printf("Packet: [");
        for(int i = 0; i < 20; i++) printf("%02x", szPacket[i]);
        printf("]\n");
        // Update twice: 
        objMd5.Start();
        objMd5.Update((unsigned char*)szPacket, 20);
        objMd5.Update((unsigned char*)"vbuzzer", strlen("vbuzzer"));
        objMd5.GetDigestHex(szHex, sizeof(szHex));
        printf("GetHex: [%s]\n", szHex);
        // Update once: 
        strcpy((char*)&szPacket[20], "vbuzzer");
        objMd5.Start();
        objMd5.Update((unsigned char*)szPacket, 27);
        objMd5.GetDigestHex(szHex, sizeof(szHex));
        printf("GetHex: [%s]\n", szHex);
*/
    }catch(CException& eEx)
    {
        printf("%s\n", eEx.GetMessageStack().c_str());
        return(-1);
    }
}
