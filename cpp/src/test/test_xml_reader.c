/*
g++ -I/usr/include/libxml2 test_xml_reader.c -lxml2
*/

#include <string.h>
#include <stdio.h>
#include <libxml/xmlreader.h>

void processNode(xmlTextReaderPtr reader) 
{
    /* handling of a node in the tree */
/*
    xmlChar *name, *value;
    name = xmlTextReaderName(reader);
    if(NULL != name) 
    {
        printf("Name: %s: %d\n", name, xmlTextReaderNodeType(reader));
        xmlFree(name);
    }
    value = xmlTextReaderValue(reader);
    if(NULL != value) 
    {
        printf("Value: %s\n", value);
        xmlFree(value);
    }
*/
    xmlChar *name, *value;

    name = xmlTextReaderName(reader);
    if (name == NULL)
        name = xmlStrdup(BAD_CAST "--");
    value = xmlTextReaderValue(reader);

    printf("Depth: %d Type: %d Name: %s Empty: %d",
            xmlTextReaderDepth(reader),
            xmlTextReaderNodeType(reader),
            name,
            xmlTextReaderIsEmptyElement(reader));
    xmlFree(name);
    if (value == NULL)
        printf("\n");
    else {
        printf(" %s\n", value);
        xmlFree(value);
    }

}

int streamFile(char *filename) 
{
    xmlTextReaderPtr reader;
    int ret;

    char szXml[2048];
    // 
    strcpy(szXml, "<api><status><code>200</code><message>OK</message></status><data><client><ip>74.13.233.143</ip><secret>1w3S5x7O9k2N4t6G8v0</secret></client></data></api>");
    reader = xmlReaderForMemory(szXml, strlen(szXml), NULL, NULL, 0);
    //reader = xmlNewTextReaderFilename(filename);
    if (reader != NULL) {
        ret = xmlTextReaderRead(reader);
        while (ret == 1) {
            processNode(reader);
            ret = xmlTextReaderRead(reader);
        }
        xmlFreeTextReader(reader);
        if (ret != 0) {
            printf("%s : failed to parse\n", filename);
        }
    } else {
        printf("Unable to open %s\n", filename);
    }
}

int main(int argc, char **argv)
{
    streamFile("test_xml1.xml");
    exit(0);
}
